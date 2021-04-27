#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>

int const width = 600;
int const height = 400;
int pixelArray[width][height][3];
int fillColor[] = {255, 255, 255, 255};
int bgColor[] = {204, 204, 204};
int defaultScreenSize[] = {100, 100};
int strokeColor[] = {0, 0, 0, 255};
bool isFill = true;
bool isStroke = true;

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

void noStroke() { isStroke = false; }

void fill(int a, int b, int c, int d = 255) {
  isFill = true;
  fillColor[0] = a;
  fillColor[1] = b;
  fillColor[2] = c;
  fillColor[3] = d;
}

void noFill() { isFill = false; }

void initializer() {
  size(defaultScreenSize[0], defaultScreenSize[1]);
  background(bgColor[0], bgColor[1], bgColor[2]);
  stroke(strokeColor[0], strokeColor[1], strokeColor[2]);
  fill(fillColor[0], fillColor[1], fillColor[2]);
}

void point(int posX, int posY) {
  if (isFill) {
    for (int a = 0; a < 3; a++) {
      pixelArray[posX][posY][a] =
          (fillColor[a] * (fillColor[3] / 255.0) +
           pixelArray[posX][posY][a] * (1 - (fillColor[3] / 255.0)));
    }
  }
}

void line(int xI, int yI, int xF, int yF) {
  if (isStroke) {
    if ((xF != xI) && (yF != yI)) {
      float slope = (yF - yI) * 1.0 / (xF - xI);
      for (int i = std::min(xI, xF); i <= std::max(xF, xI); i++) {
        int j = ((i - xI) * slope) + yI;
        for (int a = 0; a < 3; a++) {
          pixelArray[i][j][a] =
              (strokeColor[a] * (strokeColor[3] / 255.0) +
               pixelArray[i][j][a] * (1 - (strokeColor[3] / 255.0)));
        }
      }
      for (int j = std::min(yI, yF); j <= std::max(yF, yI); j++) {
        int i = ((j - yI) / slope) + xI;
        for (int a = 0; a < 3; a++) {
          pixelArray[i][j][a] =
              (strokeColor[a] * (strokeColor[3] / 255.0) +
               pixelArray[i][j][a] * (1 - (strokeColor[3] / 255.0)));
        }
      }
    }
    if (xF == xI) {
      for (int j = std::min(yI, yF); j <= std::max(yF, yI); j++) {
        for (int a = 0; a < 3; a++) {
          pixelArray[xI][j][a] =
              (strokeColor[a] * (strokeColor[3] / 255.0) +
               pixelArray[xI][j][a] * (1 - (strokeColor[3] / 255.0)));
        }
      }
    }
    if (yF == yI) {
      for (int i = std::min(xI, xF); i <= std::max(xF, xI); i++) {
        for (int a = 0; a < 3; a++) {
          pixelArray[i][yI][a] =
              (strokeColor[a] * (strokeColor[3] / 255.0) +
               pixelArray[i][yI][a] * (1 - (strokeColor[3] / 255.0)));
        }
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
  if (isStroke) {
    line(posX, posY, sizeX + posX, posY);
    line(posX, posY, posX, sizeY + posY);
    line(posX + sizeX, posX + sizeY, posX, sizeY + posY);
    line(posX + sizeX, posY + sizeY, sizeX + posX, posY);
  }
}

float triangleArea(int x1, int y1, int x2, int y2, int x3, int y3) {
  return std::abs(0.5 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)));
}

void triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
  float area = triangleArea(x1, y1, x2, y2, x3, y3);
  for (int i = std::min(x1, std::min(x2, x3));
       i <= std::max(x1, std::max(x2, x3)); i++) {
    for (int j = std::min(y1, std::min(y2, y3));
         j <= std::max(y1, std::max(y2, y3)); j++) {
      if (isFill) {
        if (triangleArea(i, j, x2, y2, x3, y3) +
                triangleArea(x1, y1, i, j, x3, y3) +
                triangleArea(x1, y1, x2, y2, i, j) ==
            area) {
          point(i, j);
        }
      }
    }
  }
  line(x1, y1, x2, y2);
  line(x1, y1, x3, y3);
  line(x2, y2, x3, y3);
}

void clear(){
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      for (int c = 0; c < 3; c++) {
        pixelArray[i][j][c] = bgColor[c];
      }
    }
  }
}

void makeFile() {
  remove("image.ppm");
  std::ofstream imageFile;
  imageFile.open("image.ppm");
  imageFile << "P3\n";
  imageFile << width << " " << height << std::endl;
  imageFile << "255\n";
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      imageFile << pixelArray[i][j][0] << " " << pixelArray[i][j][1] << " "
                << pixelArray[i][j][2] << std::endl;
    }
  }
}