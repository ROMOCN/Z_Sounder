#ifndef TOOL_MP3_H
#define TOOL_MP3_H

#include <QMainWindow>
#include "stdlib.h"
#include "stdio.h"
#include <QDebug>
#include <QImage>
#include <QMap>
#include <QTextCodec>
#include <QString>
#include <QFile>
#include <QUrl>
#include <QPixmap>
namespace MP3
{
using namespace std;
    class MP3Header;

    typedef struct ID3V2FrameH
    {
       char frameID[4];               //存放标签格式，是否为id3v2
       unsigned char size[4];         //存放标签数据的大小
       char flags[2];
    }ID3V2FH;

    typedef struct MP3INFO
    {
        QString Url;            //存放这首歌的详细地址
        QString Name;            //歌名  TIT2
        QString Album;           //专辑  TALB
        QString Singer;          //歌手  TPE1
        QString Picture_url;      //歌曲图片存放路径
        QString Picture_type;     //图片类型 jpg,png
        QPixmap picture;
        int     number;          //歌曲编号
        int     beginNum;         //图片起始位置
        int     lenth;            //图片数据长度
        bool     pic_flag;         //是否有图片


    }MP3INFO;
    typedef struct frameIDStruct
    {
        int beginNum;
        int endNum;
        QString FrameId;

    }frameIDStruct;

