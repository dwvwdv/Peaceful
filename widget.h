#ifndef WIDGET_H
#define WIDGET_H

#include <QMap>
#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>

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
    void slotChangeSong(QString newSong);
    void slotSoundSize();
private:
    Ui::Widget *ui;
    QMediaPlayer *player;

    enum class ControlStatus{
        Play,
        Pause,
        Random,
        Previous,
        Next,
        AddSong
    };
    ControlStatus controlStatus;
    QMap<QString ,QString> AllSong;
    QMap<QString, QString>::iterator it;
};

#endif // WIDGET_H
