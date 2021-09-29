#ifndef TOOL_SQLITE_H
#define TOOL_SQLITE_H
#include <qdebug.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QCoreApplication>
#include <stdlib.h>
#include <QDir>
#include <QApplication>
#include <QPluginLoader>
#include <list>
#include "Tool_File.h"
#include "Tool_Sqlite_Entity.h"
typedef std::list<QVariant> SqlData;
using namespace SQLITEENTITY;
class Tool_Sqlite
{
protected:
    QSqlDatabase database;
    QSqlQuery sql_query;
    QString create_sheet = "CREATE TABLE [sounder_sheet]([Sheet_Id] INT PRIMARY KEY NOT NULL,[Sheet_Name] TEXT(50) NOT NULL,[Sheet_Img] BLOB_TEXT);";
    QString create_music = "CREATE TABLE [sounder_music]([Sheet_Id] INT NOT NULL, [Music_Id] INT,"
        "[Music_Favor] BOOL NOT NULL DEFAULT false, [Music_Title] TEXT,[Music_Performer] TEXT, "
        "[Music_Album] TEXT, [Music_Duration] TEXT, [Music_Path] TEXT NOT NULL);";
public:
    Tool_Sqlite()
    {
        initial();
    }
    ~Tool_Sqlite()
    {
        close();
    }
    void initial()
    {
        TOOL_FILE::isDirExist("data");
        TOOL_FILE::isDirExist("sqldrivers");
//        if(!TOOL_FILE::isFileExist("./sqldrivers/qsqlite.dll"))
//        {
//            TOOL_FILE::copyFile(":/files/include/qsqlite.dll", "./sqldrivers/qsqlite.dll");
//        }
        bool dbExist = TOOL_FILE::isFileExist("./data/data.db");
        QString qexeFullPath = QCoreApplication::applicationDirPath(); //exe当前路径
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("data.db");
        database.setDatabaseName(qexeFullPath + "/data/data.db");
        if (open())
        {
            sql_query = QSqlQuery(database);
            qDebug()<<"open sql success";
            if(!dbExist)
            {
                base_cmd(create_music);
                base_cmd(create_sheet);
            }

        }
        else
        {
            qDebug()<<"open sql error";
        }

    }
    void close()
    {
        if(database.isOpen())
        {
            database.close();
        }
    }

    bool open()
    {
        if (!database.open())
        {
            qDebug() << "Error: Failed to connect database." << database.lastError();
            return false;
        }
        else
        {
            qDebug() << "Succeed to connect database." ;
            return true;
        }

    }
    bool base_cmd(QString cmd)
    {
        open();
        bool result = false;
        if(!sql_query.exec(cmd))
        {
            qDebug()<<sql_query.lastError();
            }
        else
        {
            result = true;
        }
        sql_query.clear();
        sql_query.finish();
        return result;
    }
    bool _insert(QString insert_cmd)
    {
        return base_cmd(insert_cmd);
    }
    std::list<SqlData> qsqlite_select(QString select,int count)
    {
        open();
        std::list<SqlData> values;
        select.append(" order by Sheet_id asc;");
        sql_query.setForwardOnly(true);
        //sql_query.exec(select);
        if(!sql_query.exec(select))
        {
            qDebug()<<sql_query.lastError();

            return values;
        }
        else
        {
            while(sql_query.next())
            {
                SqlData data;
                QString da;
                for(int i = 0;i < count; i++)
                {
                    data.push_back(sql_query.value(i));
                    da = sql_query.value(i).toString();
                }
//                data.push_back(sql_query.value(0));
//                data.push_back(sql_query.value(1));
//                data.push_back(sql_query.value(2));
                values.push_back(data);
            }
        }
        sql_query.clear();
        sql_query.finish();
        sql_query.setForwardOnly(false);
        return values;
    }


};

#endif // TOOL_SQLITE_H
