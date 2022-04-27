#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFileDialog>

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
};

#endif // WIDGET_H
