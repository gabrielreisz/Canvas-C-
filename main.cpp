#include "Canvas.hpp"
#include <iostream>

int main() {
  // Declaracao do canvas principal
  Canva tela;

  // PASSO 1: Criar e exibir um canvas vazio
  CriarCanva(tela, 12, 40);
  ImprimirCanva(tela);

  // PASSO 2: Desenhar pontos coloridos
  DesenharPonto(tela, 5, 2, '@', 'R'); // Ponto vermelho
  DesenharPonto(tela, 8, 5, '@', 'G'); // Ponto verde
  ImprimirCanva(tela);

  // PASSO 3: Desenhar uma linha horizontal
  DesenharLinha(tela, 2, 8, 15, 8, '=', 'B'); // Linha azul
  ImprimirCanva(tela);

  // PASSO 4: Desenhar a borda de um retangulo
  DesenharRetangulo(tela, 18, 1, 12, 5, '#', 'Y'); // Retangulo amarelo
  ImprimirCanva(tela);

  // PASSO 5: Desenhar um retangulo preenchido
  DesenharRetanguloPreenchido(tela, 28, 7, 10, 4, '*', 'C'); // Retangulo ciano
  ImprimirCanva(tela);

  // PASSO 6: Criar canvas para sobreposição e comparação
  Canva telaA, telaB, telaResultante;
  CriarCanva(telaA, 8, 20);
  CriarCanva(telaB, 8, 20);

  // Desenhar formas em cada canvas
  DesenharRetangulo(telaA, 1, 1, 10, 6, 'A', 'M');
  DesenharRetanguloPreenchido(telaB, 5, 2, 12, 4, 'B', 'G');

  ImprimirCanva(telaA);
  ImprimirCanva(telaB);

  // PASSO 7: Sobrepor os dois canvas
  SobreporCanvas(telaResultante, telaA, telaB);
  ImprimirCanva(telaResultante);

  // PASSO 8: Comparar os dois canvas
  std::cout << " \n 7 . Comparando Tela A e Tela B ... " << std::endl;
  std::cout << "Resultado: "
            << (CompararCanvas(telaA, telaB) ? "Iguais" : "Diferentes")
            << std::endl;

  Canva telaA_copia;
  CriarCanva(telaA_copia, 8, 20);
  DesenharRetangulo(telaA_copia, 1, 1, 10, 6, 'A', 'M');
  std::cout << "\nComparando Tela A e uma copia de A..." << std::endl;
  std::cout << "Resultado: "
            << (CompararCanvas(telaA, telaA_copia) ? "Iguais" : "Diferentes")
            << std::endl;
  DestruirCanvas(telaA_copia);
  
  // Redimensionar
  std::cout << "\nTeste Extra: Redimensionando a tela principal\n";
  RedimensionarCanva(tela, 15, 45);
  ImprimirCanva(tela);

  // PASSO 9: Liberar toda a memória alocada
  DestruirCanvas(tela);
  DestruirCanvas(telaA);
  DestruirCanvas(telaB);
  DestruirCanvas(telaResultante);

  return 0;
}