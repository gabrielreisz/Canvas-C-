#ifndef CANVAS_HPP
#define CANVAS_HPP
#include <iostream>

class Canva {
private:
  int _largura;
  int _altura;
  char **_pixels;
  char **_cores;

public:
  Canva(int largura, int altura, char pixel, char cores);
  ~Canva();
  void CriarCanva(Canva &Canva);
  void DestruirCanvas(Canva &Canva);
  void ImprimirCanva(Canva &Canva);
  void DesenharPonto(Canva &Canva, int x, int y, char pixel, char cores);
  void DesenharLinha(Canva &Canva, int x1, int y1, int x2, int y2, char pixel,
                     char cores);
  void DesenharRetangulo(Canva &Canva, int x_superior_esquerdo,
                         int y_superior_esquerdo, int largura, int altura,
                         char pixel, char cores);
  void DesenharRetanguloPreenchido(Canva &Canva, int x_superior_esquerdo,
                                   int y_superior_esquerdo, int largura,
                                   int altura, char pixel, char cores);
  void RedimensionarCanva(Canva &Canva, int novaLargura, int novaAltura);
  void SobreporCanvas(Canva &Canva_destino, Canva Origem1, Canva Origem2);
  void CompararCanvas(Canva &Canva_comparar, Canva Conva_comparador);
};

#endif
