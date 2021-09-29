#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPainter>
#include <QScreen>
#include <thread>
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    Qt::WindowFlags flags = this->windowFlags();
    this->setAttribute(Qt::WA_TranslucentBackground, true);//关键
    flags = (Qt::FramelessWindowHint);
    this->setAcceptDrops(true);
    this->setWindowOpacity(1);
    this->setWindowFlags(flags);
    //this->resize(600,160);
    this->resize(1400,920);
    this->setMinimumSize(1400,920);
    QScreen *screen = QGuiApplication::primaryScreen ();
    QRect mm=screen->availableGeometry() ;
    int screen_width = mm.width();
    int screen_height = mm.height();

    this->move( (screen_width - this->width())/2 ,(screen_height - this->height())/2);
    //this->setMinimumSize(600,160);
    isclose = false;

    _player = new Frame_Player(this);
    _move = new Tool_MoveWidget(this);
    _berth =new Tool_Berth(this->isclose, this);
    //player->move(10,10);
    _player->move(this->width() - _player->width() - 10,this->height() - this->_player->height() - 10);

    _list = new Frame_List(this);
    _list->move(60,60);

    btn_close = new My_Btn(this);
    btn_close->resize(20,20);
    btn_close->set_img_formal(":/images/close_black.png");
    btn_close->move(this->width() - 30 - btn_close->width(), 20);


    connect(_player, &Frame_Player::signal_add_music, _list, &Frame_List::slot_add_music);
    sql = Tool_SounderSql();
    //connect(_list, &Frame_List::signal_music_play, this, &MainWindow::play);
    connect(_list, &Frame_List::signal_music_add, this, &MainWindow::slot_music_add);
    connect(_list, &Frame_List::signal_sheet_add, this, &MainWindow::slot_sheet_add);
    connect(_list, &Frame_List::signal_music_favorChanged, this ,&MainWindow::slot_music_favorChanged);
    connect(_list, &Frame_List::signal_music_delete, this ,&MainWindow::slot_music_delete);
    //connect(_list, &Frame_List::signal_list_set, this, &MainWindow::slot_list_set);
    connect(_list, &Frame_List::signal_list_play, this, &MainWindow::slot_list_play);
    connect(btn_close, &My_Btn::clicked, this, &MainWindow::close);
    connect(_list, &Frame_List::signal_sheet_delete, this ,&MainWindow::slot_sheet_delete);
    ui->setupUi(this);
    dbInit();

}

MainWindow::~MainWindow()
{
    isclose = true;
    qDebug()<<"~MainWindow";
    delete ui;
}

void MainWindow::read_musicInfoFromDB()
{

}

void MainWindow::dbInit()
{
    std::list<SqlData> sheet;
    sheet = sql.qsqlite_select("select * from sounder_sheet;", 2);

    if(sheet.size() > 0)
    {
        SqlData first = sheet.front();
        sheet.pop_front();
        SqlData seconde = sheet.front();
        sheet.pop_front();

    }else
    {
        db_addDefaultSheet();
    }
    thread_data = new std::thread(&MainWindow:: thread_readData, this);
    thread_data->detach();
}

void MainWindow:: thread_readData()
{
    Sheet_Music allmusic = sql.get_AllMusic();
    Sheets sheets = sql.get_AllSheet();
    for(auto i : sheets)
    {
        if(i.first < 2)
            continue;
        _list->add_sheet(i.second.name);
    }
    for(auto i :allmusic)
    {
        Music ms = i.second;
        for(auto j: ms)
        {
            _list->add_music(i.first, j.favorFrom,j.favor, j.Title ,j.Performer, j.Album, j.Duration, j.Path);
        }
    }
    //_list->add_music();
}

void MainWindow::db_addDefaultSheet()
{
    std::list<SqlBase> sheet;
    SqlBase favor, def;
    favor.setData("Sheet_Id", 0);
    def.setData("Sheet_Name", "我的收藏");
    sheet.push_back(favor);
    sheet.push_back(def);

    std::list<SqlBase> sheet2;
    SqlBase favor2, def2;
    favor2.setData("Sheet_Id", 1);
    def2.setData("Sheet_Name", "默认歌单");
    sheet2.push_back(favor2);
    sheet2.push_back(def2);

    SQLITEENTITY::SQLCMDEntity insert;
    QString cmd = insert.get_insert_cmd(SQLITEENTITY::sounder_sheet, sheet);
    sql._insert(cmd);
    insert = SQLITEENTITY::SQLCMDEntity();
    cmd = insert.get_insert_cmd(SQLITEENTITY::sounder_sheet, sheet2);
    sql._insert(cmd);


}

void MainWindow::play(QString path)
{
    _player->Init(path);
    _player->Play();
}


