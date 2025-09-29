# Canvas Editor

## 📝 Descrição
Este é um editor de canvas em modo texto desenvolvido em C++. O programa permite criar e manipular múltiplos canvas, adicionar bordas, preencher com padrões e editar pixels individualmente com suporte a cores ANSI.

## ✨ Funcionalidades

### Gerenciamento de Canvas
- ✏️ Criar múltiplos canvas com nomes personalizados
- 🗑️ Deletar canvas específicos
- 📋 Listar todos os canvas existentes
- 📏 Definir dimensões personalizadas (largura x altura)

### Edição
- 🎨 Editar pixels individuais
- 🖼️ Adicionar bordas personalizadas
- 🎯 Preencher canvas com padrões
- 🌈 Suporte a 8 cores diferentes:
  - K: Preto (Black)
  - R: Vermelho (Red)
  - G: Verde (Green)
  - Y: Amarelo (Yellow)
  - B: Azul (Blue)
  - M: Magenta
  - C: Ciano (Cyan)
  - W: Branco (White)

### Visualização
- 👁️ Visualizar canvas específico
- 📺 Visualizar todos os canvas ativos
- 🎭 Visualização com cores ANSI no terminal

## 🚀 Como Usar

### Compilação
```bash
g++ -o canvas Canvas.cpp main.cpp
```

### Execução
```bash
./canvas
```

### Estrutura do Menu
```
=== Menu Principal do Canvas ===
1. Gerenciar Canvas
   ├─ 1. Criar novo Canvas
   ├─ 2. Deletar Canvas
   ├─ 3. Listar todos os Canvas
   └─ 4. Voltar

2. Editar Canvas
   ├─ 1. Editar pixel
   ├─ 2. Adicionar borda
   ├─ 3. Preencher com padrão
   └─ 4. Voltar

3. Visualizar Canvas
   ├─ 1. Visualizar Canvas específico
   ├─ 2. Visualizar todos os Canvas
   └─ 3. Voltar

0. Sair
```

## 💡 Exemplos de Uso

### Criar um Novo Canvas
1. Selecione "1" no menu principal (Gerenciar Canvas)
2. Selecione "1" para criar novo canvas
3. Digite um nome para o canvas
4. Insira as dimensões desejadas
5. Escolha se deseja adicionar borda
6. Escolha se deseja preencher com padrão

### Editar um Pixel
1. Selecione "2" no menu principal (Editar Canvas)
2. Selecione "1" para editar pixel
3. Escolha o canvas a ser editado
4. Insira as coordenadas x e y
5. Digite o caractere desejado
6. Escolha uma cor (K,R,G,Y,B,M,C,W)

## 🛠️ Requisitos
- Compilador C++ (g++)
- Terminal com suporte a cores ANSI
- Sistema operacional Linux (testado em Linux)

## ⚠️ Tratamento de Erros
O programa inclui tratamento robusto de erros para:
- Entradas inválidas
- Coordenadas fora dos limites
- Cores inválidas
- Problemas de memória
- Seleção de canvas inexistente

## 📚 Estrutura do Projeto
- `main.cpp`: Implementação do menu e interface do usuário
- `Canvas.cpp`: Implementação das funcionalidades do canvas
- `Canvas.hpp`: Declaração da classe e métodos do canvas

## 👥 Contribuição
Sinta-se à vontade para contribuir com este projeto. Algumas ideias para melhorias:
- Adicionar suporte a salvar/carregar canvas em arquivo
- Implementar novas formas de desenho (linhas, retângulos, etc.)
- Adicionar suporte a desfazer/refazer
- Implementar copy/paste entre canvas

## 🤝 Autor
Gabriel Costa Reis

---
Desenvolvido como parte do trabalho prático da disciplina INF112 - Programação II (2025/2)
Universidade Federal de Viçosa
