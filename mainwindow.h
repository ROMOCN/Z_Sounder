#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMouseEvent>
#include <QWidget>

#include <QPushButton>
#include <QSlider>
#include <thread>
#include <tool_music.h>
#include "tool_music_qt.h"
#include "Controls/my_slider.h"
#include "Controls/my_btn.h"
#include <QDoubleSpinBox>
#include "Tools/Tool_MoveWidget.h"
#include "Tools/Tool_Berth.h"
#include "Tools/Tool_SounderSql.h"
#include "Frame/frame_player.h"
#include "Frame/frame_list.h"
#include "Tools/tool_entity.h"
#include "Controls/my_messagebox.h"
#include "Controls/my_btn.h"
#ifndef MUSIC_DOCK
#define MUSIC_DOCK 100
#endif
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT
    Tool_SounderSql sql;
    std::thread *thread_data;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool isclose = false;

public slots:
    void play(QString path);
    void slot_music_add(int sheet,  int musicid,int favorFrom, bool favor, QString title, QString performer, QString duration, QString album,QString len, QString path);
    void slot_music_delete(int sheetId,int musicId,QString path);
    void slot_sheet_add(int id, QString name);
    void slot_sheet_delete(int id, QString name);
    void slot_music_favorChanged(bool,int, int, QString);
    void slot_list_set(QMediaPlaylist *);
    void slot_list_play(QMediaPlaylist *, int musicId);

private:
    Ui::MainWindow *ui;
    Tool_MoveWidget *_move;
    Tool_Berth *_berth;
    Frame_Player *_player;
    Frame_List *_list;
    My_Btn *btn_close;
    void read_musicInfoFromDB();
    void dbInit();
    void  thread_readData();
    void db_addDefaultSheet();
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

};
#endif // MAINWINDOW_H
