#include "my_slider.h"

My_Slider::My_Slider(QWidget *parent)
    : QSlider(parent)
{
    this->installEventFilter(this);
    this->setAttribute(Qt::WA_Hover,true);//开启悬停事件
    QFile styleSheet;
        styleSheet.setFileName(":/Style/style.css");
        if(styleSheet.open(QFile::ReadOnly)) {
            style = styleSheet.readAll();
            styleSheet.close();
            this->setStyleSheet(style);
        }
        styleSheet.setFileName(":/Style/style_hover.css");
        if(styleSheet.open(QFile::ReadOnly)) {
            style_hover = styleSheet.readAll();
            styleSheet.close();
        }
}

void My_Slider::mousePressEvent(QMouseEvent *ev)
{
    int x = ev->pos().x();
    int y = ev->pos().y();
    emit signal_mouse_press();

    this->setStyleSheet(style_hover);
    if (ev->button() == Qt::LeftButton)
    {
       int dur = this->maximum() - this->minimum();
       int pos = this->minimum() + dur * ((double)ev->x() / this->width());
       if(pos != this->sliderPosition())
        {
          this->setValue(pos);
        }
    }

    //注意应先调用父类的鼠标点击处理事件，这样可以不影响拖动的情况
//    QSlider::mousePressEvent(ev);
    //获取鼠标的位置，这里并不能直接从ev中取值（因为如果是拖动的话，鼠标开始点击的位置没有意义了）
    //方法一
//    int xx = ev->pos().x();
//    double pos = ev->pos().x() / (double)width();
//    setValue(pos * (maximum() - minimum()) + minimum());
    //方法二
//    double pos = ev->pos().x() / ( (double)width()/this->maximum() );
//    setValue(int(pos));

    //向父窗口发送自定义事件event type，这样就可以在父窗口中捕获这个事件进行处理
//    QEvent evEvent(static_cast<QEvent::Type>(QEvent::User + 1));
//    QCoreApplication::sendEvent(parentWidget(), &evEvent);

}

void My_Slider::mouseReleaseEvent(QMouseEvent *ev)
{
    emit signal_mouse_release();
    this->setStyleSheet(style_hover);
}

void My_Slider::mouseMoveEvent(QMouseEvent *ev)
{
    this->setFocus();
    int xx = ev->pos().x();
    double pos = ev->pos().x() / ( (double)width()/this->maximum() );
    setValue(int(pos));
    this->setStyleSheet(style_hover);
}

void My_Slider::focusInEvent(QFocusEvent *event)
{
    qDebug()<<"focus";
}
bool My_Slider::eventFilter(QObject *obj, QEvent *event)
{
    if(obj==this)
    {
//        if (event->type()==QEvent::MouseButtonPress)
//        {
//            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
//            if (mouseEvent->button() == Qt::LeftButton)
//            {
//               int dur = this->maximum() - this->minimum();
//               int pos = this->minimum() + dur * ((double)mouseEvent->x() / this->width());
//               if(pos != this->sliderPosition())
//                {
//                  this->setValue(pos);
//                }
//            }
//        }

        if(event->type() == QEvent::HoverEnter)
        {
              this->setStyleSheet(style_hover);
        }

        if(event->type() == QEvent::Leave)
        {
              this->setStyleSheet(style);
        }
    }
    return QObject::eventFilter(obj,event);
}

