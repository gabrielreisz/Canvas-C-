#ifndef CANVAS_HPP
#define CANVAS_HPP

struct Canva {
  int largura;
  int altura;
  char **pixels;
  char **cores;
};

void CriarCanva(Canva &Canva, int altura, int largura);
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
#endif
