#ifndef CANVAS_CORE_HISTORY_HPP
#define CANVAS_CORE_HISTORY_HPP

#include "Canvas.hpp"
#include <cstddef>
#include <vector>

namespace canvas {

// Historico de desfazer/refazer baseado em snapshots.
//
// Estrategia simples e robusta: antes de cada modificacao, chama-se
// registrar(estadoAtual). desfazer()/refazer() trocam entre as pilhas.
// O numero de snapshots e limitado para nao crescer indefinidamente.
class Historico {
public:
    explicit Historico(std::size_t limite = 50) : limite_(limite) {}

    // Registra o estado anterior a uma modificacao. Limpa a pilha de refazer.
    void registrar(const Canvas& estadoAtual);

    bool podeDesfazer() const { return !pilhaDesfazer_.empty(); }
    bool podeRefazer() const { return !pilhaRefazer_.empty(); }

    // Recebem o estado atual (para guardar no caminho oposto) e retornam
    // o estado restaurado. Chame apenas se podeDesfazer/podeRefazer for true.
    Canvas desfazer(const Canvas& estadoAtual);
    Canvas refazer(const Canvas& estadoAtual);

    void limpar();

private:
    std::vector<Canvas> pilhaDesfazer_;
    std::vector<Canvas> pilhaRefazer_;
    std::size_t limite_;
};

} // namespace canvas

#endif // CANVAS_CORE_HISTORY_HPP
