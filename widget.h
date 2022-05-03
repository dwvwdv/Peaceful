#ifndef WIDGET_H
#define WIDGET_H

#include <QMap>
#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>
#include <QDirIterator>
#include <QMouseEvent>
#include <QSystemTrayIcon>


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void slotControl();
    void slotControlChange();
    void changeSong();
    inline void slotSoundSize(int size);
    void slotAutoAddSong();
    void isExistFile();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::Widget *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playList;
    QSystemTrayIcon *trayIcon;

    enum class ControlStatus{
        Play,
        Pause,
        Repeat,
        Random,
        Normal,
        Previous,
        Next,
        AddSong
    };
    ControlStatus controlStatus;
    QMap<QString ,QString> AllSong;
    QMap<QString, QString>::iterator it;
    QString NowSongPath;
    bool isRepeat = false;
    bool isRandom = false;
    int preSongIndex = -1;

    QPoint      mouseStartPoint;
    QPoint      windowTopLeftPoint;
    bool m_bDrag = false;
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
};

#endif // WIDGET_H
