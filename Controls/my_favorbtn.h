#ifndef MY_FAVORBTN_H
#define MY_FAVORBTN_H

#include <QObject>
#include <QWidget>
#include "my_btn.h"

class My_FavorBtn : public My_Btn
{
    Q_OBJECT
    bool favor = false;
    QPixmap pix_favor, pix_unfavor;
public:
    My_FavorBtn(QWidget *parent = nullptr);

    void mouseReleaseEvent(QMouseEvent*event);
signals:
    void signal_favor_changed(bool);

};

#endif // MY_FAVORBTN_H
