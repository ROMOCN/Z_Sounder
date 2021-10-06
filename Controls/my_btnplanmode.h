#ifndef MY_BTNPLANMODE_H
#define MY_BTNPLANMODE_H

#include <QObject>
#include <QWidget>
#include "Controls/my_btn.h"
class My_BtnPlanMode : public My_Btn
{
    Q_OBJECT
    QPixmap map_curloop,//单曲循环
    map_sequential, //顺序播放
    map_listloop,//列表循环
    map_random; //随机播放
    QList<QPixmap> maps;
    int currentMode = 0;
public:
    My_BtnPlanMode(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
signals:
    void signal_modeChanged(int mode);
};

#endif // MY_BTNPLANMODE_H
