#include "CanvasIO.hpp"

#include <fstream>
#include <stdexcept>

namespace canvas::io {

// Formato .canvas (texto):
//   CANVASv1
//   <largura> <altura>
//   <largura caracteres>   x altura linhas (a arte)
//   ---
//   <largura codigos de cor>  x altura linhas
//
// Os caracteres sao gravados como bytes literais (preservando espacos);
// por isso lemos por contagem de colunas, nao por palavras.

void salvar(const Canvas& c, const std::string& caminho) {
    std::ofstream out(caminho, std::ios::binary);
    if (!out)
        throw std::runtime_error("Nao foi possivel abrir o arquivo para escrita: " +
                                 caminho);

    out << "CANVASv1\n" << c.largura() << ' ' << c.altura() << '\n';
    for (int y = 0; y < c.altura(); ++y) {
        for (int x = 0; x < c.largura(); ++x)
            out << c.em(x, y).ch;
        out << '\n';
    }
    out << "---\n";
    for (int y = 0; y < c.altura(); ++y) {
        for (int x = 0; x < c.largura(); ++x)
            out << paraCodigo(c.em(x, y).cor);
        out << '\n';
    }
    if (!out)
        throw std::runtime_error("Erro ao gravar o arquivo: " + caminho);
}

Canvas carregar(const std::string& caminho) {
    std::ifstream in(caminho, std::ios::binary);
    if (!in)
        throw std::runtime_error("Nao foi possivel abrir o arquivo: " + caminho);

    std::string cabecalho;
    std::getline(in, cabecalho);
    if (cabecalho != "CANVASv1")
        throw std::runtime_error("Formato de arquivo invalido (esperado CANVASv1)");

    int largura = 0, altura = 0;
    in >> largura >> altura;
    in.ignore(); // pula o '\n' apos as dimensoes
    if (largura <= 0 || altura <= 0)
        throw std::runtime_error("Dimensoes invalidas no arquivo");

    Canvas c(largura, altura);

    auto lerLinha = [&](std::string& linha) {
        if (!std::getline(in, linha))
            throw std::runtime_error("Arquivo truncado/corrompido");
    };

    std::string linha;
    for (int y = 0; y < altura; ++y) {
        lerLinha(linha);
        for (int x = 0; x < largura; ++x) {
            char ch = (x < (int)linha.size()) ? linha[x] : ' ';
            c.em(x, y).ch = ch;
        }
    }
    lerLinha(linha); // separador "---"
    for (int y = 0; y < altura; ++y) {
        lerLinha(linha);
        for (int x = 0; x < largura; ++x) {
            Color cor = Color::White;
            if (x < (int)linha.size())
                deCodigo(linha[x], cor);
            c.em(x, y).cor = cor;
        }
    }
    return c;
}

void exportarPPM(const Canvas& c, const std::string& caminho, int escala) {
    if (escala < 1)
        escala = 1;
    std::ofstream out(caminho, std::ios::binary);
    if (!out)
        throw std::runtime_error("Nao foi possivel abrir o arquivo: " + caminho);

    const int W = c.largura() * escala;
    const int H = c.altura() * escala;
    const RGB fundo{20, 20, 28};

    out << "P6\n" << W << ' ' << H << "\n255\n";
    for (int y = 0; y < c.altura(); ++y) {
        for (int linhaPix = 0; linhaPix < escala; ++linhaPix) {
            for (int x = 0; x < c.largura(); ++x) {
                const Cell& cel = c.em(x, y);
                RGB cor = (cel.ch == ' ') ? fundo : paraRGB(cel.cor);
                for (int colPix = 0; colPix < escala; ++colPix) {
                    out.put(static_cast<char>(cor.r));
                    out.put(static_cast<char>(cor.g));
                    out.put(static_cast<char>(cor.b));
                }
            }
        }
    }
    if (!out)
        throw std::runtime_error("Erro ao gravar a imagem: " + caminho);
}

void exportarTxt(const Canvas& c, const std::string& caminho) {
    std::ofstream out(caminho, std::ios::binary);
    if (!out)
        throw std::runtime_error("Nao foi possivel abrir o arquivo: " + caminho);
    for (int y = 0; y < c.altura(); ++y) {
        for (int x = 0; x < c.largura(); ++x)
            out << c.em(x, y).ch;
        out << '\n';
    }
}

} // namespace canvas::io
