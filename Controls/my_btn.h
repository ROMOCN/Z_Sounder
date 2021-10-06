#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <string.h>
#include <QDebug>
#include <QMouseEvent>
#include <QEvent>
class My_Btn : public QPushButton
{
    Q_OBJECT
public:
    explicit My_Btn(QWidget *parent = nullptr);
    My_Btn(QString str,QWidget* parent);
    int MReSize(int width = 30,int length = 80);
    void set_btn_img(QPixmap pix)
    {
        this->setIcon(pix);
        this->setIconSize(pix.size());
    }
    void set_btn_img(QPushButton *control, QString path)
    {
        QPixmap pix (path);
        control->setIcon(pix);
        control->setIconSize(pix.size());
    }

    void set_btn_img(QString path)
    {
        QPixmap pix (path);
        this->setIcon(pix);
    }
    void set_img_formal(QString path)
    {
        pix_formal = QPixmap(path);
        this->setIcon(pix_formal);
        this->setIconSize(this->size());
        pix_press = pix_formal;
        pix_release = pix_formal;
        pix_hover = pix_formal;
    }
    void set_img_formal(QPixmap pix)
    {
        this->setIcon(pix);
        //this->setIconSize(this->size());
        pix_press = pix_formal;
        pix_release = pix_formal;
        pix_hover = pix_formal;
    }
    void set_img_formal(QImage path)
    {
        pix_formal = QPixmap::fromImage(path);
        this->setIcon(pix_formal);
        //this->setIconSize(this->size());
        pix_press = pix_formal;
        pix_release = pix_formal;
        pix_hover = pix_formal;
    }
    void set_img_press(QString path)
    {
        pix_press = QPixmap(path);
        pix_press.isNull();
    }
    void set_img_release(QString path)
    {
        pix_release = QPixmap(path);
    }
    void set_img_hover(QString path)
    {
        pix_hover = QPixmap(path);
    }

signals:
    void clicked();
    void signal_mouse_press();
    void signal_mouse_release();
    void signal_mouse_move();
    void signal_mouse_leave();
private:
    QPixmap pix_formal;
    QPixmap pix_press;
    QPixmap pix_release;
    QPixmap pix_hover;
    void mousePressEvent(QMouseEvent*event);
    void mouseMoveEvent(QMouseEvent*event);
    void mouseReleaseEvent(QMouseEvent*event);


    void keyPressEvent(QKeyEvent *event);
    void leaveEvent(QEvent *event);
    void clickHappen(QMouseEvent*event);
    void UIInitial();
    bool _btnpush = false;
signals:

public slots:
};

#endif // MYBUTTON_H
