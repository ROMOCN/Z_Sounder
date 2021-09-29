#ifndef FRAME_PLAYER_H
#define FRAME_PLAYER_H

#include <QDoubleSpinBox>
#include <QLabel>
#include <QObject>
#include <QWidget>
#include <thread>
#include <tool_music_qt.h>
#include <Controls/my_btn.h>
#include <Controls/my_slider.h>
#include <Tools/Tool_Qt.h>
#include <QDropEvent>
#include <QMimeData>
#include <QLCDNumber>
#include <QMediaPlaylist>
#include <QPainter>
#include <QPaintEvent>
#include "Tools/tool_entity.h"
#ifndef MUSIC_DOCK
#define MUSIC_DOCK 100
#endif
class Frame_Player : public QWidget
{
    Q_OBJECT
    Tool_Entity::Music_Entity _entity;
    QMediaPlaylist *_list = new QMediaPlaylist(this);
public:
    explicit Frame_Player(QWidget *parent = nullptr);
    ~Frame_Player()
    {
        qDebug()<<"~Frame_Player";
    }
    void Init(QString path);
    void Play();
    void Stop();
    void Next();
    void Previous();
    void set_list(QMediaPlaylist *list);
    void play_list(QMediaPlaylist* list, int musicId);
    void slot_set_len(qint64 len);
    void slot_slider_press();
    void slot_slider_release();
    void slot_state_change(qint64 state);
    void slot_rate_change(double value);

    void set_time(int time, QLabel &label);
    void slot_currentMediaChanged(QMediaContent );
signals:
    void signal_add_music(QString path);
private:
    int Music_Thread();
    void UI_Init();
    void Signal_Init();
    My_Btn *btn_play, *btn_stop, *btn_front, *btn_reverse, *btn_before, *btn_next;
    My_Slider * slider;
    std::thread *thread_MInfo;
    Tool_Music_Qt *music;
    QDoubleSpinBox *box;
    QLabel *num_current, *num_len, *label_curTitle, *label_curPerFormer;
    My_Btn *picture;
    bool press_slider = false;
    void resizeEvent(QResizeEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

};

#endif // FRAME_PLAYER_H
