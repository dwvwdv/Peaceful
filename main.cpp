#include "widget.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile qss(R"(:/DarkGreen.qss)");
    qss.open(QFile::ReadOnly);
    if(qss.isOpen()){
        qApp->setStyleSheet(qss.readAll());
        qss.close();
    }

    Widget w;
    w.show();
    return a.exec();
}
