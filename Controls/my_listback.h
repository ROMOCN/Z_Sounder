#ifndef MY_LISTBACK_H
#define MY_LISTBACK_H

#include <QObject>
#include <QWidget>
class My_ListBack : public QWidget
{
    Q_OBJECT
public:
    explicit My_ListBack(QWidget *parent = nullptr);
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void set_height(int h)
    {
        _height = h;
    }
signals:
private:
    int _height = 80;

};

#endif // MY_LISTBACK_H
