#ifndef MY_SLIDER_H
#define MY_SLIDER_H

#include <QObject>
#include <QSlider>
#include <QWidget>
#include <QCoreApplication>
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>

class My_Slider : public QSlider
{
    Q_OBJECT
public:
    My_Slider(QWidget *parent = nullptr);
signals:
    void signal_mouse_press();
    void signal_mouse_release();
private:
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void focusInEvent(QFocusEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private :
    QString style = "", style_hover = "";

};

#endif // MY_SLIDER_H
