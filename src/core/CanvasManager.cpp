#include "CanvasManager.hpp"

#include <stdexcept>

namespace canvas {

Documento& GerenciadorCanvas::criar(const std::string& nome, int largura,
                                    int altura) {
    docs_.emplace_back(nome, Canvas(largura, altura));
    return docs_.back();
}

void GerenciadorCanvas::remover(std::size_t indice) {
    if (indice >= docs_.size())
        throw std::out_of_range("GerenciadorCanvas::remover: indice invalido");
    docs_.erase(docs_.begin() + indice);
}

} // namespace canvas
