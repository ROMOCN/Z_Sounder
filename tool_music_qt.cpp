#include "tool_music_qt.h"

Tool_Music_Qt::Tool_Music_Qt(QObject *parent) : QObject(parent)
{
    player = new QMediaPlayer;
    this->connect(player,&QMediaPlayer::stateChanged,this, &Tool_Music_Qt::Music_State_Changed);
    this->connect(player, &QMediaPlayer::durationChanged, this, &Tool_Music_Qt::Music_Signal_Length);
    this->connect(player, &QMediaPlayer::positionChanged, this, &Tool_Music_Qt::Music_Position_Changed);
    this->connect(player, &QMediaPlayer::stateChanged, this, &Tool_Music_Qt::Signal_State_Changed);
//    this->connect(player, &QMediaPlayer::mediaStatus, this, &Tool_Music_Qt::Signal_State_Changed);
    connect(player, &QMediaPlayer::metaDataAvailableChanged,this, &Tool_Music_Qt::Slot_MetaDataAvailableChanged);
    connect(player, &QMediaPlayer::mediaChanged,this, &Tool_Music_Qt::Signal_MediaChanged);
    connect(player, &QMediaPlayer::mediaChanged,this, &Tool_Music_Qt::Music_Get_Info);

}

void Tool_Music_Qt::Music_SetList(QMediaPlaylist *list)
{
    player->setPlaylist(list);

}

void Tool_Music_Qt::Slot_MetaDataAvailableChanged(bool available)
{
    if(available){
        foreach(QString str,player->availableMetaData()){
            qDebug()<<str<<"   :"<< player->metaData(str).toString().toUtf8().data();
        }
        //playlist->setCurrentIndex(++count);
    }
}

void Tool_Music_Qt::Music_Inital(QString path)
{
    //player->setMedia(QUrl(path));
    status = 0;
    //player->setMedia(QMediaContent(QUrl(path)));

}

void Tool_Music_Qt::Music_Play()
{
    player->play();
}

void Tool_Music_Qt::Music_Play_From(long position)
{
}

void Tool_Music_Qt::Music_Stop()
{
    player->stop();
}

void Tool_Music_Qt::Music_Pause()
{
    player->pause();
}

void Tool_Music_Qt::Music_Step_Front()
{

}

void Tool_Music_Qt::Music_Step_Reverse()
{

}

int Tool_Music_Qt::Music_Get_Length()
{
    return player->duration();
}

int Tool_Music_Qt::Music_Get_Current_Position()
{
    return player->position();

}

int Tool_Music_Qt::Music_Get_Status()
{
    int status = player->state();
//    QMediaPlayer::State::StoppedState = 0;
//    QMediaPlayer::State::PlayingState = 1;
//    QMediaPlayer::State::PausedState = 2;
    return status;

}

int Tool_Music_Qt::Music_Get_Media_Status()
{
    return player->mediaStatus();
//    QMediaPlayer::MediaStatus
//    UnknownMediaStatus	0	未知状态
//    Nomedia	1	无媒体文件，player处于StoppedState
//    LoadingMedia	2	媒体文件加载中，player可以处于任何状态
//    LoadedMedia	3	媒体文件已加载，player处于StoppedState
//    StalledMedia	4	媒体处于延迟或者暂时的中断状态，player处于PlayingState或者PauseState
//    BufferingMedia	5	媒体正在缓冲数据，player处于PlayingState或者PausedState
//    BufferedMedia	6	媒体缓冲数据完成，player处于PlayingState或者PausedState
//    EndOfMedia	7	媒体结束，player处于StoppedState
    //    InvalidMedia	8	非法的媒体文件，player处于StoppedState
}

void Tool_Music_Qt::Music_Set_Voice(int value)
{
    player->setVolume(value);
}

void Tool_Music_Qt::Music_Set_Position(int value)
{
    player->setPosition(value);
}


int Tool_Music_Qt::Music_Get_Rate()
{
    return player->playbackRate();
}

void Tool_Music_Qt::Music_Set_Rate(double rate)
{
    player->setPlaybackRate(rate);
}

void Tool_Music_Qt::Music_Get_Info()
{
    QVariant title = player->metaData(QString("Title"));
    QVariant performer = player->metaData(QString("Performer"));
   QString title_s;
   if(title.canConvert(QVariant::Type::String))
   {
       title_s = title.toString();
       qDebug()<<title_s;

   }
    //player->metaData(QString("ContributingArtist"));
}

void Tool_Music_Qt::Music_State_Changed()
{
    this->status = player->state();
}

void Tool_Music_Qt::Music_Position_Changed(qint64 po)
{
    qDebug()<<"changed";
}
