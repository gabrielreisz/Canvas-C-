#include "Canvas.hpp"
#include <iostream>

int main() {
  // Declaracao do canvas principal
  Canva tela;

  // PASSO 1: Criar e exibir um canvas vazio
  std::cout << "PASSO 1: Canvas Vazio\n";
  CriarCanva(tela, 12, 40);
  ImprimirCanva(tela);

  // PASSO 2: Desenhar pontos coloridos
  std::cout << "\nPASSO 2: Desenhando Pontos\n";
  DesenharPonto(tela, 5, 2, '@', 'R');
  DesenharPonto(tela, 8, 5, '@', 'G');
  ImprimirCanva(tela);

  // PASSO 3: Desenhar uma linha horizontal
  std::cout << "\nPASSO 3: Desenhando Linha\n";
  DesenharLinha(tela, 2, 8, 15, 8, '=', 'B');
  ImprimirCanva(tela);

  // PASSO 4: Desenhar a borda de um retangulo
  std::cout << "\nPASSO 4: Desenhando Retangulo (Borda)\n";
  DesenharRetangulo(tela, 18, 1, 12, 5, '#', 'Y');
  ImprimirCanva(tela);

  // PASSO 5: Desenhar um retangulo preenchido
  std::cout << "\nPASSO 5: Desenhando Retangulo (Preenchido)\n";
  DesenharRetanguloPreenchido(tela, 28, 7, 10, 4, '*', 'C');
  ImprimirCanva(tela);

  // PASSO 6: Criar canvas para sobreposição e comparação
  std::cout << "\nPASSO 6: Criando Canvas A e B\n";
  Canva telaA, telaB, telaResultante;
  CriarCanva(telaA, 8, 20);
  CriarCanva(telaB, 8, 20);

  // Desenhar formas em cada canvas
  DesenharRetangulo(telaA, 1, 1, 10, 6, 'A', 'M');
  DesenharRetanguloPreenchido(telaB, 5, 2, 12, 4, 'B', 'G');

  std::cout << "Tela A:\n";
  ImprimirCanva(telaA);
  std::cout << "Tela B:\n";
  ImprimirCanva(telaB);

  // PASSO 7: Sobrepor os dois canvas
  std::cout << "\nPASSO 7: Sobrepondo A e B\n";
  SobreporCanvas(telaResultante, telaA, telaB);
  ImprimirCanva(telaResultante);

  // PASSO 8: Comparar os dois canvas
  std::cout << "\nPASSO 8: Comparando Tela A e Tela B..." << std::endl;
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
  
  // Teste extra: Redimensionar
  std::cout << "\nTeste Extra: Redimensionando a tela principal\n";
  RedimensionarCanva(tela, 15, 45);
  ImprimirCanva(tela);

  // PASSO 9: Liberar toda a memória alocada
  std::cout << "\nPASSO 9: Liberando toda a memoria.\n";
  DestruirCanvas(tela);
  DestruirCanvas(telaA);
  DestruirCanvas(telaB);
  DestruirCanvas(telaResultante);

  return 0;
}