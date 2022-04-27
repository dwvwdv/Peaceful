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
    connect(ui->ControlList,QListWidget::doubleClicked,this,Widget::slotControlChange);
    connect(ui->SongList,QListWidget::doubleClicked,this,[&]{
        slotChangeSong(AllSong[ui->SongList->currentItem()->text()]);
//        it = AllSong[AllSong[ui->SongList->currentItem()->text()];
    });

    connect(ui->SoundSize,QSlider::valueChanged,this,[&](int n){
        player->setVolume(n);
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::slotControlChange(){
    if(ui->ControlList->currentItem()->text() == "Play"){
        controlStatus = ControlStatus::Play;
        ui->ControlList->currentItem()->setText("Pause");
    }
    else if(ui->ControlList->currentItem()->text() == "Pause"){
        controlStatus = ControlStatus::Pause;
        ui->ControlList->currentItem()->setText("Play");
    }
    else if(ui->ControlList->currentItem()->text() == "Add Song"){
        controlStatus = ControlStatus::AddSong;

    }
    //update
    slotControl();
}

void Widget::slotControl(){
    switch (controlStatus) {
        case ControlStatus::Play:
            player->play();
            break;
        case ControlStatus::Pause:
            player->pause();
            break;
        case ControlStatus::Previous:
            break;
        case ControlStatus::Next:
            break;
        case ControlStatus::AddSong:{
            QStringList files = QFileDialog::getOpenFileNames(this,"Select Music",QDir::homePath(),"file (*.mp3 *.wav)");{
                for(auto &file:files){
                    QFileInfo fi(file);
                    AllSong.insert(fi.fileName(),fi.filePath());
                    if(fi.fileName() != "")
                        ui->SongList->addItem(fi.fileName());
                    qDebug() << AllSong[fi.fileName()];
                }
            }

            break;
        }

        default:
            break;
    }
}

void Widget::slotChangeSong(QString newSong){
    player->setMedia(QUrl(newSong));
    player->play();
    ui->ControlList->item(0)->setText("Pause");
}

void Widget::slotSoundSize(){

}
