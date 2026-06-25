#ifndef CANVAS_CORE_CANVASIO_HPP
#define CANVAS_CORE_CANVASIO_HPP

#include "Canvas.hpp"
#include <string>

// Persistencia e exportacao de canvas.
namespace canvas::io {

// Formato de texto proprio (.canvas): guarda caractere e cor de cada celula.
// Lanca std::runtime_error em caso de erro de arquivo/formato.
void salvar(const Canvas& c, const std::string& caminho);
Canvas carregar(const std::string& caminho);

// Exporta como imagem PPM (P6 binario): cada celula vira um bloco de
// 'escala'x'escala' pixels com a cor RGB correspondente. Celulas em branco
// usam a cor de fundo. Util para abrir o desenho em qualquer visualizador.
void exportarPPM(const Canvas& c, const std::string& caminho, int escala = 16);

// Exporta a arte como texto puro (apenas os caracteres, sem cor) — bom para
// colar em qualquer lugar.
void exportarTxt(const Canvas& c, const std::string& caminho);

} // namespace canvas::io

#endif // CANVAS_CORE_CANVASIO_HPP
