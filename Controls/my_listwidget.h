#ifndef MY_LISTWIDGET_H
#define MY_LISTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QListWidget>
#include <QMenu>
#include <QAction>
#include <QFile>
#include <QMouseEvent>
#include <QDateTime>
class My_Item :public QListWidgetItem
{
public:
    My_Item(QIcon icon, QString str):QListWidgetItem(icon,str)
    {
        setSheetId();
    }
    void setSheetId()
    {
        Sheet_Id = QDateTime::currentDateTime().toTime_t();
    }
    int Sheet_Id = 0;

};
class My_ListWidget : public QListWidget
{
    Q_OBJECT
public:
    My_ListWidget(QWidget *parent = nullptr);
    My_ListWidget(My_ListWidget *list, QWidget *parent = nullptr);
    void Init(My_ListWidget *list);
    void slot_itemEntered(QListWidgetItem *item);

public slots:
    void slot_itemClick(QListWidgetItem *item);

signals:
    void signal_selectionChanged(int row);
private:
    void menuInit();


};

#endif // MY_LISTWIDGET_H
