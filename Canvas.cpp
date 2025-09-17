#include "Canvas.hpp"
Canva::Canva(Canva &Canva, int largura, int altura) {
  if (largura <= 0 || altura <= 0) {
    std::cout << "Error: Altura ou Largura invalida" << std::endl;
  } else {
    _largura = largura;
    _altura = altura;
    _pixels = new _pixels[altura];
  }
}
Canva::~Canva() {
  delete[] _pixels;
  delete[] _cores;
}

Canva::ImprimirCanva(&Canva) {
  for (int i = 0; i < Canva->_altura; i++) {
    for (int j = 0; j < Canva->_largura; j++) {
    }
  }
}
