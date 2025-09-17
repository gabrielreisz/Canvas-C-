#include "Canvas.hpp"

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
    _pixels = new char*[_altura];
    _cores = new char*[_altura];
    for(int i = 0; i<_altura; i++){
      _pixels[i] = new char[_largura];
      _cores[i] = new char[_largura];
      for(int j = 0; j<_largura; j++){
        _pixels[i][j] =' ';
        _cores[i][j] ='W';
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
