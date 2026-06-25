#ifndef CANVAS_CORE_COLOR_HPP
#define CANVAS_CORE_COLOR_HPP

#include <array>
#include <cstdint>
#include <string>

namespace canvas {

// Paleta de 8 cores nomeadas. Cada cor tem um codigo de 1 letra
// (compatibilidade com o formato antigo), um codigo de escape ANSI
// para o terminal e um valor RGB para a interface grafica/exportacao.
enum class Color : std::uint8_t {
    Black,   // K
    Red,     // R
    Green,   // G
    Yellow,  // Y
    Blue,    // B
    Magenta, // M
    Cyan,    // C
    White    // W
};

struct RGB {
    std::uint8_t r = 0, g = 0, b = 0;
};

// Lista canonica de todas as cores, util para iterar (paletas, menus...).
inline const std::array<Color, 8>& todasAsCores() {
    static const std::array<Color, 8> cores = {
        Color::Black, Color::Red,     Color::Green, Color::Yellow,
        Color::Blue,  Color::Magenta, Color::Cyan,  Color::White};
    return cores;
}

inline char paraCodigo(Color c) {
    switch (c) {
    case Color::Black:   return 'K';
    case Color::Red:     return 'R';
    case Color::Green:   return 'G';
    case Color::Yellow:  return 'Y';
    case Color::Blue:    return 'B';
    case Color::Magenta: return 'M';
    case Color::Cyan:    return 'C';
    case Color::White:   return 'W';
    }
    return 'W';
}

// Converte um codigo de letra em cor. Aceita maiusculas/minusculas.
// Retorna false se o codigo for invalido (out fica inalterado).
inline bool deCodigo(char codigo, Color& out) {
    switch (codigo) {
    case 'K': case 'k': out = Color::Black;   return true;
    case 'R': case 'r': out = Color::Red;     return true;
    case 'G': case 'g': out = Color::Green;   return true;
    case 'Y': case 'y': out = Color::Yellow;  return true;
    case 'B': case 'b': out = Color::Blue;    return true;
    case 'M': case 'm': out = Color::Magenta; return true;
    case 'C': case 'c': out = Color::Cyan;    return true;
    case 'W': case 'w': out = Color::White;   return true;
    default:            return false;
    }
}

inline const char* codigoAnsi(Color c) {
    switch (c) {
    case Color::Black:   return "\033[30m";
    case Color::Red:     return "\033[31m";
    case Color::Green:   return "\033[32m";
    case Color::Yellow:  return "\033[33m";
    case Color::Blue:    return "\033[34m";
    case Color::Magenta: return "\033[35m";
    case Color::Cyan:    return "\033[36m";
    case Color::White:   return "\033[37m";
    }
    return "\033[0m";
}

inline const char* nomeCor(Color c) {
    switch (c) {
    case Color::Black:   return "Preto";
    case Color::Red:     return "Vermelho";
    case Color::Green:   return "Verde";
    case Color::Yellow:  return "Amarelo";
    case Color::Blue:    return "Azul";
    case Color::Magenta: return "Magenta";
    case Color::Cyan:    return "Ciano";
    case Color::White:   return "Branco";
    }
    return "Branco";
}

inline RGB paraRGB(Color c) {
    switch (c) {
    case Color::Black:   return {30, 30, 30};
    case Color::Red:     return {222, 56, 43};
    case Color::Green:   return {57, 181, 74};
    case Color::Yellow:  return {255, 199, 6};
    case Color::Blue:    return {0, 111, 184};
    case Color::Magenta: return {180, 60, 170};
    case Color::Cyan:    return {44, 181, 233};
    case Color::White:   return {220, 220, 220};
    }
    return {220, 220, 220};
}

} // namespace canvas

#endif // CANVAS_CORE_COLOR_HPP
