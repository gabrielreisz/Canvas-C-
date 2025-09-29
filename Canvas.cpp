#include "Canvas.hpp"
#include <iostream>
std::string Canva::GetColorCode(char cor) {
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
Canva::~Canva() {
    if (pixels != nullptr) {
        for (int i = 0; i < altura; i++) {
            delete[] pixels[i];
            delete[] cores[i];
        }
        delete[] pixels;
        delete[] cores;
        pixels = nullptr;
        cores = nullptr;
    }
}
int Canva::getaltura() const { return this->altura; }
int Canva::getlargura() const { return this->largura; }
char **Canva::getpixels() const { return this->pixels; }
char **Canva::getcores() const { return this->cores; }
void Canva::setaltura(int altura) { this->altura = altura; }
void Canva::setlargura(int largura) { this->largura = largura; }
void Canva::setpixels(char **pixels) { this->pixels = pixels; }
void Canva::setcores(char **cores) { this->cores = cores; }

void CriarCanva(Canva &canva, int altura, int largura) {
    canva.setaltura(altura);
    canva.setlargura(largura);
    
    char **novosPixels = new char*[altura];
    char **novasCores = new char*[altura];
    
    for (int i = 0; i < altura; i++) {
        novosPixels[i] = new char[largura];
        novasCores[i] = new char[largura];
        for (int j = 0; j < largura; j++) {
            novosPixels[i][j] = ' ';
            novasCores[i][j] = 'W';
        }
    }
    
    canva.setpixels(novosPixels);
    canva.setcores(novasCores);
}
