#ifndef TOOL_SOUNDERSQL_H
#define TOOL_SOUNDERSQL_H
#include "Tool_Sqlite.h"
#include "tool_entity.h"
typedef std::map<int , Tool_Entity::Sheet_Info> Sheets;
typedef std::vector<Tool_Entity::Music_Info> Music;
typedef std::map<int ,Music> Sheet_Music;

class Tool_SounderSql :public Tool_Sqlite
{
public:
    Tool_SounderSql()
    {
        this->initial();
    }
    ~Tool_SounderSql()
    {
        this->close();
    }
    Sheets get_AllSheet()
    {
        Sheets s;
        QString select = "select * from sounder_sheet";
        select.append(" order by Sheet_id asc;");
        sql_query.setForwardOnly(true);
        if(!sql_query.exec(select))
        {
            qDebug()<<sql_query.lastError();
        }
        else
        {
            while(sql_query.next())
            {
                Tool_Entity::Sheet_Info sheet;
                int id = sql_query.value(0).toInt();
                QString name = sql_query.value(1).toString();
                sheet.id = id;
                sheet.name = name;
                s.insert(std::map<int, Tool_Entity::Sheet_Info>::value_type(sheet.id, sheet));
            }
        }
        sql_query.clear();
        sql_query.finish();
        sql_query.setForwardOnly(false);
        return s;
    }
    Sheet_Music get_AllMusic()
    {
        Sheet_Music all;
        QString select = "select * from sounder_music";
        select.append(" order by Sheet_Id asc;");
        sql_query.setForwardOnly(true);
        if(!sql_query.exec(select))
        {
            qDebug()<<sql_query.lastError();
        }
        else
        {
            int Sheet_id = -1;
            while(sql_query.next())
            {
                Sheet_id = sql_query.value(0).toInt();
                Tool_Entity::Music_Info m = Tool_Entity::Music_Info();
                m.Music_Id = sql_query.value(1).toInt();
                m.favor = sql_query.value(2).toBool();
                m.Title = sql_query.value(3).toString();
                m.Performer = sql_query.value(4).toString();
                m.Album = sql_query.value(5).toString();
                m.Duration = sql_query.value(6).toString();
                m.Path = sql_query.value(7).toString();
                m.favorFrom = sql_query.value(8).toInt();
                if(all.count(Sheet_id) == 0)
                {
                    all.insert(std::map<int, Music>::value_type(Sheet_id, std::vector<Tool_Entity::Music_Info>()));
                }
                all.find(Sheet_id)->second.push_back(m);
            }
        }
        sql_query.clear();
        sql_query.finish();
        sql_query.setForwardOnly(false);
        return all;
    }

    bool add_sheet(int sheet_id, QString sheet_name)
    {
        std::list<SqlBase> sheet;
        std::list<SqlBase> def;
        SqlBase id, name;
        id.setData("Sheet_Id", sheet_id);
        name.setData("Sheet_Name", sheet_name);
        sheet.push_back(id);
        sheet.push_back(name);
        SQLITEENTITY::SQLCMDEntity insert;
        QString cmd = insert.get_insert_cmd(SQLITEENTITY::sounder_sheet, sheet);
        _insert(cmd);
        return true;
    }

    bool delet_sheet(int sheet_id)
    {
        QString cmd1 = QString("delete from sounder_sheet where Sheet_Id = %1 ;").arg(sheet_id);
        QString cmd2 = QString("delete from sounder_music where Sheet_Id = %1 ;").arg(sheet_id);
        QString cmd3 = QString("update sounder_sheet set Sheet_Id = Sheet_Id - 1 where Sheet_Id > %1;").arg(sheet_id);
        bool del1 = base_cmd(cmd1);
        bool del2 = base_cmd(cmd2);
        bool del3 = base_cmd(cmd3);
        return del1&del2&del3;
    }
    bool delet_music(int sheet_id, int music_id, QString path)
    {
//        QString cmd = QString("delete from sounder_music where (Sheet_Id = %1 and Music_Id = %2) or "
//                              "(Sheet_Id = %1 and Music_Path = \"%3\");").arg(sheet_id).arg(music_id).arg(path);
        QString cmd = QString("delete from sounder_music where "
                              "Sheet_Id = %1 and Music_Path = \"%2\";").arg(sheet_id).arg(path);

        bool del = base_cmd(cmd);
        QString updateID = QString("update sounder_music set Music_Id = Music_Id - 1 where Sheet_id = %1 and Music_Id > %2;").arg(sheet_id).arg(music_id);
        return base_cmd(cmd)&del;
    }

    bool updateSheetName(int sheet_id, QString newName)
    {
        QString cmd = QString("delete from sounder_sheet where SheetId = %1 ;").arg(sheet_id);
        return base_cmd(cmd);
    }
    bool add_music(int sheet,  int musicid,
                   QString title, QString performer,
                   QString duration, QString album ,
                   QString len, QString path)
    {
        SqlBase base1;
        base1.setData(SQLITEENTITY::music_sheet_id, sheet);
        SqlBase base2;
        base2.setData(SQLITEENTITY::music_id, musicid);
        SqlBase base3;
        base3.setData(SQLITEENTITY::music_favor,"");
        SqlBase base4;
        base4.setData(SQLITEENTITY::music_title,title);
        SqlBase base5;
        base5.setData(SQLITEENTITY::music_performer, performer);
        SqlBase base6;
        base6.setData(SQLITEENTITY::music_album, duration);
        SqlBase base7;
        base7.setData(SQLITEENTITY::music_duration, album);
        SqlBase base8;
        base8.setData(SQLITEENTITY::music_path,path);

        std::list<SqlBase> bases;
        bases.push_back(base1);
        bases.push_back(base2);
        bases.push_back(base3);
        bases.push_back(base4);
        bases.push_back(base5);
        bases.push_back(base6);
        bases.push_back(base7);
        bases.push_back(base8);

        QString cmd = SQLCMDEntity::get_insert_cmd(sounder_music,bases);
        _insert(cmd);
        return true;
    }

    bool updateFavorState(bool favor, QString path)
    {
        QString cmd = QString("update sounder_music set Music_Favor = %1 where Music_Path = %2;").arg(favor).arg(path);
        bool del = base_cmd(cmd);
        return del;
    }
    bool updateFavorState(bool favor, int sheet, int music ,QString path)
    {
        QString cmd = QString("update sounder_music set Music_Favor = %1 where Sheet_Id = %2 and Music_Id = %3 and Music_Path = \"%4\";").arg(favor).arg(sheet).arg(music).arg(path);
        bool del = base_cmd(cmd);
        return del;
    }

};

#endif // TOOL_SOUNDERSQL_H
