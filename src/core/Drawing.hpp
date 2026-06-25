#ifndef CANVAS_CORE_DRAWING_HPP
#define CANVAS_CORE_DRAWING_HPP

#include "Canvas.hpp"

// Algoritmos de desenho que operam sobre um Canvas.
//
// Sao funcoes livres (nao metodos) para manter o modelo Canvas enxuto e
// permitir adicionar novas formas sem tocar na classe — separacao clara
// entre "dados" (Canvas) e "operacoes" (este modulo).
namespace canvas::desenho {

void ponto(Canvas& c, int x, int y, char ch, Color cor);

// Linha pelo algoritmo de Bresenham: apenas aritmetica inteira, O(n) no
// numero de pixels da linha, sem ponto flutuante nem divisoes por celula.
void linha(Canvas& c, int x1, int y1, int x2, int y2, char ch, Color cor);

void retangulo(Canvas& c, int x, int y, int largura, int altura, char ch,
               Color cor);
void retanguloPreenchido(Canvas& c, int x, int y, int largura, int altura,
                         char ch, Color cor);

// Circulo pelo algoritmo midpoint (aritmetica inteira, simetria de 8 octantes).
void circulo(Canvas& c, int cx, int cy, int raio, char ch, Color cor);
void circuloPreenchido(Canvas& c, int cx, int cy, int raio, char ch, Color cor);

// Elipse pelo algoritmo midpoint (duas regioes, simetria de 4 quadrantes).
void elipse(Canvas& c, int cx, int cy, int rx, int ry, char ch, Color cor);

// Borda retangular com cantos "+", laterais "|" e topo/base "-".
void borda(Canvas& c, Color cor);

// Preenchimento por inundacao (balde de tinta). Substitui a regiao conexa
// de 4 vizinhos que compartilha a mesma celula da posicao inicial.
// Implementado com BFS iterativa (sem risco de estouro de pilha): O(largura*altura).
// Retorna quantas celulas foram preenchidas.
int preencher(Canvas& c, int x, int y, char ch, Color cor);

// Sobrepoe 'fonte' sobre 'destino' a partir de (offX, offY). Celulas em
// branco (' ') da fonte sao tratadas como transparentes.
void sobrepor(Canvas& destino, const Canvas& fonte, int offX = 0, int offY = 0);

// Similaridade entre dois canvas: fracao de celulas iguais (0.0 a 1.0).
// Canvas de tamanhos diferentes retornam 0.0.
double similaridade(const Canvas& a, const Canvas& b);

} // namespace canvas::desenho

#endif // CANVAS_CORE_DRAWING_HPP
