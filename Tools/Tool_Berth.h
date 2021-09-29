#ifndef TOOL_BERTH_H
#define TOOL_BERTH_H
#include <QGuiApplication>
#include <QObject>
#include <QScreen>
#include <thread>
#include <QWidget>
#include <QMouseEvent>
#include <QDebug>
class Tool_Berth :public QWidget
{
public:
    Tool_Berth(QWidget *parent = nullptr): QWidget(parent)
    {
        isclose = false;
        this->setParent(parent);
        father = parent;
        thread_Graphic =new std::thread(&Tool_Berth::Thread_Graphic,this);
        thread_Graphic->detach();
    }
    Tool_Berth(bool &close, QWidget *parent): QWidget(parent)
    {
        isclose = close;
        this->setParent(parent);
        father = parent;
        thread_Graphic =new std::thread(&Tool_Berth::Thread_Graphic,this);
        thread_Graphic->detach();
    }
    ~Tool_Berth()
    {
        isclose = true;
    }
    void mousePressEvent(QMouseEvent *event)
    {
        press_mouse = true;
    }

    void mouseReleaseEvent(QMouseEvent *event)
    {
        press_mouse = false;
    }

    void mouseMoveEvent(QMouseEvent *event)
    {
    }

    void leaveEvent(QEvent *event)
    {
        press_mouse = false;
        if(widget_state == 1)
        {
            father->setVisible(false);
            widget_state = 2;
        }
    }
    void Close()
    {
         isclose = true;
    }
private:
    bool  isclose = false;
    QWidget *father;
    std::thread *thread_Graphic;
    bool press_mouse = false;
    int widget_state = 0;  //0：不停靠； 1：在边缘； 2：在外侧
    bool start_graphic = false;
    int Thread_Graphic()
    {
        while( isclose == false )
        {
            //qDebug()<<"berth running";

            if(!press_mouse && father->size() != father->maximumSize())
            {

                {
                    QPoint pos_mouse = QCursor::pos();
                    QPoint pos_widget = this->parentWidget()->pos();
                    //QPoint pos_widget = father->pos();
                    //qDebug()<<"father:"<< pos_widget;
                    QScreen *screen = QGuiApplication::primaryScreen ();
                    QRect mm=screen->availableGeometry() ;
                    int screen_width = mm.width();
                    //int screen_height = mm.height();
                    if(pos_widget.y() <=0)
                    {
                        if(father->isVisible())
                        {
                            father->move(  (screen_width - father->width())/2 ,0);
                            start_graphic = true;
                            widget_state = 1;
                        }
                        else
                        {

                        }
                    }
                    else
                    {
                        widget_state = 0;
                    }

                    if(pos_mouse.y() == 0 && widget_state == 2)
                    {
                        father->setVisible(true);
                        widget_state = 1;
                    }
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        return 0;
    }


};

#endif // TOOL_BERTH_H
