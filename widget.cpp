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

    playList->addMedia(QUrl("qrc:/one.mp3"));
    player->setMedia(playList);


    connect(ui->ControlList,QListWidget::doubleClicked,this,Widget::slotControlChange);

    connect(ui->SongList,QListWidget::doubleClicked,this,[&]{
        qDebug() << AllSong[ui->SongList->currentItem()->text()];
        changeSong(AllSong[ui->SongList->currentItem()->text()]);
    });

    connect(ui->SoundSize,QSlider::valueChanged,this,Widget::slotSoundSize);

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
                playList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
                ui->ControlList->currentItem()->setIcon(QIcon(":/icon.ico"));
                isRepeat = true;
            }
            else{
                playList->setPlaybackMode(QMediaPlaylist::Sequential);
                ui->ControlList->currentItem()->setIcon(QIcon(""));
                isRepeat = false;
            }
        case ControlStatus::Previous:
            for(it = AllSong.begin(); it != AllSong.end();++it){

                if(it.value() == NowSongPath){
                    qDebug() << it.key() << " " << it.value() << '\n';
                    if(it == AllSong.begin()){
                        it = AllSong.end()-1;
                        changeSong(it.value());
                    }
                    else
                        changeSong((--it).value());
                    player->play();
                    break;
                }
            }
            break;
        case ControlStatus::Next:
            for(it = AllSong.begin(); it != AllSong.end();++it){

                if(it.value() == NowSongPath){
                    qDebug() << it.key() << " " << it.value() << '\n';
                    if(it + 1 == AllSong.end()){
                        it = AllSong.begin();
                        changeSong(it.value());
                    }
                    else
                        changeSong((++it).value());
                    player->playlist();
                    break;
                }
            }
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
//                player->setPlaylist(playList);
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
        playList->addMedia(QUrl(AllSong[fi.fileName()]));
        qDebug() << fi.fileName() << fi.filePath();
        ui->SongList->addItem(fi.fileName());

    }
    player->setPlaylist(playList);
}
