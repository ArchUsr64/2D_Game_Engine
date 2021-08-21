#include "/usr/include/SDL2/SDL.h"
#include <cmath>
#include <fstream>
#include <iostream>

int const width = 600;
int const height = 400;
int pixelArray[width][height][3];
int fillColor[] = { 255, 255, 255, 255 };
int bgColor[] = { 204, 204, 204 };
int defaultScreenSize[] = { 640, 480 };
int strokeColor[] = { 0, 0, 0, 255 };
bool isFill = true;
bool isStroke = true;
float textSize = 16;
int fontAtlas[1024][384];

void loadFont() {
  std::ifstream fontFile;
  fontFile.open("font.ppm", std::fstream::in);
  int i = -3;
  std::string word;
  while(fontFile >> word){
    if(i>0){
      fontAtlas[i%1024][i/1024] = int(word);
      if(int(word)==255){
        std::cout << "Value is 255 at: " << i+3 << std::endl;
      }
    }
    i++;
  }
  

}

void background(int a, int b, int c) {
  bgColor[0] = a;
  bgColor[1] = b;
  bgColor[2] = c;
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      for (int c = 0; c < 3; c++) {
        pixelArray[i][j][c] = bgColor[c];
      }
    }
  }
}

void size(int localWidth, int localHeight) {
  background(bgColor[0], bgColor[1], bgColor[2]);
}

void stroke(int a, int b, int c, int d = 255) {
  isStroke = true;
  strokeColor[0] = a;
  strokeColor[1] = b;
  strokeColor[2] = c;
  strokeColor[3] = d;
}

void noStroke() {
  isStroke = false;
}

void fill(int a, int b, int c, int d = 255) {
  isFill = true;
  fillColor[0] = a;
  fillColor[1] = b;
  fillColor[2] = c;
  fillColor[3] = d;
}

void noFill() {
  isFill = false;
}

float reverseTransformationSRGBtoCIEXYZ(int num) {
  float color;
  color = num / 255;
  if (color <= 0.04045) {
    return color / 12.92;
  }
  else {
    return pow((color + 0.055) / 1.055, 2.4);
  }
}

int forwardTransformationCIEXYZtoSRGB(float num) {
  if (num <= 0.0031308) {
    return 255 * 12.92 * num;
  }
  else {
    return 255 * ((1.055 * pow(num, 1 / 2.4)) - 0.055);
  }
}

void applyAlphaCompositing(int i, int j, int colorChannel, bool isLine = false) {
  if (!isLine) {
    pixelArray[i][j][colorChannel] = forwardTransformationCIEXYZtoSRGB(
      (reverseTransformationSRGBtoCIEXYZ(fillColor[colorChannel]) *
        (fillColor[3] / 255.0) +
        reverseTransformationSRGBtoCIEXYZ(pixelArray[i][j][colorChannel]) *
        (1 - (fillColor[3] / 255.0))));
  }
  else {
    pixelArray[i][j][colorChannel] = forwardTransformationCIEXYZtoSRGB(
      (reverseTransformationSRGBtoCIEXYZ(strokeColor[colorChannel]) *
        (strokeColor[3] / 255.0) +
        reverseTransformationSRGBtoCIEXYZ(pixelArray[i][j][colorChannel]) *
        (1 - (strokeColor[3] / 255.0))));
  }
}

void point(int posX, int posY, bool isLine = false) {
  if (isFill && !isLine) {
    if (fillColor[3] != 255) {
      for (int a = 0; a < 3; a++) {
        applyAlphaCompositing(posX, posY, a);
      }
    }
    else {
      for (int a = 0; a < 3; a++) {
        pixelArray[posX][posY][a] = fillColor[a];
      }
    }
  }
  if (isLine) {
    if (strokeColor[3] != 255) {
      for (int a = 0; a < 3; a++) {
        applyAlphaCompositing(posX, posY, a, true);
      }
    }
    else {
      for (int a = 0; a < 3; a++)
      {
        pixelArray[posX][posY][a] = strokeColor[a];
      }
    }
  }
}

