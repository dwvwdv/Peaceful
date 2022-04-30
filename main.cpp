#include "widget.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QDesktopWidget>

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
    QDesktopWidget *screenResolution = QApplication::desktop();
    qDebug() << w.geometry().width() << " " << w.geometry().height();
    qDebug() << screenResolution->width() << screenResolution->height();
    w.setGeometry(screenResolution->width() - w.geometry().width(),
                  screenResolution->height()- w.geometry().height(),
                  w.geometry().width(),
                  w.geometry().height());
    w.show();
    return a.exec();
}
