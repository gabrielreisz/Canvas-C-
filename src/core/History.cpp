#include "History.hpp"

namespace canvas {

void Historico::registrar(const Canvas& estadoAtual) {
    pilhaDesfazer_.push_back(estadoAtual);
    if (pilhaDesfazer_.size() > limite_)
        pilhaDesfazer_.erase(pilhaDesfazer_.begin());
    pilhaRefazer_.clear();
}

Canvas Historico::desfazer(const Canvas& estadoAtual) {
    Canvas anterior = pilhaDesfazer_.back();
    pilhaDesfazer_.pop_back();
    pilhaRefazer_.push_back(estadoAtual);
    return anterior;
}

Canvas Historico::refazer(const Canvas& estadoAtual) {
    Canvas proximo = pilhaRefazer_.back();
    pilhaRefazer_.pop_back();
    pilhaDesfazer_.push_back(estadoAtual);
    return proximo;
}

void Historico::limpar() {
    pilhaDesfazer_.clear();
    pilhaRefazer_.clear();
}

} // namespace canvas
