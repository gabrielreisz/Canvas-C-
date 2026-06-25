#include "MainWindow.hpp"

#include "core/CanvasIO.hpp"

#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QFileDialog>
#include <QInputDialog>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QStatusBar>
#include <QToolBar>

namespace {
QColor paraQColor(canvas::Color c) {
    canvas::RGB rgb = canvas::paraRGB(c);
    return QColor(rgb.r, rgb.g, rgb.b);
}
} // namespace

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Canvas Editor — Qt");
    resize(900, 640);

    view_ = new CanvasView(this);
    setCentralWidget(view_);

    criarMenu();
    criarBarraFerramentas();
    criarPaleta();

    statusPos_ = new QLabel("Pronto", this);
    statusBar()->addPermanentWidget(statusPos_);

    connect(view_, &CanvasView::posicaoMudou, this, &MainWindow::atualizarStatus);
}

void MainWindow::criarMenu() {
    QMenu* arquivo = menuBar()->addMenu("&Arquivo");
    arquivo->addAction("&Novo", QKeySequence::New, this, &MainWindow::novo);
    arquivo->addAction("&Abrir...", QKeySequence::Open, this, &MainWindow::abrir);
    arquivo->addAction("&Salvar...", QKeySequence::Save, this, &MainWindow::salvar);
    arquivo->addAction("Exportar &imagem...", this, &MainWindow::exportarImagem);
    arquivo->addSeparator();
    arquivo->addAction("Sai&r", QKeySequence::Quit, qApp, &QApplication::quit);

    QMenu* editar = menuBar()->addMenu("&Editar");
    editar->addAction("&Desfazer", QKeySequence::Undo, view_, &CanvasView::desfazer);
    editar->addAction("&Refazer", QKeySequence::Redo, view_, &CanvasView::refazer);
    editar->addSeparator();
    editar->addAction("&Limpar tudo", view_, &CanvasView::limpar);
    editar->addAction("Re&dimensionar...", this, &MainWindow::redimensionar);
    editar->addAction("Adicionar &borda", this,
                      [this] { view_->aplicarBorda(canvas::Color::White); });
}

void MainWindow::criarBarraFerramentas() {
    QToolBar* tb = addToolBar("Ferramentas");
    tb->setMovable(false);
    grupoFerramentas_ = new QActionGroup(this);

    struct Item {
        const char* rotulo;
        CanvasView::Ferramenta ferramenta;
    };
    const Item itens[] = {
        {"Pincel", CanvasView::Ferramenta::Pincel},
        {"Borracha", CanvasView::Ferramenta::Borracha},
        {"Linha", CanvasView::Ferramenta::Linha},
        {"Retangulo", CanvasView::Ferramenta::Retangulo},
        {"Ret. cheio", CanvasView::Ferramenta::RetanguloPreenchido},
        {"Circulo", CanvasView::Ferramenta::Circulo},
        {"Circ. cheio", CanvasView::Ferramenta::CirculoPreenchido},
        {"Elipse", CanvasView::Ferramenta::Elipse},
        {"Balde", CanvasView::Ferramenta::Balde},
    };
    for (const auto& it : itens) {
        QAction* a = tb->addAction(it.rotulo);
        a->setCheckable(true);
        grupoFerramentas_->addAction(a);
        CanvasView::Ferramenta f = it.ferramenta;
        connect(a, &QAction::triggered, this,
                [this, f] { view_->definirFerramenta(f); });
        if (it.ferramenta == CanvasView::Ferramenta::Pincel)
            a->setChecked(true);
    }

    tb->addSeparator();
    QAction* charAct = tb->addAction("Caractere: #");
    connect(charAct, &QAction::triggered, this, &MainWindow::escolherCaractere);
    charAct->setObjectName("acaoCaractere");

    tb->addAction("Desfazer", view_, &CanvasView::desfazer);
    tb->addAction("Refazer", view_, &CanvasView::refazer);
}

