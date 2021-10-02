#ifndef TOOL_MUSIC_QT_H
#define TOOL_MUSIC_QT_H

#include <QObject>
#include <QMediaPlayer>
#include <QMediaContent>
class Tool_Music_Qt : public QObject
{
    Q_OBJECT
    QMediaPlayer *player;

public:
    explicit Tool_Music_Qt(QObject *parent = nullptr);
    ~Tool_Music_Qt()
    {
        this->status = -1;
        qDebug()<<"~Tool_Music_Qt";
    }
    QMediaContent Music_Data()
    {
      return player->media();
    };
    void Music_SetList(QMediaPlaylist *list);
    void Music_Inital(QString path);
    void Music_Play();
    void Music_Play_From(long position);
    void Music_Stop();
    void Music_Pause();
    void Music_Step_Front();
    void Music_Step_Reverse();
    int Music_Get_Current_Position();
    int Music_Get_Status();
    int Music_Get_Media_Status();
    void Music_Set_Voice(int value);//音量
    void Music_Set_Position(int value);
    int Music_Get_Rate();//播放速度
    void Music_Set_Rate(double rate);//播放速度
    void Music_Get_Info();
    void Error_Msg(int errorMsg);
    int status = -1;
signals:
    void Music_Signal_Length(qint64 len);
    void Signal_State_Changed(qint64 state);
    void Signal_MediaChanged(QMediaContent);
private:
    int length = 0; //current_music_len
    void Music_State_Changed();
    void Music_Position_Changed(qint64 po);

    int Music_Get_Length(); //音乐长度通过信号推送， 此函数不用

    void Slot_MetaDataAvailableChanged(bool available);

};



#endif // TOOL_MUSIC_QT_H
