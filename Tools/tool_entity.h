#ifndef TOOL_ENTITY_H
#define TOOL_ENTITY_H

#include <QString>
#include "include/MediaInfoDLL.h"
#include <QFileInfo>
#include <QDebug>
#include <QTime>
namespace  Tool_Entity
{
    class Music_Info
    {
    public:
        Music_Info()
        {

        }
        int Music_Id = 0;
        int favorFrom = 0;
        bool favor = false;
        QString Title = "";
        QString Performer = "";
        QString Duration = "";
        QString FileSize = "";
        QString Album = "";
        QString Path = "";

    };
    class Sheet_Info
    {
    public:
        QString name = "";
        int id = 0;
    };

    class Music_Entity :public Music_Info
    {
    public:
        Music_Entity()
        {

        }
        Music_Entity(QString path)
        {
            MediaInfo(path);
            Path = path;
        }
        void MediaInfo(QString fileName)
        {
            QTime time;
            time.start();
            using namespace MediaInfoDLL;
            MediaInfoDLL::MediaInfo MI = MediaInfoDLL::MediaInfo();
            if(!MI.IsReady())//"MI is ready";
                return;
//            if(isExist("MediaInfo.dll"))
//                qDebug()<<"mediainfo.dll is exist";
//            else
//                return;
            if(!isExist(fileName)) //"file is exist"
                return;
            size_t b = MI.Open(fileName.toStdWString());
            if(b == 0)
            {
                qDebug()<<fileName + " is not opened";
                return;
            }
            if(b == 1)
            {
                qDebug()<<fileName + " is opened";
            }
            Title = QString::fromStdWString( MI.Get(stream_t::Stream_General, 0, QString("Title").toStdWString())  );
            Performer = QString::fromStdWString( MI.Get(stream_t::Stream_General, 0, QString("Performer").toStdWString())  );
            Duration = QString::fromStdWString( MI.Get(stream_t::Stream_General, 0, QString("Duration/String3").toStdWString())  );
            FileSize = QString::fromStdWString( MI.Get(stream_t::Stream_General, 0, QString("FileSize").toStdWString())  );
            Album  = QString::fromStdWString( MI.Get(stream_t::Stream_General, 0, QString("Album").toStdWString())  );

            qDebug()<<time.elapsed();
            //ValueInfo = QString::fromStdWString(MI.Option(QString("Info_Parameters").toStdWString()));

//            MI.Option(QString("Complete").toStdWString());
//            QString q_info;
//            q_info = QString::fromStdWString(MI.Inform());

        }

        //QString ValueInfo;
    private:
        bool isExist(QString fileName)
        {
            QFileInfo info(fileName);
            return info.exists();
        }
    };
}


#endif // TOOL_ENTITY_H