    class MP3Header
    {
    public:
        MP3Header()
        {

        }
        FILE *fp;
        QString m_url;
        unsigned char Header[3];
        unsigned char FrameId[4];     //存放帧标识
        unsigned char Header_size[4];
        unsigned int mp3_TagSize;
        unsigned char frameSize[4];      //存放该帧内容的大小
        unsigned int framecount;          //计算出帧内容的大小
        //void GetMp3IDV2(const wchar_t *url);
//        void GetPicture(MP3INFO *mp3info);
//        void GetFrameId();
//        QString GetInfo(QString fId);
        MP3INFO GetAllInfo(const wchar_t *url,int songNumber)
        {
            m_url = QString::fromWCharArray(url);
            fp = _wfopen(url,L"rb");
            if (NULL==fp)
            {
             printf("open read file error!!");
             MP3INFO falseInfo;
             falseInfo.pic_flag = false;
             return falseInfo;
            }
            fseek(fp,0,SEEK_SET);
            fread(&Header,1,3,fp);
            if(Header[0]=='I'&&Header[1]=='D'&&Header[2]=='3')
            {
                qDebug()<<"open ID3 correct!";
            }
            unsigned int i = 10;
            MP3INFO mp3info_struct;
            mp3info_struct.Url = m_url;
            mp3info_struct.number = songNumber;
            while(i<(mp3_TagSize-10))
            {
                frameIDStruct m_struct;

                fseek(fp,i,SEEK_SET);
                fread(&FrameId,1,4,fp);
                fseek(fp,4+i,SEEK_SET);
                fread(&frameSize,1,4,fp);
                framecount = frameSize[0]*0x1000000+frameSize[1]*0x10000+frameSize[2]*0x100+frameSize[3];
                //qDebug()<<"framecount:"<<framecount;
                QString aa;
                aa = FrameId[0];
                aa+=FrameId[1];
                aa+=FrameId[2];
                aa+=FrameId[3];
                //qDebug()<<"aa:"<<aa;
                m_struct.beginNum = i+10;
                m_struct.FrameId = aa;
                i =10+ i+framecount;
                m_struct.endNum = i;
                m_IDmap.insert(aa,m_struct);
                int lenth = m_struct.endNum-m_struct.beginNum;
                if(m_struct.FrameId=="APIC")
                {
                    unsigned char temp[20] = {0};
                    fseek(fp,m_struct.beginNum,SEEK_SET);
                    fread(&temp,1,20,fp);
                    int tank=0;
                    int j = 0;
                    int pictureFlag=0;
                    while(1)
                    {
                        if((temp[j] == 0xff)&&(temp[j+1] == 0xd8))              //jpeg
                        {
                            tank = j;
                            pictureFlag=1;
                            mp3info_struct.Picture_type = ".jpg";
                            qDebug()<<"jpeg";
                            qDebug()<<"j:"<<j;
                            break;
                        }else if((temp[j] == 0x89)&&(temp[j+1] == 0x50))       //png
                        {
                            tank = j;
                            pictureFlag=2;
                            mp3info_struct.Picture_type = ".png";
                            qDebug()<<"png";
                            qDebug()<<"j:"<<j;
                            break;
                        }
                        j++;
                    }
                    //qDebug()<<"frameSize:"<<i;
                    //10+i为frameid自己的首地址

                    unsigned char t[lenth];
                    memset(t, 0 ,lenth);
                    fseek(fp,m_struct.beginNum+tank,SEEK_SET);
                    mp3info_struct.beginNum = m_struct.beginNum+tank;
                    mp3info_struct.lenth = lenth;
                    fread(&t,1,lenth,fp);
                    if(pictureFlag==1)          //jpeg
                    {
                        QString temp_1 = "C:/Users/Admin/Desktop/new_text";
                        QString temp_2 = QString::number(songNumber,10);
                        temp_1+=temp_2;
                        temp_1+=".jpg";
                        mp3info_struct.Picture_url = temp_1;
                        if(songNumber<3)
                        {
                            std::string str_temp = temp_1.toStdString();
                            const char *ch = str_temp.c_str();
                            FILE *fpw = fopen( ch, "wb" );
                            fwrite(&t,lenth,1,fpw);
                            fclose(fpw);     //是否也需要关掉fp
                        }

                    }else if(pictureFlag==2)        //png
                    {
                        QString temp_1 = "C:/Users/Admin/Desktop/new_text";
                        QString temp_2 = QString::number(songNumber,10);
                        temp_1+=temp_2;
                        temp_1+=".png";
                        mp3info_struct.Picture_url = temp_1;
                        if(songNumber<4)
                        {
                            std::string str_temp = temp_1.toStdString();
                            const char *ch = str_temp.c_str();
                            FILE *fpw = fopen( ch, "wb" );
                            fwrite(&t,lenth,1,fpw);
                            fclose(fpw);     //是否也需要关掉fp
                            fclose(fp);
                        }

                    }
                    mp3info_struct.picture.loadFromData(t, lenth);

                }
                else if(m_struct.FrameId=="TIT2")       //标题
                {
                    QFile file(m_url);
                     if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
                         qDebug()<<"Can't open the file!"<<endl;
                         MP3INFO falseInfo;
                         falseInfo.pic_flag = false;
                         return falseInfo;
                     }
                     QTextStream stream(&file);
                     stream.seek(m_struct.beginNum+1);
                     QString all= stream.readLine(lenth-1);
                     QTextCodec *codec = QTextCodec::codecForName("GBK");
                     QString ua = codec->toUnicode(all.toLocal8Bit().data());
                     QString unser = ua.mid(0,(int)(lenth/2-1));
                     mp3info_struct.Name = unser;
                     //mp3info_struct.beginNum = m_struct.beginNum;
                     //mp3info_struct.lenth = lenth;
                     file.close();
                }else if(m_struct.FrameId=="TPE1")       //歌手
                {
                    QFile file(m_url);
                     if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
                         qDebug()<<"Can't open the file!"<<endl;
                         MP3INFO falseInfo;
                         falseInfo.pic_flag = false;
                         return falseInfo;
                     }
                     QTextStream stream(&file);
                     stream.seek(m_struct.beginNum+1);
                     QString all= stream.readLine(lenth-1);
                     QTextCodec *codec = QTextCodec::codecForName("GBK");
                     QString ua = codec->toUnicode(all.toLocal8Bit().data());
                     QString unser = ua.mid(0,(int)(lenth/2-1));
                     mp3info_struct.Singer = unser;
                     //mp3info_struct.beginNum = m_struct.beginNum;
                     //mp3info_struct.lenth = lenth;
                     file.close();
                }else if(m_struct.FrameId=="TALB")       //专辑
                {
                    QFile file(m_url);
                     if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
                         qDebug()<<"Can't open the file!"<<endl;
                         MP3INFO falseInfo;
                         falseInfo.pic_flag = false;
                         return falseInfo;
                     }
                     QTextStream stream(&file);
                     stream.seek(m_struct.beginNum+1);
                     QString all= stream.readLine(lenth-1);
                     QTextCodec *codec = QTextCodec::codecForName("GBK");
                     QString ua = codec->toUnicode(all.toLocal8Bit().data());
                     QString unser = ua.mid(0,(int)(lenth/2-1));
                     mp3info_struct.Album = unser;
                     //mp3info_struct.beginNum = m_struct.beginNum;
                     //mp3info_struct.lenth = lenth;
                     file.close();
                }
                //qDebug()<<"frameSize:"<<i;     //10+i为frameid对应数据的首地址
                if(aa=="APIC")
                {
                    break;
                }
            }
            mp3info_struct.pic_flag = true;
            return mp3info_struct;
        }


        QMap<QString,frameIDStruct> m_IDmap;

    };


}
#endif // TOOL_MP3_H
