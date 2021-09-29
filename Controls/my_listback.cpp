#include "my_listback.h"

#include <QPainter>

My_ListBack::My_ListBack(QWidget *parent) : QWidget(parent)
{
    Qt::WindowFlags flags = this->windowFlags();
    this->setAttribute(Qt::WA_TranslucentBackground, true);//关键
    flags = (Qt::FramelessWindowHint);
    this->setWindowFlags(flags);

}

void My_ListBack::resizeEvent(QResizeEvent *event)
{

}

void My_ListBack::paintEvent(QPaintEvent *event)
{
//    QPainter paint(this);
//    paint.setRenderHint(QPainter::Antialiasing);
//    QPainterPath path;
//    path.addRoundedRect(QRectF(2, 2, width()-4, height()-4), 20, 20);
//    paint.save();
//    QBrush brush = QBrush(QColor(222,198,162,180)); //158,137,99,80 //123,105,80,120
//    paint.setBrush(brush);
//    paint.drawPath(path);
//    paint.restore();


    QPainter paint(this);
    paint.setRenderHint(QPainter::Antialiasing);
    QPen pen = QPen(QColor(255,255,255),5,Qt::SolidLine); //211,220,213 //233,195,130
    QPainterPath path;
    path.addRoundedRect(QRectF(pen.width(), pen.width(), width() - pen.width()*2, height() - pen.width()*2), 20, 20);
    paint.save();
    QBrush brush = QBrush(QColor(222,198,162,180)); //158,137,99,80 //123,105,80,120
    paint.setBrush(brush);
    paint.drawPath(path);
    paint.restore();

    paint.save();
    paint.setPen(pen);
    paint.drawPath(path);
    paint.restore();

    paint.save();
    paint.setPen(pen);
    paint.drawLine(pen.width(), _height,this->width() - pen.width(), _height);
    paint.restore();


}