void MainWindow::criarPaleta() {
    QToolBar* tb = new QToolBar("Cores", this);
    addToolBar(Qt::BottomToolBarArea, tb);
    tb->addWidget(new QLabel(" Cores: ", tb));

    for (canvas::Color c : canvas::todasAsCores()) {
        QPushButton* b = new QPushButton(tb);
        b->setFixedSize(28, 28);
        b->setToolTip(canvas::nomeCor(c));
        b->setStyleSheet(QString("background-color: %1; border: 1px solid #555;")
                             .arg(paraQColor(c).name()));
        connect(b, &QPushButton::clicked, this,
                [this, c] { view_->definirCor(c); });
        tb->addWidget(b);
    }
}

void MainWindow::escolherCaractere() {
    bool ok = false;
    QString atual = QString(QChar(view_->caractere()));
    QString s = QInputDialog::getText(this, "Caractere",
                                      "Caractere para desenhar:", QLineEdit::Normal,
                                      atual, &ok);
    if (ok && !s.isEmpty()) {
        char ch = s.at(0).toLatin1();
        view_->definirCaractere(ch);
        if (QAction* a = findChild<QAction*>("acaoCaractere"))
            a->setText(QString("Caractere: %1").arg(ch));
    }
}

void MainWindow::novo() {
    bool ok = false;
    int largura = QInputDialog::getInt(this, "Novo canvas", "Largura:", 32, 1,
                                       300, 1, &ok);
    if (!ok)
        return;
    int altura = QInputDialog::getInt(this, "Novo canvas", "Altura:", 20, 1, 200,
                                      1, &ok);
    if (!ok)
        return;
    view_->novoCanvas(largura, altura);
    caminhoAtual_.clear();
}

void MainWindow::abrir() {
    QString caminho = QFileDialog::getOpenFileName(this, "Abrir canvas", {},
                                                   "Canvas (*.canvas)");
    if (caminho.isEmpty())
        return;
    try {
        view_->definirCanvas(canvas::io::carregar(caminho.toStdString()));
        caminhoAtual_ = caminho;
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Erro", e.what());
    }
}

void MainWindow::salvar() {
    QString caminho = QFileDialog::getSaveFileName(this, "Salvar canvas",
                                                   caminhoAtual_, "Canvas (*.canvas)");
    if (caminho.isEmpty())
        return;
    if (!caminho.endsWith(".canvas"))
        caminho += ".canvas";
    try {
        canvas::io::salvar(view_->canvasAtual(), caminho.toStdString());
        caminhoAtual_ = caminho;
        statusBar()->showMessage("Salvo em " + caminho, 3000);
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Erro", e.what());
    }
}

void MainWindow::exportarImagem() {
    QString caminho = QFileDialog::getSaveFileName(
        this, "Exportar imagem", {}, "Imagem PNG (*.png);;Imagem JPEG (*.jpg)");
    if (caminho.isEmpty())
        return;
    QImage img = view_->renderizarImagem(24);
    if (img.save(caminho))
        statusBar()->showMessage("Imagem exportada: " + caminho, 3000);
    else
        QMessageBox::warning(this, "Erro", "Falha ao salvar a imagem.");
}

void MainWindow::redimensionar() {
    bool ok = false;
    int largura = QInputDialog::getInt(this, "Redimensionar", "Largura:",
                                       view_->canvasAtual().largura(), 1, 300, 1,
                                       &ok);
    if (!ok)
        return;
    int altura = QInputDialog::getInt(this, "Redimensionar", "Altura:",
                                      view_->canvasAtual().altura(), 1, 200, 1,
                                      &ok);
    if (!ok)
        return;
    canvas::Canvas c = view_->canvasAtual();
    c.redimensionar(largura, altura);
    view_->definirCanvas(c);
}

void MainWindow::atualizarStatus(int x, int y) {
    if (x < 0)
        statusPos_->setText(QString("%1 x %2")
                                .arg(view_->canvasAtual().largura())
                                .arg(view_->canvasAtual().altura()));
    else
        statusPos_->setText(QString("(%1, %2)").arg(x).arg(y));
}
