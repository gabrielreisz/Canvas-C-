#include "Canvas.hpp"
#include <algorithm>
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
    Canva.pixels = nullptr;
    Canva.cores = nullptr;
    Canva.altura = 0;
    Canva.largura = 0;
    return;
  }
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

void DestruirCanvas(Canva &Canva) {
  if (Canva.pixels == nullptr) {
    return;
  }
  for (int i = 0; i < Canva.altura; i++) {
    delete[] Canva.pixels[i];
    delete[] Canva.cores[i];
  }
  delete[] Canva.pixels;
  delete[] Canva.cores;
  Canva.pixels = nullptr;
  Canva.cores = nullptr;
}

void ImprimirCanva(Canva &Canva) {
  if (Canva.pixels == nullptr) {
    return;
  }

  for (int i = 0; i <= Canva.largura; i++) {
    std::cout << "-";
  }
 

  for (int i = 0; i < Canva.altura; i++) {
    std::cout << "|";
    for (int j = 0; j < Canva.largura; j++) {
      std::cout << GetColorCode(Canva.cores[i][j]) << Canva.pixels[i][j];
    }
    std::cout << GetColorCode('W') << "|" << std::endl;
  }


  for (int i = 0; i <= Canva.largura; i++) {
    std::cout << "-";
  }

}

void DesenharPonto(Canva &Canva, int x, int y, char pixel, char cores) {
  if (x < 0 || x >= Canva.largura || y < 0 || y >= Canva.altura) {
    return;
  }
  Canva.pixels[y][x] = pixel;
  Canva.cores[y][x] = cores;
}

void DesenharLinha(Canva &Canva, int x1, int y1, int x2, int y2, char pixel,
                   char cores) {
  if (x1 == x2) {
    if (y1 > y2)
      std::swap(y1, y2);
    for (int y = y1; y <= y2; y++) {
      DesenharPonto(Canva, x1, y, pixel, cores);
    }
  } else if (y1 == y2) {
    if (x1 > x2)
      std::swap(x1, x2);
    for (int x = x1; x <= x2; x++) {
      DesenharPonto(Canva, x, y1, pixel, cores);
    }
  } else {
    std::cout << GetColorCode('R');
    std::cout << "Error: A linha nao forma uma horizontal ou vertical"
              << std::endl;
  }
}

void DesenharRetangulo(Canva &Canva, int x_superior_esquerdo,
                       int y_superior_esquerdo, int largura, int altura,
                       char pixel, char cores) {
  int x2 = x_superior_esquerdo + largura - 1;
  int y2 = y_superior_esquerdo + altura - 1;

  DesenharLinha(Canva, x_superior_esquerdo, y_superior_esquerdo, x2, y_superior_esquerdo, pixel, cores);
  DesenharLinha(Canva, x_superior_esquerdo, y2, x2, y2, pixel, cores);
  DesenharLinha(Canva, x_superior_esquerdo, y_superior_esquerdo, x_superior_esquerdo, y2, pixel, cores);
  DesenharLinha(Canva, x2, y_superior_esquerdo, x2, y2, pixel, cores);
}

void DesenharRetanguloPreenchido(Canva &Canva, int x_superior_esquerdo,
                                 int y_superior_esquerdo, int largura,
                                 int altura, char pixel, char cores) {
  for (int i = 0; i < altura; i++) {
    for (int j = 0; j < largura; j++) {
      DesenharPonto(Canva, x_superior_esquerdo + j, y_superior_esquerdo + i, pixel, cores);
    }
  }
}

void RedimensionarCanva(Canva &Canva, int novaLargura, int novaAltura) {
  struct Canva nova_tela;
  CriarCanva(nova_tela, novaAltura, novaLargura);

  if (nova_tela.pixels == nullptr) {
    return;
  }

  int altura_comum = std::min(Canva.altura, novaAltura);
  int largura_comum = std::min(Canva.largura, novaLargura);

  for (int i = 0; i < altura_comum; i++) {
    for (int j = 0; j < largura_comum; j++) {
      nova_tela.pixels[i][j] = Canva.pixels[i][j];
      nova_tela.cores[i][j] = Canva.cores[i][j];
    }
  }

  DestruirCanvas(Canva);
  Canva = nova_tela;
}

void SobreporCanvas(Canva &Canva_destino, Canva &Origem1, Canva &Origem2) {
  if (Origem1.largura != Origem2.largura || Origem1.altura != Origem2.altura) {
    return;
  }
  CriarCanva(Canva_destino, Origem1.altura, Origem1.largura);

  for (int i = 0; i < Origem1.altura; i++) {
    for (int j = 0; j < Origem1.largura; j++) {
      Canva_destino.pixels[i][j] = Origem1.pixels[i][j];
      Canva_destino.cores[i][j] = Origem1.cores[i][j];
      if (Origem2.pixels[i][j] != ' ') {
        Canva_destino.pixels[i][j] = Origem2.pixels[i][j];
        Canva_destino.cores[i][j] = Origem2.cores[i][j];
      }
    }
  }
}

bool CompararCanvas(Canva &Canva1, Canva &Canva2) {
  if (Canva1.largura != Canva2.largura || Canva1.altura != Canva2.altura) {
    return false;
  }

  for (int i = 0; i < Canva1.altura; i++) {
    for (int j = 0; j < Canva1.largura; j++) {
      if (Canva1.pixels[i][j] != Canva2.pixels[i][j] ||
          Canva1.cores[i][j] != Canva2.cores[i][j]) {
        return false;
      }
    }
  }
  return true;
}