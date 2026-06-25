#ifndef CANVAS_GUI_CANVASVIEW_HPP
#define CANVAS_GUI_CANVASVIEW_HPP

#include "core/Canvas.hpp"
#include "core/History.hpp"

#include <QImage>
#include <QWidget>

// Widget que renderiza um Canvas como uma grade e permite editar com o mouse.
//
// Concentra toda a interacao de desenho (ferramentas, preview de formas,
// historico de undo/redo) e expoe sinais para a janela principal reagir.
class CanvasView : public QWidget {
    Q_OBJECT
public:
    enum class Ferramenta {
        Pincel,
        Borracha,
        Linha,
        Retangulo,
        RetanguloPreenchido,
        Circulo,
        CirculoPreenchido,
        Elipse,
        Balde
    };

    explicit CanvasView(QWidget* parent = nullptr);

    void novoCanvas(int largura, int altura);
    void definirCanvas(const canvas::Canvas& c);
    const canvas::Canvas& canvasAtual() const { return canvas_; }

    void definirFerramenta(Ferramenta f) { ferramenta_ = f; }
    void definirCor(canvas::Color c) { cor_ = c; }
    void definirCaractere(char ch) { caractere_ = ch; }
    char caractere() const { return caractere_; }

    void desfazer();
    void refazer();
    bool podeDesfazer() const { return historico_.podeDesfazer(); }
    bool podeRefazer() const { return historico_.podeRefazer(); }

    void limpar();
    void aplicarBorda(canvas::Color cor);

    // Renderiza o canvas para uma QImage (para exportar PNG/JPG).
    QImage renderizarImagem(int escala) const;

    QSize minimumSizeHint() const override { return {320, 240}; }

signals:
    void posicaoMudou(int x, int y); // (-1, -1) quando o cursor sai da grade
    void canvasModificado();

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void leaveEvent(QEvent*) override;

private:
    struct Geometria {
        int tam;     // pixels por celula (quadrada)
        int offsetX; // borda esquerda para centralizar
        int offsetY;
    };
    Geometria geometria() const;
    bool celulaNoPonto(const QPoint& p, int& cx, int& cy) const;
    void desenharCelulaPincel(int cx, int cy);
    void aplicarFormaPreview(); // redesenha sobre o snapshot durante o arraste
    void commitForma();

    canvas::Canvas canvas_{32, 20};
    canvas::Historico historico_;

    Ferramenta ferramenta_ = Ferramenta::Pincel;
    canvas::Color cor_ = canvas::Color::White;
    char caractere_ = '#';

    bool arrastando_ = false;
    bool desenhandoForma_ = false;
    int inicioX_ = 0, inicioY_ = 0;
    int atualX_ = 0, atualY_ = 0;
    canvas::Canvas snapshot_; // base intacta durante o preview da forma
};

#endif // CANVAS_GUI_CANVASVIEW_HPP
