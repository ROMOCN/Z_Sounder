#ifndef TOOL_FILE_H
#define TOOL_FILE_H
#include <QFile>
#include <QDir>
#include <QDebug>
namespace TOOL_FILE
{
    bool static isFileExist(QString path)
    {
        bool result = false;
        QFile file(path);
        result = file.exists();
        if(!result)
        {
            file.setFileName(path);
            file.open(QIODevice::ReadWrite);
            file.close();
        }
        return result;
    }

    bool static isDirExist(QString dirname)
    {
        bool result = false;
        QDir dir;
        if(!dir.exists(dirname)){
            dir.mkdir(dirname);
        }
        else {
            result = true;
        }
        return result;
    }

    bool static copyFile(QString source, QString dist)
    {
        QFile file(source);
        qDebug()<<"copy file size:"<<file.size();
        return  QFile::copy(source,dist);
    }
}


#endif // TOOL_FILE_H
