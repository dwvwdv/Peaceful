#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/musicIco.ico"));       //Icon設定
    this->setAttribute(Qt::WA_TranslucentBackground);//背景透明化
    this->setWindowFlags(Qt::FramelessWindowHint);   //無邊窗口
//    this->setFixedSize(300,270);                           //固定窗口大小
    this->setWindowTitle("Peaceful");	 //視窗名稱設定
    qApp->installEventFilter(this);

    player = new QMediaPlayer;
    playList = new QMediaPlaylist;
    playList->setPlaybackMode(QMediaPlaylist::Loop);

    //debug btn
    ui->listBtn->hide();
    connect(ui->listBtn,QPushButton::clicked,this,[=]{
        qDebug() << playList->mediaCount();
    });

    //Control
    connect(ui->ControlList,QListWidget::doubleClicked,this,Widget::slotControlChange);

    //SongList
    connect(ui->SongList,QListWidget::doubleClicked,this,[&]{
        changeSong();
    });

    //Now song icon
    connect(player,QMediaPlayer::currentMediaChanged,ui->SongList,[&]{
        if(preSongIndex != -1)
            ui->SongList->item(preSongIndex)->setIcon(QIcon(""));
        ui->SongList->item(playList->currentIndex())->setIcon(QIcon(":/musicIco.ico"));
        preSongIndex = playList->currentIndex();

        qDebug() << playList->currentIndex();
    });
    //Sound size
    connect(ui->SoundSize,QSlider::valueChanged,this,Widget::slotSoundSize);

    //Scan Music
    connect(ui->Auto_Add,QPushButton::clicked,this,Widget::slotAutoAddSong);

     trayIcon = new QSystemTrayIcon(this);
     trayIcon->setIcon(QIcon(":/musicIco.ico"));
     trayIcon->show();
     //右下角圖標事件連結
     connect(trayIcon,&QSystemTrayIcon::activated,this,Widget::iconActivated);

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
        controlStatus = ControlStatus::Random;
        ui->ControlList->currentItem()->setText("Random");
    }
    else if(ui->ControlList->currentItem()->text() == "Random"){
        controlStatus = ControlStatus::Normal;
        ui->ControlList->currentItem()->setText("Normal");
    }
    else if(ui->ControlList->currentItem()->text() == "Normal"){
        controlStatus = ControlStatus::Repeat;
        ui->ControlList->currentItem()->setText("Repeat");
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
            player->playlist()->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
            ui->ControlList->currentItem()->setIcon(QIcon(":/icon.ico"));
            isRepeat = true;
            break;
        case ControlStatus::Random:
            player->playlist()->setPlaybackMode(QMediaPlaylist::Random);
            ui->ControlList->currentItem()->setIcon(QIcon(":/musicIco.ico"));
            isRepeat = false;
            break;
        case ControlStatus::Normal:
                player->playlist()->setPlaybackMode(QMediaPlaylist::Loop);
                ui->ControlList->currentItem()->setIcon(QIcon(""));
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
                if(files.isEmpty())
                    return;
                for(auto &file:files){
                    QFileInfo fi(file);
                    AllSong.insert(fi.fileName(),fi.filePath());
                    if(fi.fileName() != ""){
                        ui->SongList->addItem(fi.fileName());
                        playList->addMedia(QUrl(AllSong[fi.fileName()]));
                    }
                }
                player->setPlaylist(playList);
            }

            break;
        }

        default:
            break;
    }
}

void Widget::changeSong(){
//    player->setMedia(QUrl(newSong));
//    NowSongPath = newSong;
    //Solution Click Repeat ,after Double Click SongList Bug
    if(isRepeat){
        player->playlist()->setPlaybackMode(QMediaPlaylist::Loop);
    }

    while(player->playlist()->currentIndex() != ui->SongList->row(ui->SongList->currentItem())){
        player->playlist()->next();
    }
    player->play();
    ui->ControlList->item(0)->setText("Pause");

    if(isRepeat){
        player->playlist()->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    }
}

inline void Widget::slotSoundSize(int size){
    player->setVolume(size);
}

void Widget::slotAutoAddSong(){
    QDirIterator it("D:/", {"*.mp3", "*.wav"} , QDir::Files ,QDirIterator::Subdirectories );
//    QDirIterator it("D:/", {"*.mp3", "*.wav"} , QDir::Files );
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

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bDrag = true;
        mouseStartPoint = event->globalPos();
        windowTopLeftPoint = this->frameGeometry().topLeft();
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bDrag)
    {
        QPoint distance = event->globalPos() - mouseStartPoint;
        this->move(windowTopLeftPoint + distance);
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bDrag = false;
    }
    else if(event->button() == Qt::RightButton){
        QApplication::exit();
    }
}

void Widget::iconActivated(QSystemTrayIcon::ActivationReason reason){
    switch(reason){
    case QSystemTrayIcon::DoubleClick :
        this->setWindowState(Qt::WindowActive);
        this->activateWindow();
        this->show();
        qDebug() << "get icon double click!" << endl;
        break;
    default:
        break;
    }
}


bool Widget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
//           qDebug() << "key " << keyEvent->key() << "from" << obj;
        if(keyEvent->key() == Qt::Key_Escape)
            this->hide();
    }
    return QObject::eventFilter(obj, event);
}
