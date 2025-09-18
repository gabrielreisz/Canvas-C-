#include "Canvas.hpp"
#include <iostream>
std::string GetColorCode(char cor) {
  switch (cor) {
  case 'K':
    return "\033[30m"; // Black
  case 'R':
    return "\033[31m"; // Red
  case 'G':
    return "\033[32m"; // Green
  case 'Y':
    return "\033[33m"; // Yellow
  case 'B':
    return "\033[34m"; // Blue
  case 'M':
    return "\033[35m"; // Magenta
  case 'C':
    return "\033[36m"; // Cyan
  case 'W':
    return "\033[37m"; // White
  default:
    return "\033[0m"; // Código para Resetar/Padrão
  }
}

Canva::Canva() {
  pixels = nullptr;
  cores = nullptr;
}
Canva::~Canva() {}
int Canva::getaltura() { return this->altura; }
int Canva::getlargura() { return this->largura; }
char **Canva::getpixels() { return this->pixels; }
char **Canva::getcores() { return this->cores; }
void Canva::setaltura(int altura) { this->altura = altura; }
void Canva::setlargura(int largura) { this->largura = largura; }
void Canva::setpixels(char pixels) { this->pixels = **pixels; }
void Canva::setcores(char cores) { this->cores = **cores; }

void CriarCanva(Canva &canva, int altura, int largura) {
  if (altura <= 0 || largura <= 0) {
    std::cout << "Error: Altura ou Largura invalidas" << std::endl;
    return;
  } else {
    canva.setaltura(altura);
    canva.setlargura(largura);
    pixels = new char *[altura];
    cores = new char *[altura];
    for (int i = 0; i < altura; i++) {
      pixels[i] = new char[largura];
      cores[i] = new char[largura];
      for (int j = 0; j < largura; j++) {
        canva.setpixels(' ');
        canva.setcores('W');
      }
    }
  }
}

void DestruirCanvas(Canva &canva) {
  if (canva.getpixels != nullptr) {
    for (int i = 0; i < canva._altura; i++) {
      delete[] canva._pixels[i];
    }
    delete[] canva._pixels;
    canva._pixels = nullptr;
  }

  if (canva._cores != nullptr) {
    for (int i = 0; i < canva._altura; i++) {
      delete[] canva._cores[i];
    }
    delete[] canva._cores;
    canva._cores = nullptr;
  }

  canva._altura = 0;
  canva._largura = 0;
}
void ImprimirCanva(Canva &Canva) {
  std::cout << "+";
  for (int i = 0; i < _largura; i++) {
    std::cout << "-";
  }
  std::cout << "+" << std::endl;
  for (int i = 0; i < _altura; i++) {
    std::cout << "|";
    for (int j = 0; j < _largura; j++) {
      std::cout << GetColorCode(Canva._cores[i][j]);
      std::cout << Canva._pixels[i][j];
    }
    std::cout << "|" << std::endl;
  }
  std::cout << "+";
  for (int i = 0; i < _largura; i++) {
    std::cout << "-";
  }
  std::cout << "+" << std::endl;
}
void DesenharPonto(Canva &Canva, int x, int y, char pixel, char cores) {
  if (x > 0 && x < Canva._largura && y > 0 && y < Canva._altura) {
    Canva._cores[x][y] = cores;
    Canva._pixels[x][y] = pixel;
  } else {
    std::cout << "X ou Y estao fora do limite do Canvas!" << std::endl;
  }
}
void DesenharLinha(Canva &Canva, int x1, int y1, int x2, int y2, char pixel,
                   char cores) {
  for (int i = x1; i < x2; i++) {
    for (int j = y1; j < y2; j++) {
      Canva._pixels[i][j] = pixel;
      Canva._cores[i][j] = cores;
    }
  }
}
void DesenharRetangulo(Canva &Canva, int x_superior_esquerdo,
                       int y_superior_esquerdo, int largura, int altura,
                       char pixel, char cores) {}
