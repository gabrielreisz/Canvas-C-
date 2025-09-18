#include "Canvas.hpp"
#include <cstddef>
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
void CriarCanva(Canva &Canva, int altura, int largura) {
  if (altura <= 0 || largura <= 0) {
    std::cout << GetColorCode('R');
    std::cout << "Error: Altura ou Largura invalida" << std::endl;
  } else {
    Canva.altura = altura;
    Canva.largura = largura;
    Canva.pixels = new char *[altura];
    Canva.cores = new char *[altura];
    for (int i = 0; i < altura; i++) {
      Canva.pixels[i] = new char[largura];
      Canva.cores[i] = new char[largura];

      for (int j = 0; j < largura; j++) {
        Canva.pixels[i][j] = ' ';
        Canva.cores[i][j] = 'W';
      }
    }
  }
}
void DestruirCanvas(Canva &Canva) {
  for (int i = 0; i < Canva.largura; i++) {
    delete[] Canva.pixels[i];
  }
  delete[] Canva.pixels;
}
void ImprimirCanva(Canva &Canva) {
  for (int i = 0; i < Canva.largura; i++) {
    std::cout << "-";
  }
  std::cout << "\n";
  for (int i = 0; i < Canva.altura; i++) {
    for (int j = 0; j < Canva.largura; j++) {
      std::cout << "|";
      std::cout << GetColorCode(Canva.cores[i][j]) << std::endl;
      std::cout << Canva.pixels[i][j] << std::endl;

      std::cout << "|";
    }
    std::cout << "\n";
  }
}

void DesenharPonto(Canva &Canva, int x, int y, char pixel, char cores) {
  if (x < 0 || x > Canva.altura || y < 0 || y > Canva.largura) {
    std::cout << GetColorCode('R');
    std::cout << "Error: X ou Y esta fora dos limites do Canva" << std::endl;
  }
  Canva.pixels[x][y] = pixel;
  Canva.cores[x][y] = cores;
}

void DesenharLinha(Canva &Canva, int x1, int y1, int x2, int y2, char pixel,
                   char cores) {
  if (x1 < 0 || x1 > Canva.altura || x2 < 0 || x2 < Canva.altura || y1 < 0 ||
      y1 > Canva.largura || y2 < 0 || y2 > Canva.largura) {

    std::cout << GetColorCode('R');
    std::cout << "Error: X ou Y esta fora dos limites do Canva" << std::endl;
  } else {
    if (x1 == x2 || y1 == y2) {
      for (int i = x1; i <= x2; i++) {
        for (int j = y1; j <= y2; j++) {
          Canva.cores[i][j] = cores;
          Canva.pixels[i][j] = pixel;
        }
      }
    } else {
      std::cout << GetColorCode('R');
      std::cout << "Error: A linha nao forma uma horizontal ou vertical"
                << std::endl;
    }
  }
}
void DesenharRetangulo(Canva &Canva, int x_superior_esquerdo,
                       int y_superior_esquerdo, int largura, int altura,
                       char pixel, char cores) {
  if (x_superior_esquerdo < 0 || x_superior_esquerdo > Canva.altura ||
      y_superior_esquerdo < 0 || y_superior_esquerdo > Canva.largura) {

    std::cout << GetColorCode('R');
    std::cout << "Error: X ou Y esta fora dos limites do Canva" << std::endl;
  } else {
    if (x_superior_esquerdo + altura > Canva.altura ||
        y_superior_esquerdo + largura > Canva.largura) {

      std::cout << GetColorCode('R');
      std::cout << "Error: X+altura ou Y+largura esta fora dos limites do Canva"
                << std::endl;
    } else {
      for (int i = x_superior_esquerdo; i < x_superior_esquerdo + altura; i++) {
        for (int j = y_superior_esquerdo; j < y_superior_esquerdo + largura;
             j++) {
          if (i == x_superior_esquerdo || j == y_superior_esquerdo) {
            Canva.pixels[i][j] = pixel;
            Canva.cores[i][j] = cores;
          }
        }
      }
    }
  }
}
