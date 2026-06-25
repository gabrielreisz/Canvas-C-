#include "Canvas.hpp"

#include <algorithm>
#include <stdexcept>

namespace canvas {

Canvas::Canvas(int largura, int altura, char preenchimento, Color cor)
    : largura_(std::max(0, largura)), altura_(std::max(0, altura)),
      celulas_(static_cast<std::size_t>(largura_) * altura_,
               Cell{preenchimento, cor}) {}

const Cell& Canvas::em(int x, int y) const {
    if (!dentro(x, y))
        throw std::out_of_range("Canvas::em: coordenada fora dos limites");
    return celulas_[indice(x, y)];
}

Cell& Canvas::em(int x, int y) {
    if (!dentro(x, y))
        throw std::out_of_range("Canvas::em: coordenada fora dos limites");
    return celulas_[indice(x, y)];
}

void Canvas::definir(int x, int y, char ch, Color cor) {
    if (!dentro(x, y))
        return; // clipping automatico
    celulas_[indice(x, y)] = Cell{ch, cor};
}

void Canvas::limpar(char ch, Color cor) {
    std::fill(celulas_.begin(), celulas_.end(), Cell{ch, cor});
}

void Canvas::redimensionar(int novaLargura, int novaAltura, char preenchimento,
                           Color cor) {
    novaLargura = std::max(0, novaLargura);
    novaAltura = std::max(0, novaAltura);

    std::vector<Cell> novo(static_cast<std::size_t>(novaLargura) * novaAltura,
                           Cell{preenchimento, cor});

    // Preserva o conteudo que cabe nas novas dimensoes.
    const int copiarLargura = std::min(largura_, novaLargura);
    const int copiarAltura = std::min(altura_, novaAltura);
    for (int y = 0; y < copiarAltura; ++y)
        for (int x = 0; x < copiarLargura; ++x)
            novo[y * novaLargura + x] = celulas_[indice(x, y)];

    largura_ = novaLargura;
    altura_ = novaAltura;
    celulas_ = std::move(novo);
}

} // namespace canvas
