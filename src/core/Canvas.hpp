#ifndef CANVAS_CORE_CANVAS_HPP
#define CANVAS_CORE_CANVAS_HPP

#include "Color.hpp"
#include <vector>

namespace canvas {

// Uma celula do canvas: caractere visivel + sua cor.
struct Cell {
    char ch = ' ';
    Color cor = Color::White;

    bool operator==(const Cell& o) const { return ch == o.ch && cor == o.cor; }
    bool operator!=(const Cell& o) const { return !(*this == o); }
};

// Modelo de dados do canvas.
//
// Usa um unico std::vector em ordem row-major (indice = y * largura + x),
// o que torna a classe copiavel/movivel com seguranca pela "Regra do Zero":
// nao ha gerenciamento manual de memoria, eliminando os vazamentos e
// double-free da versao original baseada em char**.
class Canvas {
public:
    Canvas() = default;
    Canvas(int largura, int altura, char preenchimento = ' ',
           Color cor = Color::White);

    int largura() const { return largura_; }
    int altura() const { return altura_; }
    bool vazio() const { return largura_ == 0 || altura_ == 0; }

    bool dentro(int x, int y) const {
        return x >= 0 && y >= 0 && x < largura_ && y < altura_;
    }

    // Acesso a celula. Lanca std::out_of_range se (x, y) estiver fora.
    const Cell& em(int x, int y) const;
    Cell& em(int x, int y);

    // Define uma celula. Ignora silenciosamente coordenadas fora do canvas,
    // o que simplifica os algoritmos de desenho (clipping automatico).
    void definir(int x, int y, char ch, Color cor);

    void limpar(char ch = ' ', Color cor = Color::White);
    void redimensionar(int novaLargura, int novaAltura, char preenchimento = ' ',
                       Color cor = Color::White);

    // Acesso direto ao buffer (row-major) para renderizadores.
    const std::vector<Cell>& dados() const { return celulas_; }

private:
    int indice(int x, int y) const { return y * largura_ + x; }

    int largura_ = 0;
    int altura_ = 0;
    std::vector<Cell> celulas_;
};

} // namespace canvas

#endif // CANVAS_CORE_CANVAS_HPP
