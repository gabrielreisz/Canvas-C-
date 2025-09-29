#include "Canvas.hpp"
#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <stdexcept>

struct CanvasInfo {
    Canva canvas;
    std::string nome;
    bool ativo;
    
    CanvasInfo(const std::string& n) : nome(n), ativo(true) {}
};

void limparBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void desenharBorda(Canva &canva, char cor) {
    int altura = canva.getaltura();
    int largura = canva.getlargura();
    char** pixels = canva.getpixels();
    char** cores = canva.getcores();
    
    // Bordas horizontais
    for (int j = 0; j < largura; j++) {
        pixels[0][j] = '-';
        pixels[altura-1][j] = '-';
        cores[0][j] = cor;
        cores[altura-1][j] = cor;
    }
    
    // Bordas verticais
    for (int i = 1; i < altura-1; i++) {
        pixels[i][0] = '|';
        pixels[i][largura-1] = '|';
        cores[i][0] = cor;
        cores[i][largura-1] = cor;
    }
    
    // Cantos
    pixels[0][0] = '+';
    pixels[0][largura-1] = '+';
    pixels[altura-1][0] = '+';
    pixels[altura-1][largura-1] = '+';
    cores[0][0] = cor;
    cores[0][largura-1] = cor;
    cores[altura-1][0] = cor;
    cores[altura-1][largura-1] = cor;
}

void preencherPadrao(Canva &canva, char pixel, char cor) {
    int altura = canva.getaltura();
    int largura = canva.getlargura();
    char** pixels = canva.getpixels();
    char** cores = canva.getcores();
    
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            pixels[i][j] = pixel;
            cores[i][j] = cor;
        }
    }
}

void exibirCanvas(const std::vector<CanvasInfo>& canvases) {
    if (canvases.empty()) {
        std::cout << "Nenhum canvas existe!\n";
        return;
    }
    
    std::cout << "\nLista de Canvases:\n";
    for (size_t i = 0; i < canvases.size(); i++) {
        std::cout << i + 1 << ". " << canvases[i].nome 
                 << " (" << canvases[i].canvas.getlargura() << "x" << canvases[i].canvas.getaltura() << ")"
                 << (canvases[i].ativo ? "" : " [DELETADO]") << "\n";
    }
}