void line(int xI, int yI, int xF, int yF) {
  if (isStroke) {
    if ((xF != xI) && (yF != yI)) {
      float slope = (yF - yI) * 1.0 / (xF - xI);
      for (int i = std::min(xI, xF); i <= std::max(xF, xI); i++) {
        int j = ((i - xI) * slope) + yI;
        point(i, j, true);
      }

      for (int j = std::min(yI, yF); j <= std::max(yF, yI); j++) {
        int i = ((j - yI) / slope) + xI;
        point(i, j, true);
      }
    }

    if (xF == xI) {
      for (int j = std::min(yI, yF); j <= std::max(yF, yI); j++) {
        point(xI, j, true);
      }
    }
    if (yF == yI) {
      for (int i = std::min(xI, xF); i <= std::max(xF, xI); i++) {
        point(i, yI, true);
      }
    }
  }
}

void rect(int posX, int posY, int sizeX, int sizeY) {
  for (int i = posX; i < sizeX + posX; i++) {
    for (int j = posY; j < sizeY + posY; j++) {
      point(i, j);
    }
  }

  line(posX, posY, sizeX + posX, posY);
  line(posX, posY, posX, sizeY + posY);
  line(posX + sizeX, posY + sizeY, posX, sizeY + posY);
  line(posX + sizeX, posY + sizeY, sizeX + posX, posY);
}

float triangleArea(int x1, int y1, int x2, int y2, int x3, int y3) {
  return std::abs(0.5 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)));
}

void triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
  float area = triangleArea(x1, y1, x2, y2, x3, y3);
  for (int i = std::min(x1, std::min(x2, x3));
    i <= std::max(x1, std::max(x2, x3)); i++) {
    for (int j = std::min(y1, std::min(y2, y3)); j <= std::max(y1, std::max(y2, y3)); j++) {
      if (isFill) {
        if (triangleArea(i, j, x2, y2, x3, y3) + triangleArea(x1, y1, i, j, x3, y3) + triangleArea(x1, y1, x2, y2, i, j) == area) {
          point(i, j);
        }
      }
    }
  }
  line(x1, y1, x2, y2);
  line(x1, y1, x3, y3);
  line(x2, y2, x3, y3);
}

void text(char toPrint[], int xLoc, int yLoc) {
  int charGap = floor(textSize / 10.0);
  int textMap[sizeof(toPrint) * 32][32];
  for (int i = 0; i < sizeof(toPrint); i++) {
    int charCode = int(toPrint[i]);
    int charPosX = (charCode % 16) * 64;
    int charPosY = (charCode / 16) * 64;
    for (int k = 0; k < 64; k++) {
      for (int j = 0; j < 64; j++) {
        textMap[i * 64 + k][j] = fontAtlas[charPosX + k][charPosY + j];
      }
    }
  }
}
void clear() {
  // Clears the screen by replacing every pixel value to background
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      for (int c = 0; c < 3; c++) {
        pixelArray[i][j][c] = bgColor[c];
      }
    }
  }
}

void makeFile() {
  // Makes a ppm file
  remove("image.ppm");
  std::ofstream imageFile;
  imageFile.open("image.ppm");
  imageFile << "P3\n";
  imageFile << width << " " << height << std::endl;
  imageFile << "255\n";
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      imageFile << pixelArray[i][j][0] << " " << pixelArray[i][j][1] << " " << pixelArray[i][j][2] << std::endl;
    }
  }
}

bool running = true;

void render() {
  // I got no idea how this function works but it does work
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "Failed to initialize the SDL2 library\n";
  }

  SDL_Window* window =
    SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED, width, height, 0);

  if (!window) {
    std::cout << "Failed to create window\n";
  }

  // Setup renderer
  SDL_Renderer* renderer = NULL;
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

  SDL_RenderClear(renderer);

  SDL_Delay(10);
  SDL_Event ev;

  // Main loop
  while (running) {
    for (int i = 0; i < width; i++) {
      for (int j = 0; j < height; j++) {
        SDL_SetRenderDrawColor(renderer, pixelArray[i][j][0], pixelArray[i][j][1], pixelArray[i][j][2], 255);
        SDL_RenderDrawPoint(renderer, i, j);
      }
    }
    SDL_RenderPresent(renderer);          // Event loop
    while (SDL_PollEvent(&ev) != 0) {    // check event type
      switch (ev.type) {
      case SDL_QUIT:                     // shut down
        running = false;
        break;
      }
    }
  }
}