#include "CanvasView.hpp"

#include "core/Drawing.hpp"

#include <QFont>
#include <QMouseEvent>
#include <QPainter>
#include <algorithm>

namespace {
QColor paraQColor(canvas::Color c) {
    canvas::RGB rgb = canvas::paraRGB(c);
    return QColor(rgb.r, rgb.g, rgb.b);
}
const QColor kFundo{18, 18, 26};
const QColor kGrade{40, 40, 55};
} // namespace

CanvasView::CanvasView(QWidget* parent) : QWidget(parent) {
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

void CanvasView::novoCanvas(int largura, int altura) {
    canvas_ = canvas::Canvas(largura, altura);
    historico_.limpar();
    update();
    emit canvasModificado();
}

void CanvasView::definirCanvas(const canvas::Canvas& c) {
    canvas_ = c;
    historico_.limpar();
    update();
    emit canvasModificado();
}

CanvasView::Geometria CanvasView::geometria() const {
    if (canvas_.vazio())
        return {1, 0, 0};
    int tam = std::min(width() / canvas_.largura(), height() / canvas_.altura());
    tam = std::max(tam, 1);
    int usadoX = tam * canvas_.largura();
    int usadoY = tam * canvas_.altura();
    return {tam, (width() - usadoX) / 2, (height() - usadoY) / 2};
}

bool CanvasView::celulaNoPonto(const QPoint& p, int& cx, int& cy) const {
    Geometria g = geometria();
    int x = (p.x() - g.offsetX) / g.tam;
    int y = (p.y() - g.offsetY) / g.tam;
    if (p.x() < g.offsetX || p.y() < g.offsetY || !canvas_.dentro(x, y))
        return false;
    cx = x;
    cy = y;
    return true;
}

void CanvasView::paintEvent(QPaintEvent*) {
    QPainter pintor(this);
    pintor.fillRect(rect(), kFundo);
    if (canvas_.vazio())
        return;

    Geometria g = geometria();
    QFont fonte("Menlo");
    fonte.setStyleHint(QFont::Monospace);
    fonte.setPixelSize(std::max(6, g.tam - 2));
    pintor.setFont(fonte);

    for (int y = 0; y < canvas_.altura(); ++y) {
        for (int x = 0; x < canvas_.largura(); ++x) {
            QRect r(g.offsetX + x * g.tam, g.offsetY + y * g.tam, g.tam, g.tam);
            const canvas::Cell& cel = canvas_.em(x, y);
            if (cel.ch != ' ') {
                pintor.setPen(paraQColor(cel.cor));
                pintor.drawText(r, Qt::AlignCenter, QString(QChar(cel.ch)));
            }
        }
    }

    // Grade sutil (apenas quando as celulas sao grandes o suficiente).
    if (g.tam >= 8) {
        pintor.setPen(kGrade);
        for (int x = 0; x <= canvas_.largura(); ++x)
            pintor.drawLine(g.offsetX + x * g.tam, g.offsetY, g.offsetX + x * g.tam,
                            g.offsetY + canvas_.altura() * g.tam);
        for (int y = 0; y <= canvas_.altura(); ++y)
            pintor.drawLine(g.offsetX, g.offsetY + y * g.tam,
                            g.offsetX + canvas_.largura() * g.tam,
                            g.offsetY + y * g.tam);
    }
}

void CanvasView::desenharCelulaPincel(int cx, int cy) {
    if (ferramenta_ == Ferramenta::Borracha)
        canvas_.definir(cx, cy, ' ', canvas::Color::White);
    else
        canvas_.definir(cx, cy, caractere_, cor_);
}

void CanvasView::aplicarFormaPreview() {
    canvas_ = snapshot_; // restaura a base e redesenha a forma atual
    using namespace canvas::desenho;
    char ch = caractere_;
    canvas::Color cor = cor_;
    int x0 = inicioX_, y0 = inicioY_, x1 = atualX_, y1 = atualY_;

    switch (ferramenta_) {
    case Ferramenta::Linha:
        linha(canvas_, x0, y0, x1, y1, ch, cor);
        break;
    case Ferramenta::Retangulo:
        retangulo(canvas_, std::min(x0, x1), std::min(y0, y1),
                  std::abs(x1 - x0) + 1, std::abs(y1 - y0) + 1, ch, cor);
        break;
    case Ferramenta::RetanguloPreenchido:
        retanguloPreenchido(canvas_, std::min(x0, x1), std::min(y0, y1),
                            std::abs(x1 - x0) + 1, std::abs(y1 - y0) + 1, ch, cor);
        break;
    case Ferramenta::Circulo: {
        int raio = (int)std::round(std::hypot(x1 - x0, y1 - y0));
        circulo(canvas_, x0, y0, raio, ch, cor);
        break;
    }
    case Ferramenta::CirculoPreenchido: {
        int raio = (int)std::round(std::hypot(x1 - x0, y1 - y0));
        circuloPreenchido(canvas_, x0, y0, raio, ch, cor);
        break;
    }
    case Ferramenta::Elipse:
        elipse(canvas_, x0, y0, std::abs(x1 - x0), std::abs(y1 - y0), ch, cor);
        break;
    default:
        break;
    }
    update();
}

void CanvasView::mousePressEvent(QMouseEvent* e) {
    int cx, cy;
    if (e->button() != Qt::LeftButton || !celulaNoPonto(e->pos(), cx, cy))
        return;

    switch (ferramenta_) {
    case Ferramenta::Pincel:
    case Ferramenta::Borracha:
        historico_.registrar(canvas_);
        desenharCelulaPincel(cx, cy);
        arrastando_ = true;
        update();
        emit canvasModificado();
        break;
    case Ferramenta::Balde:
        historico_.registrar(canvas_);
        if (ferramenta_ == Ferramenta::Balde)
            canvas::desenho::preencher(canvas_, cx, cy, caractere_, cor_);
        update();
        emit canvasModificado();
        break;
    default: // formas: comeca o preview
        snapshot_ = canvas_;
        inicioX_ = atualX_ = cx;
        inicioY_ = atualY_ = cy;
        desenhandoForma_ = true;
        aplicarFormaPreview();
        break;
    }
}

void CanvasView::mouseMoveEvent(QMouseEvent* e) {
    int cx, cy;
    bool dentro = celulaNoPonto(e->pos(), cx, cy);
    emit posicaoMudou(dentro ? cx : -1, dentro ? cy : -1);

    if (!dentro)
        return;
    if (arrastando_) {
        desenharCelulaPincel(cx, cy);
        update();
        emit canvasModificado();
    } else if (desenhandoForma_) {
        atualX_ = cx;
        atualY_ = cy;
        aplicarFormaPreview();
    }
}

void CanvasView::mouseReleaseEvent(QMouseEvent* e) {
    if (e->button() != Qt::LeftButton)
        return;
    if (arrastando_) {
        arrastando_ = false;
    } else if (desenhandoForma_) {
        desenhandoForma_ = false;
        historico_.registrar(snapshot_); // um unico passo de undo para a forma
        emit canvasModificado();
    }
}

void CanvasView::leaveEvent(QEvent*) { emit posicaoMudou(-1, -1); }

void CanvasView::desfazer() {
    if (!historico_.podeDesfazer())
        return;
    canvas_ = historico_.desfazer(canvas_);
    update();
    emit canvasModificado();
}

void CanvasView::refazer() {
    if (!historico_.podeRefazer())
        return;
    canvas_ = historico_.refazer(canvas_);
    update();
    emit canvasModificado();
}

void CanvasView::limpar() {
    historico_.registrar(canvas_);
    canvas_.limpar();
    update();
    emit canvasModificado();
}

void CanvasView::aplicarBorda(canvas::Color cor) {
    historico_.registrar(canvas_);
    canvas::desenho::borda(canvas_, cor);
    update();
    emit canvasModificado();
}

QImage CanvasView::renderizarImagem(int escala) const {
    escala = std::max(escala, 4);
    QImage img(canvas_.largura() * escala, canvas_.altura() * escala,
               QImage::Format_RGB32);
    img.fill(kFundo);
    QPainter pintor(&img);
    QFont fonte("Menlo");
    fonte.setStyleHint(QFont::Monospace);
    fonte.setPixelSize(escala - 2);
    pintor.setFont(fonte);
    for (int y = 0; y < canvas_.altura(); ++y) {
        for (int x = 0; x < canvas_.largura(); ++x) {
            const canvas::Cell& cel = canvas_.em(x, y);
            if (cel.ch == ' ')
                continue;
            pintor.setPen(paraQColor(cel.cor));
            pintor.drawText(QRect(x * escala, y * escala, escala, escala),
                            Qt::AlignCenter, QString(QChar(cel.ch)));
        }
    }
    return img;
}
