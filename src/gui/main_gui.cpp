#include "MainWindow.hpp"
#include <QApplication>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    app.setApplicationName("Canvas Editor");

    MainWindow janela;
    janela.show();
    return app.exec();
}