void exibirCanvas(Canva &canva) {
    std::cout << "\nCanvas atual:\n";
    for (int i = 0; i < canva.getaltura(); i++) {
        for (int j = 0; j < canva.getlargura(); j++) {
            std::cout << canva.GetColorCode(canva.getcores()[i][j]) 
                     << canva.getpixels()[i][j] << "\033[0m";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool validarCor(char cor) {
    return cor == 'K' || cor == 'R' || cor == 'G' || cor == 'Y' || 
           cor == 'B' || cor == 'M' || cor == 'C' || cor == 'W';
}

void editarPixel(Canva &canva) {
    try {
        int x, y;
        char pixel, cor;
        
        std::cout << "\nPosição atual do canvas: " << canva.getlargura() << "x" << canva.getaltura() << std::endl;
        
        do {
            std::cout << "Digite a posição x (0 até " << canva.getlargura()-1 << "): ";
            if (!(std::cin >> x)) {
                throw std::invalid_argument("Entrada inválida para posição x");
            }
            limparBuffer();
        } while (x < 0 || x >= canva.getlargura());
        
        do {
            std::cout << "Digite a posição y (0 até " << canva.getaltura()-1 << "): ";
            if (!(std::cin >> y)) {
                throw std::invalid_argument("Entrada inválida para posição y");
            }
            limparBuffer();
        } while (y < 0 || y >= canva.getaltura());
        
        std::cout << "Digite o caractere: ";
        if (!(std::cin >> pixel)) {
            throw std::invalid_argument("Entrada inválida para caractere");
        }
        limparBuffer();
        
        do {
            std::cout << "Digite a cor (K,R,G,Y,B,M,C,W): ";
            if (!(std::cin >> cor)) {
                throw std::invalid_argument("Entrada inválida para cor");
            }
            cor = std::toupper(cor);
            limparBuffer();
        } while (!validarCor(cor));
        
        char** pixels = canva.getpixels();
        char** cores = canva.getcores();
        
        if (!pixels || !cores) {
            throw std::runtime_error("Canvas não inicializado corretamente");
        }
        
        pixels[y][x] = pixel;
        cores[y][x] = cor;
        
        std::cout << "Pixel editado com sucesso!\n";
    } catch (const std::exception& e) {
        std::cerr << "Erro ao editar pixel: " << e.what() << std::endl;
        limparBuffer();
    }
}

void mostrarMenu() {
    std::cout << "\n=== Menu Principal do Canvas ===\n"
              << "1. Gerenciar Canvas\n"
              << "2. Editar Canvas\n"
              << "3. Visualizar Canvas\n"
              << "0. Sair\n"
              << "Escolha uma opção: ";
}

void mostrarSubMenuGerenciar() {
    std::cout << "\n=== Gerenciar Canvas ===\n"
              << "1. Criar novo Canvas\n"
              << "2. Deletar Canvas\n"
              << "3. Listar todos os Canvas\n"
              << "4. Voltar\n"
              << "Escolha uma opção: ";
}

void mostrarSubMenuEditar() {
    std::cout << "\n=== Editar Canvas ===\n"
              << "1. Editar pixel\n"
              << "2. Adicionar borda\n"
              << "3. Preencher com padrão\n"
              << "4. Voltar\n"
              << "Escolha uma opção: ";
}

void mostrarSubMenuVisualizar() {
    std::cout << "\n=== Visualizar Canvas ===\n"
              << "1. Visualizar Canvas específico\n"
              << "2. Visualizar todos os Canvas\n"
              << "3. Voltar\n"
              << "Escolha uma opção: ";
}

size_t selecionarCanvas(const std::vector<CanvasInfo>& canvases, bool apenasAtivos = true) {
    exibirCanvas(canvases);
    
    if (canvases.empty()) {
        throw std::runtime_error("Nenhum canvas disponível");
    }
    
    size_t indice;
    do {
        std::cout << "Selecione o número do canvas: ";
        if (!(std::cin >> indice) || indice < 1 || indice > canvases.size()) {
            limparBuffer();
            std::cout << "Índice inválido. Tente novamente.\n";
            continue;
        }
        indice--;
        
        if (apenasAtivos && !canvases[indice].ativo) {
            std::cout << "Este canvas foi deletado. Escolha outro.\n";
            continue;
        }
        break;
    } while (true);
    
    return indice;
}

int main() {
    std::vector<CanvasInfo> canvases;
    int opcao, subOpcao;
    
    do {
        try {
            mostrarMenu();
            if (!(std::cin >> opcao)) {
                throw std::invalid_argument("Opção inválida");
            }
            limparBuffer();
        
            switch (opcao) {
                case 1: { // Gerenciar Canvas
                    do {
                        mostrarSubMenuGerenciar();
                        if (!(std::cin >> subOpcao)) {
                            throw std::invalid_argument("Opção inválida");
                        }
                        limparBuffer();
                        
                        switch (subOpcao) {
                            case 1: { // Criar novo Canvas
                                std::string nome;
                                int altura, largura;
                                char resposta;
                                
                                std::cout << "Digite um nome para o canvas: ";
                                std::getline(std::cin, nome);
                                
                                do {
                                    std::cout << "Digite a altura (maior que 0): ";
                                    if (!(std::cin >> altura) || altura <= 0) {
                                        throw std::invalid_argument("Altura inválida");
                                    }
                                    limparBuffer();
                                } while (altura <= 0);
                                
                                do {
                                    std::cout << "Digite a largura (maior que 0): ";
                                    if (!(std::cin >> largura) || largura <= 0) {
                                        throw std::invalid_argument("Largura inválida");
                                    }
                                    limparBuffer();
                                } while (largura <= 0);
                                
                                canvases.push_back(CanvasInfo(nome));
                                CriarCanva(canvases.back().canvas, altura, largura);
                                
                                std::cout << "Deseja adicionar uma borda? (S/N): ";
                                std::cin >> resposta;
                                limparBuffer();
                                
                                if (std::toupper(resposta) == 'S') {
                                    char cor;
                                    do {
                                        std::cout << "Digite a cor da borda (K,R,G,Y,B,M,C,W): ";
                                        std::cin >> cor;
                                        cor = std::toupper(cor);
                                        limparBuffer();
                                    } while (!validarCor(cor));
                                    
                                    desenharBorda(canvases.back().canvas, cor);
                                }
                                
                                std::cout << "Deseja preencher com um padrão? (S/N): ";
                                std::cin >> resposta;
                                limparBuffer();
                                
                                if (std::toupper(resposta) == 'S') {
                                    char pixel, cor;
                                    std::cout << "Digite o caractere para preenchimento: ";
                                    std::cin >> pixel;
                                    limparBuffer();
                                    
                                    do {
                                        std::cout << "Digite a cor (K,R,G,Y,B,M,C,W): ";
                                        std::cin >> cor;
                                        cor = std::toupper(cor);
                                        limparBuffer();
                                    } while (!validarCor(cor));
                                    
                                    preencherPadrao(canvases.back().canvas, pixel, cor);
                                }
                                
                                std::cout << "Canvas criado com sucesso!\n";
                                exibirCanvas(canvases.back().canvas);
                                break;
                            }
                            
                            case 2: { // Deletar Canvas
                                if (canvases.empty()) {
                                    std::cout << "Nenhum canvas para deletar!\n";
                                    break;
                                }
                                
                                size_t indice = selecionarCanvas(canvases);
                                canvases[indice].canvas.~Canva();
                                canvases[indice].ativo = false;
                                std::cout << "Canvas '" << canvases[indice].nome << "' deletado com sucesso!\n";
                                break;
                            }
                            
                            case 3: { // Listar Canvas
                                exibirCanvas(canvases);
                                break;
                            }
                            
                            case 4: // Voltar
                                break;
                                
                            default:
                                std::cout << "Opção inválida!\n";
                        }
                    } while (subOpcao != 4);
                    break;
                }
                
                case 2: { // Editar Canvas
                    if (canvases.empty()) {
                        std::cout << "Nenhum canvas disponível para edição!\n";
                        break;
                    }
                    
                    do {
                        mostrarSubMenuEditar();
                        if (!(std::cin >> subOpcao)) {
                            throw std::invalid_argument("Opção inválida");
                        }
                        limparBuffer();
                        
                        try {
                            switch (subOpcao) {
                                case 1: { // Editar pixel
                                    size_t indice = selecionarCanvas(canvases);
                                    editarPixel(canvases[indice].canvas);
                                    exibirCanvas(canvases[indice].canvas);
                                    break;
                                }
                                
                                case 2: { // Adicionar borda
                                    size_t indice = selecionarCanvas(canvases);
                                    char cor;
                                    do {
                                        std::cout << "Digite a cor da borda (K,R,G,Y,B,M,C,W): ";
                                        std::cin >> cor;
                                        cor = std::toupper(cor);
                                        limparBuffer();
                                    } while (!validarCor(cor));
                                    
                                    desenharBorda(canvases[indice].canvas, cor);
                                    exibirCanvas(canvases[indice].canvas);
                                    break;
                                }
                                
                                case 3: { // Preencher com padrão
                                    size_t indice = selecionarCanvas(canvases);
                                    char pixel, cor;
                                    std::cout << "Digite o caractere para preenchimento: ";
                                    std::cin >> pixel;
                                    limparBuffer();
                                    
                                    do {
                                        std::cout << "Digite a cor (K,R,G,Y,B,M,C,W): ";
                                        std::cin >> cor;
                                        cor = std::toupper(cor);
                                        limparBuffer();
                                    } while (!validarCor(cor));
                                    
                                    preencherPadrao(canvases[indice].canvas, pixel, cor);
                                    exibirCanvas(canvases[indice].canvas);
                                    break;
                                }
                                
                                case 4: // Voltar
                                    break;
                                    
                                default:
                                    std::cout << "Opção inválida!\n";
                            }
                        } catch (const std::exception& e) {
                            std::cerr << "Erro: " << e.what() << std::endl;
                        }
                    } while (subOpcao != 4);
                    break;
                }
                
                case 3: { // Visualizar Canvas
                    if (canvases.empty()) {
                        std::cout << "Nenhum canvas disponível para visualização!\n";
                        break;
                    }
                    
                    do {
                        mostrarSubMenuVisualizar();
                        if (!(std::cin >> subOpcao)) {
                            throw std::invalid_argument("Opção inválida");
                        }
                        limparBuffer();
                        
                        try {
                            switch (subOpcao) {
                                case 1: { // Visualizar específico
                                    size_t indice = selecionarCanvas(canvases);
                                    exibirCanvas(canvases[indice].canvas);
                                    break;
                                }
                                
                                case 2: { // Visualizar todos
                                    for (size_t i = 0; i < canvases.size(); i++) {
                                        if (canvases[i].ativo) {
                                            std::cout << "\nCanvas '" << canvases[i].nome << "':\n";
                                            exibirCanvas(canvases[i].canvas);
                                        }
                                    }
                                    break;
                                }
                                
                                case 3: // Voltar
                                    break;
                                    
                                default:
                                    std::cout << "Opção inválida!\n";
                            }
                        } catch (const std::exception& e) {
                            std::cerr << "Erro: " << e.what() << std::endl;
                        }
                    } while (subOpcao != 3);
                    break;
                }
                
                case 0: {
                    for (auto& canvas : canvases) {
                        if (canvas.ativo) {
                            canvas.canvas.~Canva();
                        }
                    }
                    std::cout << "Saindo do programa...\n";
                    break;
                }
                
                default:
                    std::cout << "Opção inválida! Escolha um número entre 0 e 3.\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Erro: " << e.what() << std::endl;
            limparBuffer();
        }
    } while (opcao != 0);
    
    return 0;
}