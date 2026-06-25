# Canvas Editor 2.0

## 📝 Descrição
Editor de arte em caracteres (ASCII/ANSI) escrito em **C++17**. Permite criar e
manipular múltiplos canvas, desenhar formas geométricas, pintar com cores e
exportar o resultado. A versão 2.0 traz uma **interface gráfica em Qt6**, uma
**biblioteca central reutilizável** e algoritmos de desenho clássicos.

## 🏗️ Arquitetura

O projeto foi modularizado em uma biblioteca central e duas interfaces que a
consomem:

```
src/
├── core/                  → biblioteca canvas_core (lógica pura, sem I/O de tela)
│   ├── Color.hpp          → paleta de 8 cores (código de letra, ANSI e RGB)
│   ├── Canvas.{hpp,cpp}   → modelo de dados (RAII com std::vector)
│   ├── Drawing.{hpp,cpp}  → algoritmos: linha, retângulo, círculo, elipse, flood fill
│   ├── History.{hpp,cpp}  → desfazer/refazer (snapshots)
│   ├── CanvasIO.{hpp,cpp} → salvar/carregar (.canvas) e exportar (PPM/TXT)
│   └── CanvasManager.{hpp,cpp} → coleção de documentos abertos
├── cli/                   → interface de terminal (canvas_cli)
└── gui/                   → interface gráfica Qt6 (canvas_gui)
    ├── CanvasView.{hpp,cpp} → widget de grade + edição com mouse
    └── MainWindow.{hpp,cpp} → janela, ferramentas, paleta, menus
```

### Por que essa separação?
- **`core` não depende de nada de interface** → a mesma lógica roda no terminal,
  na GUI e em testes.
- **Modelo `Canvas` segue a "Regra do Zero"**: usa `std::vector`, então é
  copiável/movível com segurança. Isso elimina os vazamentos de memória e o
  *double-free* da versão original baseada em `char**`.

## ✨ Funcionalidades

### Desenho
- 🖌️ Pincel e borracha
- 📏 **Linha** — algoritmo de **Bresenham** (apenas aritmética inteira, O(n))
- ▭ Retângulo (contorno e preenchido)
- ⭕ **Círculo** — algoritmo **midpoint** (simetria de 8 octantes)
- 🥚 **Elipse** — algoritmo midpoint de duas regiões
- 🪣 **Balde de tinta** — *flood fill* com BFS iterativa, O(largura × altura)
- 🔳 Borda automática
- 🌈 8 cores (K, R, G, Y, B, M, C, W) com renderização ANSI no terminal e RGB na GUI

### Edição e produtividade
- ↩️ **Desfazer / Refazer** (histórico por canvas)
- 📐 Redimensionar preservando o conteúdo
- 🗂️ Múltiplos canvas nomeados
- 🧩 Sobreposição e comparação de canvas (na biblioteca)

### Arquivos
- 💾 Salvar/abrir no formato próprio **`.canvas`** (preserva caracteres e cores)
- 🖼️ Exportar como **imagem** (PNG/JPG na GUI, PPM na CLI)
- 📄 Exportar como **texto puro**

## 🚀 Como compilar

Requisitos: **CMake ≥ 3.16**, compilador C++17 e (opcional) **Qt6** para a GUI.

```bash
cmake -S . -B build              # adicione -DCMAKE_PREFIX_PATH=$(brew --prefix qt) no macOS
cmake --build build -j
```

Os binários ficam em `build/bin/`:
- `canvas_cli` — interface de terminal (sempre compilada)
- `canvas_gui` — interface gráfica (compilada apenas se o Qt6 for encontrado)

> Sem o Qt instalado, o CMake compila apenas a CLI automaticamente.

## 🖥️ Como usar

### Interface gráfica
```bash
./build/bin/canvas_gui
```
Escolha uma ferramenta na barra superior, uma cor na paleta inferior e o
caractere de desenho. Clique e arraste na grade. As formas mostram um **preview
em tempo real** durante o arraste.

### Terminal
```bash
./build/bin/canvas_cli
```
Menu numérico com todas as operações (criar, desenhar formas, balde, desfazer,
salvar, exportar...).

## 🛠️ Detalhes de implementação relevantes
- **Bresenham** para linhas e **midpoint** para círculos/elipses: zero ponto
  flutuante no caminho crítico, sem divisões por pixel.
- **Flood fill iterativo (fila)** em vez de recursão, evitando estouro de pilha
  em regiões grandes.
- **Clipping automático**: `Canvas::definir` ignora coordenadas fora dos limites,
  simplificando todos os algoritmos de desenho.
- **`-Wall -Wextra`** habilitados na biblioteca central.

## 👤 Autor
Gabriel Costa Reis

---
Evolução do trabalho prático da disciplina **INF112 – Programação II (2025/2)**
— Universidade Federal de Viçosa.
