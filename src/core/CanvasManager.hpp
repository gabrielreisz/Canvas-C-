#ifndef CANVAS_CORE_CANVASMANAGER_HPP
#define CANVAS_CORE_CANVASMANAGER_HPP

#include "Canvas.hpp"
#include "History.hpp"
#include <string>
#include <vector>

namespace canvas {

// Um documento: canvas + nome + seu proprio historico de undo/redo.
struct Documento {
    std::string nome;
    Canvas canvas;
    Historico historico;

    Documento(std::string n, Canvas c)
        : nome(std::move(n)), canvas(std::move(c)) {}
};

// Gerencia a colecao de documentos abertos. Substitui o vetor solto de
// CanvasInfo da versao original e centraliza a logica de selecao/remocao.
class GerenciadorCanvas {
public:
    Documento& criar(const std::string& nome, int largura, int altura);
    void remover(std::size_t indice);

    std::size_t quantidade() const { return docs_.size(); }
    bool vazio() const { return docs_.empty(); }

    Documento& em(std::size_t i) { return docs_.at(i); }
    const Documento& em(std::size_t i) const { return docs_.at(i); }

    std::vector<Documento>& todos() { return docs_; }
    const std::vector<Documento>& todos() const { return docs_; }

private:
    std::vector<Documento> docs_;
};

} // namespace canvas

#endif // CANVAS_CORE_CANVASMANAGER_HPP
