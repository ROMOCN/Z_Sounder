#include "my_btn.h"

My_Btn::My_Btn(QWidget *parent) : QPushButton(parent)
{
    this->setStyleSheet("border:none");
    UIInitial();
}

My_Btn::My_Btn(QString str,QWidget *parent):QPushButton(str,parent)
{
    qDebug()<<"initial/n";
    UIInitial();
}

void My_Btn::UIInitial()
{
    QPixmap chatBtnPix(tr(":/img/writer.png"));
    QPalette p;
    p.setBrush(QPalette::Background,QBrush(chatBtnPix));
    this->setPalette(p);
}

int My_Btn::MReSize(int width,int length)
{
    qDebug()<<"resize/n";
    this->resize(width,length);
    return 1;
}


void My_Btn::mouseMoveEvent(QMouseEvent *event)
{
    emit signal_mouse_move();
    event->ignore();
    this->setIcon(pix_hover);
    qDebug()<<"进入";
}
void My_Btn::mousePressEvent(QMouseEvent *event)
{
    emit signal_mouse_press();
    event->ignore();
    _btnpush = true;
    this->setIcon(pix_press);
}
void My_Btn::mouseReleaseEvent(QMouseEvent *event)
{
    emit signal_mouse_release();
    event->ignore();

    QPixmap pix(tr(":/img/writer.png"));
    this->setIcon(pix_release);
    clickHappen(event);
}

void My_Btn::keyPressEvent(QKeyEvent *event)
{

}

void My_Btn::leaveEvent(QEvent *event)
{
    emit signal_mouse_leave();
    if(_btnpush)
        _btnpush = false;
}

//点击信号重写
void My_Btn::clickHappen(QMouseEvent *event)
{
    if(_btnpush && event->button() == Qt::LeftButton)
    {
        emit clicked();
        _btnpush = false;
    }
}
