#ifndef MY_TABLEWIDGET_H
#define MY_TABLEWIDGET_H

#include <QObject>
#include <QTableView>
#include <QWidget>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QStandardItemModel>
#include <QAbstractItemModel>
#include <QDebug>
#include <QStandardItem>
#include <QAction>
#include <QMenu>
#include <QMediaPlaylist>
#include "QMouseEvent"
#include "Controls/my_delegatebtn.h"
class My_Table : public QTableView
{
    Q_OBJECT
    const QString _style= "background-color:transparent;"
                          "alternate-background-color:rgb(233,195,130);"   //交替色
                          "selection-background-color:rgba(246,237,210,200);"
                          "selection-color: black;"
                          "font:20px;color:rgb(255,255,255);}";
    std::map<int , QStandardItemModel*> _models;
    std::map<int , QMediaPlaylist*> _playlists;
    My_DelegateBtn * _delegateBtn;
    int currentModelId = 0;

public:
    My_Table(QWidget *parent = nullptr);
    void add_model(int sheetId);
    void set_model(int sheetId);
    void add_List(int sheetId);
    QStandardItemModel* current_model();
    QStandardItemModel* get_model(int sheetId);
    QMediaPlaylist *get_playList(int sheetId);
    void delete_playList(int sheetId);
    void delete_model(int sheetId);
    void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;
    void itemClicked(const QModelIndex &index);
public slots:
signals:
    void signal_favorChanged(bool favor, int music_id);
protected:
    int hover_row = 0;
    void menuInit();
    void ui_init();
    void row_move_init();
    bool eventFilter(QObject *watched, QEvent *event) override;
    void mousePressEvent(QMouseEvent *event)override;
    void mouseReleaseEvent(QMouseEvent *event)override;
    void mouseMoveEvent(QMouseEvent *event)override;


};

#endif // MY_TABLEWIDGET_H
