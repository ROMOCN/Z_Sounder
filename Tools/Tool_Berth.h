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
        if(widget_state == 1 &&father->size() == minsize)
        {
            father->setVisible(false);
            widget_state = 2;
        }else if(widget_state == 1 &&father->size() != minsize)
        {
            widget_state = 0;
            father->setVisible(true);

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
    int widget_state = 0;  //0：不停靠； 1：窗口停靠在边缘； 2：窗口在屏幕外侧
    QSize minsize = QSize(680, 120);

    int Thread_Graphic()
    {

        while( isclose == false )
        {
            if(!press_mouse &&father->size() == minsize)
            {

                {
                    QPoint pos_mouse = QCursor::pos();//鼠标坐标
                    QPoint pos_widget = this->parentWidget()->pos();//窗口坐标
                    //QPoint pos_widget = father->pos();
                    QScreen *screen = QGuiApplication::primaryScreen ();//屏幕大小
                    QRect mm=screen->availableGeometry() ;
                    int screen_width = mm.width();
                    //int screen_height = mm.height();
                    if(pos_widget.y() <=0)
                    {
                        if(father->isVisible())
                        {
                            father->move(  (screen_width - father->width())/2 ,0);
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
                    //鼠标贴近边缘显示窗口
                    if(pos_mouse.y() == 0 && widget_state == 2)
                    {
                        father->setVisible(true);
                        widget_state = 1;
                    }
                }
            }

//            if(father->size() != minsize && father->isVisible() == false)
//            {
//                father->setVisible(true);
//            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        return 0;
    }


};

#endif // TOOL_BERTH_H
