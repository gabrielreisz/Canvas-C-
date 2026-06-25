#include "Drawing.hpp"

#include <cstdlib>
#include <queue>

namespace canvas::desenho {

void ponto(Canvas& c, int x, int y, char ch, Color cor) {
    c.definir(x, y, ch, cor);
}

void linha(Canvas& c, int x1, int y1, int x2, int y2, char ch, Color cor) {
    // Algoritmo de Bresenham generalizado (funciona em qualquer direcao).
    int dx = std::abs(x2 - x1);
    int dy = -std::abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int erro = dx + dy;

    while (true) {
        c.definir(x1, y1, ch, cor);
        if (x1 == x2 && y1 == y2)
            break;
        int e2 = 2 * erro;
        if (e2 >= dy) {
            erro += dy;
            x1 += sx;
        }
        if (e2 <= dx) {
            erro += dx;
            y1 += sy;
        }
    }
}

void retangulo(Canvas& c, int x, int y, int largura, int altura, char ch,
               Color cor) {
    if (largura <= 0 || altura <= 0)
        return;
    int x2 = x + largura - 1;
    int y2 = y + altura - 1;
    linha(c, x, y, x2, y, ch, cor);   // topo
    linha(c, x, y2, x2, y2, ch, cor); // base
    linha(c, x, y, x, y2, ch, cor);   // esquerda
    linha(c, x2, y, x2, y2, ch, cor); // direita
}

void retanguloPreenchido(Canvas& c, int x, int y, int largura, int altura,
                         char ch, Color cor) {
    for (int j = 0; j < altura; ++j)
        for (int i = 0; i < largura; ++i)
            c.definir(x + i, y + j, ch, cor);
}

void circulo(Canvas& c, int cx, int cy, int raio, char ch, Color cor) {
    if (raio < 0)
        return;
    int x = raio;
    int y = 0;
    int erro = 1 - raio;
    while (x >= y) {
        // Os 8 octantes simetricos.
        c.definir(cx + x, cy + y, ch, cor);
        c.definir(cx + y, cy + x, ch, cor);
        c.definir(cx - y, cy + x, ch, cor);
        c.definir(cx - x, cy + y, ch, cor);
        c.definir(cx - x, cy - y, ch, cor);
        c.definir(cx - y, cy - x, ch, cor);
        c.definir(cx + y, cy - x, ch, cor);
        c.definir(cx + x, cy - y, ch, cor);
        ++y;
        if (erro < 0) {
            erro += 2 * y + 1;
        } else {
            --x;
            erro += 2 * (y - x) + 1;
        }
    }
}

void circuloPreenchido(Canvas& c, int cx, int cy, int raio, char ch, Color cor) {
    if (raio < 0)
        return;
    for (int y = -raio; y <= raio; ++y)
        for (int x = -raio; x <= raio; ++x)
            if (x * x + y * y <= raio * raio)
                c.definir(cx + x, cy + y, ch, cor);
}

void elipse(Canvas& c, int cx, int cy, int rx, int ry, char ch, Color cor) {
    if (rx <= 0 || ry <= 0)
        return;
    long rx2 = (long)rx * rx;
    long ry2 = (long)ry * ry;
    long x = 0, y = ry;
    long px = 0, py = 2 * rx2 * y;

    auto plot4 = [&](long px_, long py_) {
        c.definir(cx + (int)px_, cy + (int)py_, ch, cor);
        c.definir(cx - (int)px_, cy + (int)py_, ch, cor);
        c.definir(cx + (int)px_, cy - (int)py_, ch, cor);
        c.definir(cx - (int)px_, cy - (int)py_, ch, cor);
    };

    // Regiao 1
    long p1 = ry2 - rx2 * ry + rx2 / 4;
    while (px < py) {
        plot4(x, y);
        ++x;
        px += 2 * ry2;
        if (p1 < 0) {
            p1 += ry2 + px;
        } else {
            --y;
            py -= 2 * rx2;
            p1 += ry2 + px - py;
        }
    }
    // Regiao 2
    long p2 = ry2 * (x * x + x) + rx2 * (y - 1) * (y - 1) - rx2 * ry2;
    // arredondamento: usa (x+0.5)^2 -> ry2*(4x^2+4x+1)/4
    p2 = (long)(ry2 * (x + 0.5) * (x + 0.5) + rx2 * (y - 1) * (y - 1) -
                rx2 * ry2);
    while (y >= 0) {
        plot4(x, y);
        --y;
        py -= 2 * rx2;
        if (p2 > 0) {
            p2 += rx2 - py;
        } else {
            ++x;
            px += 2 * ry2;
            p2 += rx2 - py + px;
        }
    }
}

void borda(Canvas& c, Color cor) {
    int w = c.largura(), h = c.altura();
    if (w == 0 || h == 0)
        return;
    for (int j = 0; j < w; ++j) {
        c.definir(j, 0, '-', cor);
        c.definir(j, h - 1, '-', cor);
    }
    for (int i = 0; i < h; ++i) {
        c.definir(0, i, '|', cor);
        c.definir(w - 1, i, '|', cor);
    }
    c.definir(0, 0, '+', cor);
    c.definir(w - 1, 0, '+', cor);
    c.definir(0, h - 1, '+', cor);
    c.definir(w - 1, h - 1, '+', cor);
}

int preencher(Canvas& c, int x, int y, char ch, Color cor) {
    if (!c.dentro(x, y))
        return 0;

    const Cell alvo = c.em(x, y);
    const Cell nova{ch, cor};
    if (alvo == nova)
        return 0; // nada a fazer

    int pintadas = 0;
    std::queue<std::pair<int, int>> fila;
    fila.push({x, y});
    c.em(x, y) = nova;
    ++pintadas;

    const int dx[] = {1, -1, 0, 0};
    const int dy[] = {0, 0, 1, -1};
    while (!fila.empty()) {
        auto [cx, cy] = fila.front();
        fila.pop();
        for (int k = 0; k < 4; ++k) {
            int nx = cx + dx[k], ny = cy + dy[k];
            if (c.dentro(nx, ny) && c.em(nx, ny) == alvo) {
                c.em(nx, ny) = nova;
                ++pintadas;
                fila.push({nx, ny});
            }
        }
    }
    return pintadas;
}

void sobrepor(Canvas& destino, const Canvas& fonte, int offX, int offY) {
    for (int y = 0; y < fonte.altura(); ++y) {
        for (int x = 0; x < fonte.largura(); ++x) {
            const Cell& cel = fonte.em(x, y);
            if (cel.ch != ' ') // branco = transparente
                destino.definir(offX + x, offY + y, cel.ch, cel.cor);
        }
    }
}

double similaridade(const Canvas& a, const Canvas& b) {
    if (a.largura() != b.largura() || a.altura() != b.altura() || a.vazio())
        return 0.0;
    std::size_t iguais = 0;
    const auto& da = a.dados();
    const auto& db = b.dados();
    for (std::size_t i = 0; i < da.size(); ++i)
        if (da[i] == db[i])
            ++iguais;
    return static_cast<double>(iguais) / da.size();
}

} // namespace canvas::desenho
