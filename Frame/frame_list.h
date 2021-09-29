#ifndef FRAME_LIST_H
#define FRAME_LIST_H

#include <QObject>
#include <QWidget>
#include <QMediaPlaylist>
#include <Controls/my_listback.h>
#include <QListWidgetItem>
#include "Controls/my_btn.h"
#include "Controls/my_listwidget.h"
#include "Controls/my_table.h"
#include "Controls/my_favorbtn.h"
#include "Tools/tool_entity.h"
#include "Controls/my_delegatebtn.h"
#include "Controls/my_messagebox.h"



class Frame_List : public QWidget
{
    Q_OBJECT
    QMediaPlaylist *_list;
    My_DelegateBtn * _dbtn;
    QMenu *menu_music;
    QMenu *menu_sheet;

public:
    int currentSheet()
    {
        int sheet_id = ((My_Item*) list_sheet->item(list_sheet->currentRow()))->Sheet_Id;
        return sheet_id;
        //return list_sheet->currentRow();
    }
    explicit Frame_List(QWidget *parent = nullptr);
    void add_sheet();
    void add_sheet(QString name, QString iconPath ,int sheetId = 0);
    void add_sheet(QString name,int sheetId = 0);
    //void add_music(QString path);
    bool add_music(int sheet_id, int favorFrom ,bool favor, QString title, QString performer, QString album, QString duration, QString path);
    bool add_music(int sheet_id, int favorFrom , bool favor,QString path);
    void add_LotsMusic(QStringList path);
    void add_LotsMusic(QList<QUrl> path);
    void add_musicInList(int sheet, QString path);
    void delete_music(int sheet_id, int music_id, QString path);
    void delete_sheet(int sheetId);
    void delete_musicFromFavor(int sheet_id, int music_id, QString path);
    void update_sheet(int sheetId);
    void test_add_music();
    void slot_add_music(QString path);
    void slot_readyToPlay(QModelIndex);
    void slot_readtToDelete(QModelIndex row);
    void slot_music_favorChanged(bool, int music_id);
    void mousePressEvent(QMouseEvent *event)override;
    void mouseReleaseEvent(QMouseEvent *event)override;
    void mouseMoveEvent(QMouseEvent *event)override;

signals:
     //signal about sheet
     void signal_sheet_add(int id, QString name);
     void signal_sheet_delete(int id, QString name);
     void signal_sheet_move(int source, int dest);
     //signal about music
     void signal_music_add(int sheet, int musicid, int favorFrom ,bool favor, QString title, QString performer, QString duration, QString album,QString len, QString path);
     void signal_music_play(QString path);
     void signal_music_delete(int sheetId,int musicId,QString path);
     void signal_music_musiclist();
     void signal_music_favorChanged(bool favor,int sheet, int music,QString path);
     void signal_list_set(QMediaPlaylist *);
     void signal_list_play(QMediaPlaylist *, int musicId);

public slots:
    //void slot_playing_sheet(int sheet, int music);
     void slot_ListSheetChanged(int row);
     void slot_ListMusicChanged(int row);
private:

        My_ListWidget  *list_sheet; //歌单
        My_Table *list_music;
        My_ListBack *back_music, *back_sheet;
        My_Btn *btn_add_sheet, *btn_add_music;

        void UI_Init();
        void Menu_Init();
        void paintEvent(QPaintEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;
        void mouseDoubleClickEvent(QMouseEvent *event) override;
        QString stringHandle(QString str);
};

#endif // FRAME_LIST_H
