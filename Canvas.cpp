#include "Canvas.hpp"

std::string GetColorCode(char cor)
{
  switch (cor)
  {
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

Canva::Canva()
{
  _pixels = nullptr;
  _cores = nullptr;
}
Canva::~Canva()
{
}

void Canva::CriarCanva(Canva &canva, int altura, int largura)
{
  if (altura <= 0 || largura <= 0)
  {
    std::cout << "Error: Altura ou Largura invalidas" << std::endl;
    return;
  }
  else
  {
    _altura = altura;
    _largura = largura;
    _pixels = new char *[_altura];
    _cores = new char *[_altura];
    for (int i = 0; i < _altura; i++)
    {
      _pixels[i] = new char[_largura];
      _cores[i] = new char[_largura];
      for (int j = 0; j < _largura; j++)
      {
        _pixels[i][j] = ' ';
        _cores[i][j] = 'W';
      }
    }
  }
}

void Canva::DestruirCanvas(Canva &canva)
{
  if (canva._pixels != nullptr)
  {
    for (int i = 0; i < canva._altura; i++)
    {
      delete[] canva._pixels[i];
    }
    delete[] canva._pixels;
    canva._pixels = nullptr;
  }

  if (canva._cores != nullptr)
  {
    for (int i = 0; i < canva._altura; i++)
    {
      delete[] canva._cores[i];
    }
    delete[] canva._cores;
    canva._cores = nullptr;
  }

  canva._altura = 0;
  canva._largura = 0;
}
void Canva::ImprimirCanva(Canva &Canva)
{
  std::cout << "+";
  for (int i = 0; i < _largura; i++)
  {
    std::cout << "-";
  }
  std::cout << "+" << std::endl;
  for (int i = 0; i < _altura; i++)
  {
    std::cout << "|";
    for (int j = 0; j < _largura; j++)
    {
      std::cout << GetColorCode(Canva._cores[i][j]);
      std::cout << Canva._pixels[i][j];
    }
    std::cout << "|" << std::endl;
  }
  std::cout << "+";
  for (int i = 0; i < _largura; i++)
  {
    std::cout << "-";
  }
  std::cout << "+" << std::endl;
}
void Canva::DesenharPonto(Canva &Canva, int x, int y, char pixel, char cores)
{
  if (x > 0 && x < Canva._largura && y > 0 && y < Canva._altura)
  {
    Canva._cores[x][y] = cores;
    Canva._pixels[x][y] = pixel;
  }
  else
  {
    std::cout << "X ou Y estao fora do limite do Canvas!" << std::endl;
  }
}
void Canva::DesenharLinha(Canva &Canva, int x1, int y1, int x2, int y2, char pixel,
                          char cores)
{
  for (int i = x1; i < x2; i++)
  {
    for (int j = y1; j < y2; j++)
    {
      Canva._pixels[i][j] = pixel;
      Canva._cores[i][j] = cores;
    }
  }
}
void Canva::DesenharRetangulo(Canva &Canva, int x_superior_esquerdo,
                              int y_superior_esquerdo, int largura, int altura,
                              char pixel, char cores)
{

  
}