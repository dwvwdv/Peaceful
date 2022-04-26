#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    player = new QMediaPlayer;
    player->setMedia(QUrl("qrc:/one.mp3"));
    connect(ui->ControlList,QListWidget::doubleClicked,this,Widget::slotControl);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::slotControl(){
    player->play();
}
