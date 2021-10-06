#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPainter>
#include <QScreen>
#include <QSystemTrayIcon>
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
    this->setObjectName("Z_Sounder");
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

    sizes.append(QSize(680, 120));
    sizes.append(QSize(1400,920));
    sizes.append(mm.size());

    _player = new Frame_Player(this);
    _move = new Tool_Move(this);
    _berth =new Tool_Berth(this->isclose, this);
    //player->move(10,10);
    _player->move(this->width() - _player->width() - 10,this->height() - this->_player->height() - 10);

    _list = new Frame_List(this);
    _list->move(60,60);

    btn_close = new My_Btn(this);
    btn_close->resize(20,20);
    btn_close->set_img_formal(":/images/close_black.png");
    btn_close->move(this->width() - 30 - btn_close->width(), 30);

    btn_size = new My_Btn(this);
    btn_size->resize(20,20);
    btn_size->set_img_formal(":/images/sizeFormal.png");
    btn_size->move(btn_close->x() - 30 - btn_size->width(), 30);

    btn_mini = new My_Btn(this);
    btn_mini->resize(20,20);
    btn_mini->set_img_formal(":/images/sizeMini.png");
    btn_mini->move(btn_size->x() - 30 - btn_mini->width(), 30);

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
    connect(btn_mini, &My_Btn::clicked, [=](){ sizeState(0); });
    connect(btn_size, &My_Btn::clicked, [=]()
    {
        if(this->size() == sizes[1])
        {
            sizeState(2);
        }else if(this->size() == sizes[2])
        {
            sizeState(1);
        }else if(this->size() == sizes[0])
        {
            sizeState(1);
        }
    });
    connect(_list, &Frame_List::signal_sheet_delete, this ,&MainWindow::slot_sheet_delete);
    ui->setupUi(this);
    dbInit();
    connect(_list, &Frame_List::signal_sheet_update,  [=](int sheet, QString name){
        this->sql.updateSheetName(sheet, name);
    });
    icon_init();
}

MainWindow::~MainWindow()
{
    isclose = true;
    qDebug()<<"~MainWindow";
    delete ui;
}
void MainWindow::icon_init()
{
    if (!QSystemTrayIcon::isSystemTrayAvailable())      //判断系统是否支持系统托盘图标
    {
        return;
    }
    QSystemTrayIcon *_myIcon = new QSystemTrayIcon(this);
    _myIcon->setIcon(QIcon(":/imgs/myIcon64.ico"));   //设置图标图片

    _myIcon->setToolTip("Z_Sounder V1.0");    //托盘时，鼠标放上去的提示信息
    _myIcon->showMessage("SystemTrayIcon","Hi,This is my trayIcon",QSystemTrayIcon::Information,10000);
   // _myIcon->setContextMenu(_myMenu);     //设置托盘上下文菜单
    _myIcon->show();
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
        if(i.first < 10002)
            continue;
        _list->add_sheet(i.second.name, i.second.id);
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
    favor.setData("Sheet_Id", 10000);
    def.setData("Sheet_Name", "我的收藏");
    sheet.push_back(favor);
    sheet.push_back(def);

    std::list<SqlBase> sheet2;
    SqlBase favor2, def2;
    favor2.setData("Sheet_Id", 10001);
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

void MainWindow::sizeState(int state)
{
    this->setMinimumSize(sizes[state]);
    this->resize(sizes[state]);
    QScreen *screen = QGuiApplication::primaryScreen ();
    QRect mm=screen->availableGeometry() ;
    int screen_width = mm.width();
    int screen_height = mm.height();
    this->move( (screen_width - this->width())/2 ,(screen_height - this->height())/2);
    switch (state)
    {
    case 0: //miniSize
    {
        _list->hide();
        btn_mini->hide();
    }
    break;
    case 1://formalSize
    {
        if(_list->isHidden())
            _list->show();
        if(btn_mini->isHidden())
            btn_mini->show();
        this->showNormal();
    }
    break;
    case 2://maxSize
    {
        if(_list->isHidden())
            _list->show();
        if(btn_mini->isHidden())
            btn_mini->show();
        this->showMaximized();
    }
    break;
    }



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
    QBrush brush = QBrush(QColor(158,137,99,120)); //158,137,99,80 //123,105,80,120
    QPainterPath rectPath;
    rectPath.addRect(pen.width()/2, pen.width()/2, width() - pen.width(), height() - pen.width());

    if(this->windowState() != Qt::WindowState::WindowMaximized)
    {
        paint.save();
        paint.setBrush(brush);
        paint.drawPath(path);
        paint.restore();
        paint.save();
        paint.setPen(pen);
        paint.drawPath(path);
        paint.restore();

    }else
    {
        paint.save();
        paint.setBrush(brush);
        paint.drawPath(rectPath);
        paint.restore();
        paint.save();
        paint.setPen(pen);
        paint.drawPath(rectPath);
        paint.restore();

    }

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

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
//    QSize cursize = this->size();
    QScreen *screen = QGuiApplication::primaryScreen ();
    QRect rect=screen->availableGeometry() ;
    int screen_width = rect.width();
    int screen_height = rect.height();
    QSize minsize = QSize(1400,920);
//    this->setMinimumSize(1400,920);
//    this->setMaximumSize(rect.size());
    int state = this->windowState();
//    if(   this->size().width() < rect.width()
//            &&this->size().height() < rect.height()
//            &&this->size().width() >= minsize.width()
//            &&this->size().height() >= minsize.height())
//    {
//        //this->setWindowState(Qt::WindowMaximized);
//        this->resize(rect.size());
//        this->move(0,0);
//    }else if(this->size().width() == rect.width()
//             &&this->size().height() == rect.height())
//    {
//        //this->setWindowState(Qt::WindowFullScreen);
//        //this->setWindowState(Qt::WindowNoState);
//        this->resize(minsize);
//        this->move( (screen_width - this->width())/2 ,(screen_height - this->height())/2);

//    }

    if(this->size() != sizes[0])
    {
        if(this->windowState() == Qt::WindowState::WindowMaximized)
        {
            this->showNormal();
            this->move( (screen_width - this->width())/2 , (screen_height - this->height())/2 );
        }
        else if(   (this->size().width() < rect.width()&&this->size().height() < rect.height()
                &&this->size().width() >= minsize.width()&&this->size().height() >= minsize.height()) || state == Qt::WindowState::WindowNoState  )
        {
            this->showMaximized();
            if(_list->isHidden())
            {
                this->setMinimumSize(1400,920);
                _list->show();
            }
            this->move(0,0);
        }

    }

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
    btn_close->move(this->width() - 30 - btn_close->width(), 30);
    btn_size->move(btn_close->x() - 30 - btn_size->width(), 30);
    btn_mini->move(btn_size->x() - 30 - btn_mini->width(), 30);

    _list->move(60,60);
    _list->resize(this->width() - 120, this->height() - 60 - _player->height() );

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
