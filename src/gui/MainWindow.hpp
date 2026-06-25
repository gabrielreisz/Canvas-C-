#ifndef CANVAS_GUI_MAINWINDOW_HPP
#define CANVAS_GUI_MAINWINDOW_HPP

#include "CanvasView.hpp"
#include <QMainWindow>

class QLabel;
class QActionGroup;

// Janela principal da interface grafica: monta a barra de ferramentas, a
// paleta de cores, o seletor de caractere e conecta tudo ao CanvasView.
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void novo();
    void abrir();
    void salvar();
    void exportarImagem();
    void redimensionar();
    void escolherCaractere();

private:
    void criarBarraFerramentas();
    void criarPaleta();
    void criarMenu();
    void atualizarStatus(int x, int y);

    CanvasView* view_;
    QLabel* statusPos_;
    QActionGroup* grupoFerramentas_;
    QString caminhoAtual_;
};

#endif // CANVAS_GUI_MAINWINDOW_HPP
