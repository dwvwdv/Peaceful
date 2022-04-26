#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icon.ico"));       //Icon設定
    this->setAttribute(Qt::WA_TranslucentBackground);//背景透明化
    this->setWindowFlags(Qt::FramelessWindowHint);   //無邊窗口
//    this->setFixedSize(300,270);                           //固定窗口大小
    this->setWindowTitle("Peaceful");	 //視窗名稱設定

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
