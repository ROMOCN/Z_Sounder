#ifndef TOOL_MOVEWIDGET_H
#define TOOL_MOVEWIDGET_H


#include <QObject>
#include <QWidget>
#include <QMouseEvent>

#define Min(a,b) ((a)<(b)? (a) :(b))
#define Max(a,b) ((a)>(b)? (a) :(b))

class Tool_MoveWidget
{
public:
    Tool_MoveWidget(QWidget* _widget)
    {
        widget = _widget;
        edgeMargin = 8;        //设置检测边缘为8
        resizeDir = nodir;   //初始化检测方向为无
        widget->setWindowFlags(Qt::FramelessWindowHint);  //设置无边框
        widget->setMouseTracking(true); //开启鼠标追踪
    }
    void mousePressEvent(QMouseEvent*event)
    {
        event->ignore();
        if (event->button() == Qt::LeftButton)  //每当按下鼠标左键就记录一下位置
         {
            dragPosition = event->globalPos() - widget->frameGeometry().topLeft();  //获得鼠标按键位置相对窗口左上面的位置
         }
    }
    void leaveEvent(QEvent *event)
    {
        resizeDir = nodir;   //初始化检测方向为无
        QCursor tempCursor = widget->cursor();
        tempCursor.setShape(Qt::ArrowCursor);

    }
    void mouseMoveEvent(QMouseEvent*event)
    {
        event->ignore();
        if (event->buttons() & Qt::LeftButton){                 //如果左键是按下的
          if(resizeDir == nodir){                             //如果鼠标不是放在边缘那么说明这是在拖动窗口
             widget->move(event->globalPos() - dragPosition);
          }
          else{
              int ptop,pbottom,pleft,pright;                   //窗口上下左右的值
              ptop = widget->frameGeometry().top();
              pbottom = widget->frameGeometry().bottom();
              pleft = widget->frameGeometry().left();
              pright = widget->frameGeometry().right();
              if(resizeDir & top){                               //检测更改尺寸方向中包含的上下左右分量
                  if(widget->height() == widget->minimumHeight()){
                      ptop = Min(event->globalY(),ptop);
                  }
                  else if(widget->height() == widget->maximumHeight()){
                      ptop = Max(event->globalY(),ptop);
                  }
                  else{
                      ptop = event->globalY();
                  }
              }
              else if(resizeDir & bottom){
                  if(widget->height() == widget->minimumHeight()){
                      pbottom = Max(event->globalY(),ptop);
                  }
                  else if(widget->height() == widget->maximumHeight()){
                      pbottom = Min(event->globalY(),ptop);
                  }
                  else{
                      pbottom = event->globalY();
                  }
              }

              if(resizeDir & left){                        //检测左右分量
                  if(widget->width() == widget->minimumWidth()){
                      pleft = Min(event->globalX(),pleft);
                  }
                  else if(widget->width() == widget->maximumWidth()){
                      pleft = Max(event->globalX(),pleft);
                  }
                  else{
                      pleft = event->globalX();
                  }
              }
              else if(resizeDir & right){
                  if(widget->width() == widget->minimumWidth()){
                      pright = Max(event->globalX(),pright);
                  }
                  else if(widget->width() == widget->maximumWidth()){
                      pright = Min(event->globalX(),pright);
                  }
                  else{
                      pright = event->globalX();
                  }
              }
              widget->setGeometry(QRect(QPoint(pleft,ptop),QPoint(pright,pbottom)));
          }
      }
      else testEdge();   //当不拖动窗口、不改变窗口大小尺寸的时候  检测鼠标边缘


    }
    void mouseReleaseEvent(QMouseEvent*event)
    {
        event->ignore();
        if(resizeDir != nodir)
        {         //还原鼠标样式
            testEdge();
        }
        resizeDir = nodir;   //初始化检测方向为无
        QCursor tempCursor = widget->cursor();
        tempCursor.setShape(Qt::ArrowCursor);

    }
private:
    QPoint dragPosition;   //鼠标拖动的位置
    int    edgeMargin;     //鼠标检测的边缘距离
    QWidget *widget;
    enum
    {
        nodir,
          top = 0x01,
          bottom = 0x02,
          left = 0x04,
          right = 0x08,
          topLeft = 0x01 | 0x04,
          topRight = 0x01 | 0x08,
          bottomLeft = 0x02 | 0x04,
          bottomRight = 0x02 | 0x08
    } resizeDir; //更改尺寸的方向
    void testEdge()  //检测鼠标是否接近窗口边缘
    {
        int diffLeft = abs(widget->cursor().pos().x() - widget->frameGeometry().left());      //计算鼠标距离窗口上下左右有多少距离
        int diffRight = abs(widget->cursor().pos().x() - widget->frameGeometry().right());
        int diffTop = abs(widget->cursor().pos().y() - widget->frameGeometry().top());
        int diffBottom = abs(widget->cursor().pos().y() - widget->frameGeometry().bottom());

        QCursor tempCursor;                                    //获得当前鼠标样式，注意:只能获得当前鼠标样式然后再重新设置鼠标样式
        tempCursor = widget->cursor();                                 //因为获得的不是鼠标指针，所以不能这样用:cursor().setXXXXX

        if(diffTop < edgeMargin)
        {                              //根据 边缘距离 分类改变尺寸的方向
            if(diffLeft < edgeMargin){
                resizeDir = topLeft;
                tempCursor.setShape(Qt::SizeFDiagCursor);
            }
            else if(diffRight < edgeMargin){
                resizeDir = topRight;
                tempCursor.setShape(Qt::SizeBDiagCursor);
            }
            else{
                resizeDir = top;
                tempCursor.setShape(Qt::SizeVerCursor);
            }
        }
        else if(diffBottom < edgeMargin){
            if(diffLeft < edgeMargin){
                resizeDir = bottomLeft;
                tempCursor.setShape(Qt::SizeBDiagCursor);
            }
            else if(diffRight < edgeMargin){
                resizeDir = bottomRight;
                tempCursor.setShape(Qt::SizeFDiagCursor);
            }
            else{
                resizeDir = bottom;
                tempCursor.setShape(Qt::SizeVerCursor);
            }
        }
        else if(diffLeft < edgeMargin){
            resizeDir = left;
            tempCursor.setShape(Qt::SizeHorCursor);
        }
        else if(diffRight < edgeMargin){
            resizeDir = right;
            tempCursor.setShape(Qt::SizeHorCursor);
        }
        else{
            resizeDir = nodir;
            tempCursor.setShape(Qt::ArrowCursor);
        }

        widget->setCursor(tempCursor);                    //重新设置鼠标,主要是改样式
    }

protected:

};


#endif // TOOL_MOVEWIDGET_H