void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter paint(this);
    paint.setRenderHint(QPainter::Antialiasing);
    QPen pen = QPen(QColor(233,195,130),5,Qt::SolidLine); //211,220,213 //233,195,130
    QPainterPath path;
    path.addRoundedRect(QRectF(pen.width(), pen.width(), width() - pen.width()*2, height() - pen.width()*2), 20, 20);
    paint.save();
    QBrush brush = QBrush(QColor(158,137,99,120)); //158,137,99,80 //123,105,80,120
    paint.setBrush(brush);
    paint.drawPath(path);
    paint.restore();

    paint.save();
    paint.setPen(pen);
    paint.drawPath(path);
    paint.restore();

}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    _berth->mousePressEvent(event);
    _move->mousePressEvent(event);
    event->accept();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    _berth->mouseReleaseEvent(event);
    _move->mouseReleaseEvent(event);
    event->accept();

}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    _move->mouseMoveEvent(event);
    event->accept();

}

void MainWindow::leaveEvent(QEvent *event)
{
    _berth->leaveEvent(event);
    _move->leaveEvent(event);
    event->accept();

}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    _player->move((this->width() - _player->width())/2,this->height() - this->_player->height() - 10);
    btn_close->move(this->width() - 30 - btn_close->width(), 20);
    _list->move(60,60);
    _list->resize(this->width() - 120, this->height() - 60 - _player->height() );
    qDebug()<<_player->width()<<";"<<_player->height();

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    isclose = true;
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->ignore();
    event->acceptProposedAction();
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->ignore();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    event->accept();
    event->ignore();
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty())
        return ;
    _list->add_LotsMusic(urls);
//    for(auto url : urls)
//    {
//        QString path = url.toLocalFile();
//        QString format = path.mid( path.lastIndexOf('.') + 1, path.length() - path.lastIndexOf('.'));
//        if(format == "mp3" || format == "mp4")
//        {
//            Tool_Entity::Music_Entity m_entity = Tool_Entity::Music_Entity(path);
//            qDebug()<< "Music_Entity size:"<<sizeof (QString);
//            _list->add_music(_list->currentSheet(), 0, false,path);
//        }

//    }

}


void MainWindow::slot_music_add(int sheet,  int musicid, int favorFrom, bool favor, QString title, QString performer, QString duration, QString album , QString len, QString path)
{

    SqlBase base1;
    base1.setData(SQLITEENTITY::music_sheet_id, sheet);
    SqlBase base2;
    base2.setData(SQLITEENTITY::music_id, musicid);
    SqlBase base3;
    base3.setData(SQLITEENTITY::music_favor, favor);
    SqlBase base4;
    base4.setData(SQLITEENTITY::music_title,title);
    SqlBase base5;
    base5.setData(SQLITEENTITY::music_performer, performer);
    SqlBase base6;
    base6.setData(SQLITEENTITY::music_album, duration);
    SqlBase base7;
    base7.setData(SQLITEENTITY::music_duration, album);
    SqlBase base8;
    base8.setData(SQLITEENTITY::music_path, path);
    SqlBase base9;
    base9.setData(SQLITEENTITY::music_FavorFrom, favorFrom);

    std::list<SqlBase> bases;
    bases.push_back(base1);
    bases.push_back(base2);
    bases.push_back(base3);
    bases.push_back(base4);
    bases.push_back(base5);
    bases.push_back(base6);
    bases.push_back(base7);
    bases.push_back(base8);
    bases.push_back(base9);


    QString cmd = SQLCMDEntity::get_insert_cmd(sounder_music,bases);

    sql._insert(cmd);

}

void MainWindow::slot_music_delete(int sheetId, int musicId, QString path)
{
    sql.delet_music(sheetId, musicId, path);
}


void MainWindow::slot_sheet_add(int id, QString name)
{
    SqlBase base1;
    base1.setData(SQLITEENTITY::sheet_id,id);
    SqlBase base2;
    base2.setData(SQLITEENTITY::sheet_name,name);
    std::list<SqlBase> bases;
    bases.push_back(base1);
    bases.push_back(base2);
    QString cmd = SQLCMDEntity::get_insert_cmd(sounder_sheet,bases);
    sql._insert(cmd);
}

void MainWindow::slot_sheet_delete(int id, QString name)
{
    sql.delet_sheet(id);
}

void MainWindow::slot_music_favorChanged(bool favor, int sheet, int music, QString path)
{
    sql.updateFavorState(favor, sheet, music ,path);
}

void MainWindow::slot_list_set(QMediaPlaylist *list)
{
    _player->set_list(list);
}

void MainWindow::slot_list_play(QMediaPlaylist *list, int musicId)
{

    _player->play_list(list, musicId);
}
