#ifndef TOOL_MOVE_H
#define TOOL_MOVE_H
#include <QObject>
#include <QWidget>
#include <QMouseEvent>


class Tool_Move :public QObject
{
    bool _pressed = false;
    QPoint _prePoint;//按下鼠标时的位置
    QWidget *_parent = nullptr;
public:
    Tool_Move(QWidget *parent = nullptr):QObject(parent)
    {
        this->setParent(parent);
        _parent = parent;
    }
    void mouseMoveEvent(QMouseEvent*event)
    {
        if (event->buttons() == Qt::LeftButton)
        {
            if(_pressed)
            {
                if(_parent->windowState() == Qt::WindowState::WindowMaximized)
                    _parent->showNormal();
                _parent->move(event->globalPos() - _prePoint);
            }
        }
    }
    void mousePressEvent(QMouseEvent*event)
    {
//        if (event->button() == Qt::LeftButton && _parent->windowState() != Qt::WindowState::WindowMaximized)  //每当按下鼠标左键就记录一下位置
        if (event->button() == Qt::LeftButton)  //每当按下鼠标左键就记录一下位置
        {
            _pressed = true;
            //获得鼠标按键位置相对窗口左上面的位置
            _prePoint = event->globalPos() - _parent->frameGeometry().topLeft();
        }
    }
    void mouseReleaseEvent(QMouseEvent*event)
    {
        _pressed = false;
    }

    void leaveEvent(QEvent *event)
    {
        _pressed = false;
    }


};

#endif // TOOL_MOVE_H
