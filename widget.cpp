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
    playList = new QMediaPlaylist;
    playList->setPlaybackMode(QMediaPlaylist::Loop);

    //debug btn
    connect(ui->listBtn,QPushButton::clicked,this,[=]{
        qDebug() << playList->mediaCount();
    });

    //Control
    connect(ui->ControlList,QListWidget::doubleClicked,this,Widget::slotControlChange);

    //SongList
//    connect(ui->SongList,QListWidget::doubleClicked,this,[&]{
//        changeSong(AllSong[ui->SongList->currentItem()->text()]);
//    });

    //Now song icon
    connect(player,QMediaPlayer::currentMediaChanged,ui->SongList,[&]{
        if(preSongIndex != -1)
            ui->SongList->item(preSongIndex)->setIcon(QIcon(""));
        ui->SongList->item(playList->currentIndex())->setIcon(QIcon(":/icon.ico"));
        preSongIndex = playList->currentIndex();

        qDebug() << playList->currentIndex();
    });
    //Sound size
    connect(ui->SoundSize,QSlider::valueChanged,this,Widget::slotSoundSize);

    //Scan Music
    connect(ui->Auto_Add,QPushButton::clicked,this,Widget::slotAutoAddSong);
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
    else if(ui->ControlList->currentItem()->text() == "Repeat"){
        controlStatus = ControlStatus::Repeat;
    }
    else if(ui->ControlList->currentItem()->text() == "Random"){
        controlStatus = ControlStatus::Random;
    }
    else if(ui->ControlList->currentItem()->text() == "Previous"){
        controlStatus = ControlStatus::Previous;
    }
    else if(ui->ControlList->currentItem()->text() == "Next"){
        controlStatus = ControlStatus::Next;
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
        case ControlStatus::Repeat:
            if(!isRepeat){
                player->playlist()->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
                ui->ControlList->currentItem()->setIcon(QIcon(":/icon.ico"));
                isRepeat = true;
            }
            else{
                player->playlist()->setPlaybackMode(QMediaPlaylist::Loop);
                ui->ControlList->currentItem()->setIcon(QIcon(""));
                isRepeat = false;
            }
        case ControlStatus::Random:
            player->playlist()->setPlaybackMode(QMediaPlaylist::Random);
            break;
        case ControlStatus::Previous:

            player->playlist()->previous();
            break;
        case ControlStatus::Next:
            player->playlist()->next();
            if(player->playlist() == nullptr)
                player->setPlaylist(playList);
            break;
        case ControlStatus::AddSong:{
            QStringList files = QFileDialog::getOpenFileNames(this,"Select Music",QDir::homePath(),"file (*.mp3 *.wav)");{
                for(auto &file:files){
                    QFileInfo fi(file);
                    AllSong.insert(fi.fileName(),fi.filePath());
                    if(fi.fileName() != ""){
                        ui->SongList->addItem(fi.fileName());
                        playList->addMedia(QUrl(AllSong[fi.fileName()]));
                    }
                    qDebug() << AllSong[fi.fileName()];
                }
                player->setPlaylist(playList);
            }

            break;
        }

        default:
            break;
    }
}

void Widget::changeSong(QString newSong){
    player->setMedia(QUrl(newSong));
    NowSongPath = newSong;
    player->play();
    ui->ControlList->item(0)->setText("Pause");
}

inline void Widget::slotSoundSize(int size){
    player->setVolume(size);
}

void Widget::slotAutoAddSong(){
//    QDirIterator it("D:/", {"*.mp3", "*.wav"} , QDir::Files ,QDirIterator::Subdirectories );
    QDirIterator it("D:/", {"*.mp3", "*.wav"} , QDir::Files );
    while (it.hasNext()) {
        QString file = it.next();
        QFileInfo fi(file);
        AllSong.insert(fi.fileName(),fi.filePath());
        playList->addMedia(QUrl(fi.filePath()));
        qDebug() << fi.fileName() << fi.filePath();
        ui->SongList->addItem(fi.fileName());

    }
    player->setPlaylist(playList);
}
