#ifndef TOOL_SQLITE_ENTITY_H
#define TOOL_SQLITE_ENTITY_H

#include <QObject>
#include <QString>
#include <iostream>
#include <QVariant>
namespace SQLITEENTITY
{
//db name
const QString sounder_music = "sounder_music";
const QString sounder_sheet = "sounder_sheet";
//sheet columns
const QString sheet_id = "Sheet_Id";
const QString sheet_name = "Sheet_Name";
const QString sheet_img = "Sheet_Img";
//music colums
const QString music_sheet_id = "Sheet_Id";
const QString music_id = "Music_Id";
const QString music_favor = "Music_Favor";
const QString music_title = "Music_Title";
const QString music_performer = "Music_Performer";
const QString music_album = "Music_Album";
const QString music_duration = "Music_Duration";
const QString music_path = "Music_Path";
const QString music_FavorFrom = "Music_FavorFromSheet";
class SqlBase
{
public:
    bool bfile = false;
    char *file;
    int file_len = 0;
    void set_file(const char* data,int len)
    {
        QVariant v;
        file = new char[len];
        memcpy(file, data, len);
        file_len = len;
        bfile = true;
    }
    bool isMarked()
    {
        return bmark;
    }
    void setData(QVariant key, QVariant value)
    {
        _key = key.toString();
        _value = value.toString();
        if(value.typeName() == "QString")
        {
            bmark = true;
        }
//        QString tname = value.typeName();
//        int tid = value.userType();
//        int trlt = value.nameToType(tname.toStdString().c_str());
    }
    QString key()
    {
        return  _key;
    }
    QString value()
    {
        return _value;
    }
private:
    bool bmark = true;//是否需要添加引号
    QString _key = "";
    QString _value = "";
};

class SQLCMDEntity
{

public:
    enum ENUM_DB
    {
        DB_SHEET = 1,
        DB_MUSIC = 2
    };
    SQLCMDEntity(ENUM_DB DB)
    {
        Init(DB);
    }
    SQLCMDEntity()
    {
    }
    static QString get_insert_cmd(QString dbName, std::list<SqlBase> db_contains)
   {
       QString insert_cmd = "";
       if(dbName != NULL && dbName != "" && db_contains.size() != 0)
       {
           QString cmd_keys = "";
           QString cmd_values = "";
           for(auto in :db_contains)
           {
               cmd_keys.append(in.key() + " ,");
               if(!in.isMarked())
                   cmd_values.append(in.value() + " ,");
               else
                   cmd_values.append("'" + in.value() + "' ,");
           }
           if(cmd_keys.lastIndexOf(",") == cmd_keys.length()-1)
               cmd_keys.chop(1);
           if(cmd_values.lastIndexOf(",") == cmd_values.length()-1)
               cmd_values.chop(1);
           insert_cmd  = QString("INSERT INTO %1 (%2) VALUES (%3);")
                   .arg(dbName).arg(cmd_keys).arg(cmd_values);
       }
       return insert_cmd;
   }

   static QString get_select_cmd(QString db_name, std::list<SqlBase> db_content, QString db_select_contain = "*")
    {
        count = db_content.size();
        QString select_cmd = "select " + db_select_contain + " from " + db_name;
        if(db_content.begin()!=db_content.end())
        {
            select_cmd.append(" where ");
            for(auto i : db_content)
            {
                if(i.isMarked())
                {
                    select_cmd += i.key() + " = '" + i.value() + "'" + " and ";
                }
                else
                {
                    select_cmd += i.key() + " = " + i.value()  + " and ";
                }
            }
            select_cmd.chop(5);
        }
         return select_cmd;
    }
   static int count;//表 列数

private:
    QString _db_name = "";
    std::list<SqlBase> _db_content;
    void Init(ENUM_DB DB)
    {
        switch (DB)
        {
            case ENUM_DB::DB_MUSIC:
            _db_name = sounder_music;
            break;
            case ENUM_DB::DB_SHEET:
            _db_name = sounder_sheet;
            break;

        }
    }
};

}
#endif // TOOL_SQLITE_ENTITY_H
