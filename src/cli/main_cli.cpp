// ============================================================================
//  Canvas Editor — Interface de Terminal (CLI)
//
//  Front-end de texto construido sobre a biblioteca canvas_core. Mantem o
//  espirito do programa original (menus em portugues, cores ANSI), mas agora
//  com formas geometricas, balde de tinta, desfazer/refazer e salvar/abrir.
// ============================================================================

#include "core/Canvas.hpp"
#include "core/CanvasIO.hpp"
#include "core/CanvasManager.hpp"
#include "core/Drawing.hpp"

#include <iostream>
#include <limits>
#include <string>

using namespace canvas;

namespace {

// ---- Utilidades de entrada -------------------------------------------------

void limparBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int lerInt(const std::string& prompt) {
    int v;
    while (true) {
        std::cout << prompt;
        if (std::cin >> v) {
            limparBuffer();
            return v;
        }
        limparBuffer();
        std::cout << "  Entrada invalida, tente novamente.\n";
    }
}

int lerIntFaixa(const std::string& prompt, int min, int max) {
    while (true) {
        int v = lerInt(prompt);
        if (v >= min && v <= max)
            return v;
        std::cout << "  Valor fora da faixa [" << min << ", " << max << "].\n";
    }
}

char lerChar(const std::string& prompt) {
    std::cout << prompt;
    char c;
    std::cin >> c;
    limparBuffer();
    return c;
}

std::string lerLinha(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

bool lerSimNao(const std::string& prompt) {
    char c = lerChar(prompt + " (S/N): ");
    return std::toupper(static_cast<unsigned char>(c)) == 'S';
}

Color lerCor(const std::string& prompt) {
    while (true) {
        char c = lerChar(prompt + " (K,R,G,Y,B,M,C,W): ");
        Color cor;
        if (deCodigo(c, cor))
            return cor;
        std::cout << "  Cor invalida.\n";
    }
}

// ---- Renderizacao ----------------------------------------------------------

void desenharCanvas(const Canvas& c) {
    std::cout << "\n     ";
    for (int x = 0; x < c.largura(); ++x)
        std::cout << x % 10;
    std::cout << '\n';
    for (int y = 0; y < c.altura(); ++y) {
        std::cout.width(3);
        std::cout << y << "  ";
        for (int x = 0; x < c.largura(); ++x) {
            const Cell& cel = c.em(x, y);
            std::cout << codigoAnsi(cel.cor) << cel.ch << "\033[0m";
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

void listar(const GerenciadorCanvas& g) {
    if (g.vazio()) {
        std::cout << "\nNenhum canvas existe ainda.\n";
        return;
    }
    std::cout << "\n=== Canvas existentes ===\n";
    for (std::size_t i = 0; i < g.quantidade(); ++i) {
        const auto& d = g.em(i);
        std::cout << "  " << i + 1 << ". " << d.nome << " ("
                  << d.canvas.largura() << "x" << d.canvas.altura() << ")\n";
    }
}

// Retorna ponteiro para o documento escolhido, ou nullptr se nao houver canvas.
Documento* selecionar(GerenciadorCanvas& g) {
    if (g.vazio()) {
        std::cout << "Nenhum canvas disponivel.\n";
        return nullptr;
    }
    if (g.quantidade() == 1)
        return &g.em(0);
    listar(g);
    int i = lerIntFaixa("Selecione o numero do canvas: ", 1,
                        (int)g.quantidade());
    return &g.em(i - 1);
}

// Registra o estado atual no historico antes de uma modificacao.
void prepararEdicao(Documento& d) { d.historico.registrar(d.canvas); }

// ---- Acoes -----------------------------------------------------------------

void criarCanvas(GerenciadorCanvas& g) {
    std::string nome = lerLinha("Nome do canvas: ");
    if (nome.empty())
        nome = "Canvas " + std::to_string(g.quantidade() + 1);
    int largura = lerIntFaixa("Largura (1-200): ", 1, 200);
    int altura = lerIntFaixa("Altura (1-100): ", 1, 100);

    Documento& d = g.criar(nome, largura, altura);

    if (lerSimNao("Adicionar borda?"))
        desenho::borda(d.canvas, lerCor("Cor da borda"));
    if (lerSimNao("Preencher com um padrao?")) {
        char p = lerChar("Caractere de preenchimento: ");
        Color cor = lerCor("Cor");
        d.canvas.limpar(p, cor);
    }
    std::cout << "Canvas '" << nome << "' criado!\n";
    desenharCanvas(d.canvas);
}

void menuFormas(Documento& d) {
    Canvas& c = d.canvas;
    std::cout << "\n=== Desenhar forma em '" << d.nome << "' ===\n"
              << "  1. Ponto\n  2. Linha\n  3. Retangulo\n"
              << "  4. Retangulo preenchido\n  5. Circulo\n"
              << "  6. Circulo preenchido\n  7. Elipse\n  0. Voltar\n";
    int op = lerInt("Opcao: ");
    if (op == 0)
        return;

    char ch = (op >= 1) ? lerChar("Caractere: ") : '*';
    Color cor = lerCor("Cor");
    prepararEdicao(d);

    switch (op) {
    case 1:
        desenho::ponto(c, lerInt("x: "), lerInt("y: "), ch, cor);
        break;
    case 2:
        desenho::linha(c, lerInt("x inicial: "), lerInt("y inicial: "),
                       lerInt("x final: "), lerInt("y final: "), ch, cor);
        break;
    case 3:
        desenho::retangulo(c, lerInt("x: "), lerInt("y: "),
                           lerInt("largura: "), lerInt("altura: "), ch, cor);
        break;
    case 4:
        desenho::retanguloPreenchido(c, lerInt("x: "), lerInt("y: "),
                                     lerInt("largura: "), lerInt("altura: "),
                                     ch, cor);
        break;
    case 5:
        desenho::circulo(c, lerInt("centro x: "), lerInt("centro y: "),
                         lerInt("raio: "), ch, cor);
        break;
    case 6:
        desenho::circuloPreenchido(c, lerInt("centro x: "), lerInt("centro y: "),
                                   lerInt("raio: "), ch, cor);
        break;
    case 7:
        desenho::elipse(c, lerInt("centro x: "), lerInt("centro y: "),
                        lerInt("raio x: "), lerInt("raio y: "), ch, cor);
        break;
    default:
        std::cout << "Opcao invalida.\n";
        return;
    }
    desenharCanvas(c);
}

void editarPixel(Documento& d) {
    Canvas& c = d.canvas;
    int x = lerIntFaixa("x (0.." + std::to_string(c.largura() - 1) + "): ", 0,
                        c.largura() - 1);
    int y = lerIntFaixa("y (0.." + std::to_string(c.altura() - 1) + "): ", 0,
                        c.altura() - 1);
    char ch = lerChar("Caractere: ");
    Color cor = lerCor("Cor");
    prepararEdicao(d);
    c.definir(x, y, ch, cor);
    std::cout << "Pixel editado!\n";
    desenharCanvas(c);
}

void baldeDeTinta(Documento& d) {
    Canvas& c = d.canvas;
    int x = lerIntFaixa("x: ", 0, c.largura() - 1);
    int y = lerIntFaixa("y: ", 0, c.altura() - 1);
    char ch = lerChar("Novo caractere: ");
    Color cor = lerCor("Nova cor");
    prepararEdicao(d);
    int n = desenho::preencher(c, x, y, ch, cor);
    std::cout << n << " celulas preenchidas.\n";
    desenharCanvas(c);
}

void redimensionar(Documento& d) {
    int w = lerIntFaixa("Nova largura (1-200): ", 1, 200);
    int h = lerIntFaixa("Nova altura (1-100): ", 1, 100);
    prepararEdicao(d);
    d.canvas.redimensionar(w, h);
    std::cout << "Redimensionado para " << w << "x" << h << ".\n";
    desenharCanvas(d.canvas);
}

void desfazerRefazer(Documento& d, bool refazer) {
    if (refazer) {
        if (!d.historico.podeRefazer()) {
            std::cout << "Nada para refazer.\n";
            return;
        }
        d.canvas = d.historico.refazer(d.canvas);
        std::cout << "Refeito.\n";
    } else {
        if (!d.historico.podeDesfazer()) {
            std::cout << "Nada para desfazer.\n";
            return;
        }
        d.canvas = d.historico.desfazer(d.canvas);
        std::cout << "Desfeito.\n";
    }
    desenharCanvas(d.canvas);
}

void salvarAbrir(GerenciadorCanvas& g, bool abrir) {
    if (abrir) {
        std::string caminho = lerLinha("Caminho do arquivo .canvas: ");
        std::string nome = lerLinha("Nome para o canvas: ");
        try {
            Canvas c = io::carregar(caminho);
            Documento& d = g.criar(nome.empty() ? caminho : nome, 1, 1);
            d.canvas = std::move(c);
            std::cout << "Canvas carregado!\n";
            desenharCanvas(d.canvas);
        } catch (const std::exception& e) {
            std::cout << "Erro: " << e.what() << "\n";
        }
        return;
    }
    Documento* d = selecionar(g);
    if (!d)
        return;
    std::string caminho = lerLinha("Salvar como (.canvas): ");
    try {
        io::salvar(d->canvas, caminho);
        std::cout << "Salvo em " << caminho << "\n";
    } catch (const std::exception& e) {
        std::cout << "Erro: " << e.what() << "\n";
    }
}

void exportar(GerenciadorCanvas& g) {
    Documento* d = selecionar(g);
    if (!d)
        return;
    std::cout << "\n  1. Imagem PPM (.ppm)\n  2. Texto puro (.txt)\n";
    int op = lerInt("Formato: ");
    std::string caminho = lerLinha("Caminho de saida: ");
    try {
        if (op == 1) {
            int escala = lerIntFaixa("Escala (pixels por celula, 1-64): ", 1, 64);
            io::exportarPPM(d->canvas, caminho, escala);
        } else {
            io::exportarTxt(d->canvas, caminho);
        }
        std::cout << "Exportado para " << caminho << "\n";
    } catch (const std::exception& e) {
        std::cout << "Erro: " << e.what() << "\n";
    }
}

void menuPrincipal() {
    std::cout << "\n============ Canvas Editor (CLI) ============\n"
              << "  1. Criar canvas\n"
              << "  2. Listar canvas\n"
              << "  3. Visualizar canvas\n"
              << "  4. Editar pixel\n"
              << "  5. Desenhar forma\n"
              << "  6. Balde de tinta (flood fill)\n"
              << "  7. Borda\n"
              << "  8. Redimensionar\n"
              << "  9. Desfazer / 10. Refazer\n"
              << "  11. Salvar / 12. Abrir\n"
              << "  13. Exportar (imagem/texto)\n"
              << "  14. Remover canvas\n"
              << "  0. Sair\n"
              << "Escolha: ";
}

} // namespace

int main() {
    GerenciadorCanvas g;
    int opcao;

    do {
        menuPrincipal();
        if (!(std::cin >> opcao)) {
            limparBuffer();
            std::cout << "Opcao invalida.\n";
            continue;
        }
        limparBuffer();

        try {
            switch (opcao) {
            case 1:
                criarCanvas(g);
                break;
            case 2:
                listar(g);
                break;
            case 3:
                if (Documento* d = selecionar(g))
                    desenharCanvas(d->canvas);
                break;
            case 4:
                if (Documento* d = selecionar(g))
                    editarPixel(*d);
                break;
            case 5:
                if (Documento* d = selecionar(g))
                    menuFormas(*d);
                break;
            case 6:
                if (Documento* d = selecionar(g))
                    baldeDeTinta(*d);
                break;
            case 7:
                if (Documento* d = selecionar(g)) {
                    prepararEdicao(*d);
                    desenho::borda(d->canvas, lerCor("Cor da borda"));
                    desenharCanvas(d->canvas);
                }
                break;
            case 8:
                if (Documento* d = selecionar(g))
                    redimensionar(*d);
                break;
            case 9:
                if (Documento* d = selecionar(g))
                    desfazerRefazer(*d, false);
                break;
            case 10:
                if (Documento* d = selecionar(g))
                    desfazerRefazer(*d, true);
                break;
            case 11:
                salvarAbrir(g, false);
                break;
            case 12:
                salvarAbrir(g, true);
                break;
            case 13:
                exportar(g);
                break;
            case 14:
                if (!g.vazio()) {
                    listar(g);
                    int i = lerIntFaixa("Numero do canvas para remover: ", 1,
                                        (int)g.quantidade());
                    g.remover(i - 1);
                    std::cout << "Removido.\n";
                }
                break;
            case 0:
                std::cout << "Saindo...\n";
                break;
            default:
                std::cout << "Opcao invalida.\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Erro: " << e.what() << "\n";
        }
    } while (opcao != 0);

    return 0;
}
