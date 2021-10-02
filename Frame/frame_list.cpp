#include "frame_list.h"

#include <QPainter>
#include <QWidgetAction>
#include <thread>
Frame_List::Frame_List(QWidget *parent) : QWidget(parent)
{
    UI_Init();
    connect(btn_add_sheet, &My_Btn::clicked, this, static_cast<void (Frame_List::*)()>(&Frame_List::add_sheet));
    connect(list_music , &My_Table::doubleClicked,this ,&Frame_List::slot_readyToPlay);
    connect(btn_add_music, &My_Btn::clicked, this , &Frame_List::test_add_music);
    connect(list_sheet, &My_ListWidget::currentRowChanged, this, &Frame_List::slot_ListSheetChanged);
    connect(list_music, &My_Table::signal_favorChanged, this, &Frame_List::slot_music_favorChanged);
    add_sheet("我的收藏", ":/images/unfavor.png", 10000);
    add_sheet("默认歌单", ":/images/sheet2.png", 10001);
    list_sheet->item(1)->setSelected(true);
    list_sheet->setCurrentRow(1);
    list_music->set_model(currentSheet());
}
void Frame_List::add_sheet()
{    
    My_MessageBox box;
    QString str = "";
    box.setText("新建歌单:");
    box.setValue(&str);
    box.setKind(My_MessageBox::ENUM_KIND::KIND_NEW);
    int result = box.exec();
    if(result == QDialog::Accepted)
    {
        add_sheet(str, ":/images/sheet2.png");
    }

}

void Frame_List::add_sheet(QString name, QString iconPath ,int sheetId)
{
    QPixmap icon(iconPath);
    My_Item *item = new My_Item(QIcon(icon.scaled(QSize(100,100))), name);
    if(sheetId != 0)
    {
        item->Sheet_Id = sheetId;
    }
    list_sheet->addItem(item);
//    list_music->add_model(list_sheet->count() - 1);
//    emit signal_sheet_add(list_sheet->count() - 1, name);
    list_music->add_model(item->Sheet_Id);
    emit signal_sheet_add(item->Sheet_Id, name);

}

void Frame_List::add_sheet(QString name ,int sheetId)
{
    QString path = ":/images/sheet2.png";
    if(name == "我的收藏" && sheetId == 10000)
    {
        path = ":/images/unfavor.png";
    }

    if(name != "默认歌单" & name != "我的收藏")
    {
    }
    add_sheet(name, path, sheetId);

}
void Frame_List::add_musicInList(int sheet_id, QString path)
{
    QMediaContent content = NULL;
    content = QMediaContent(path);
    QMediaPlaylist *list = nullptr;
    list = list_music->get_playList(sheet_id);
    if(list != nullptr)
        list->addMedia(content);
}

bool Frame_List::add_music(int sheet_id, int favorFrom ,bool favor, QString title, QString performer, QString album, QString duration, QString path)
{
    bool result = false;
    if(sheet_id == 2)
    {
        qDebug()<<"2";
    }
    add_musicInList(sheet_id, path);

    QStandardItemModel *model = NULL;
    model = list_music->get_model(sheet_id);
    if(sheet_id >= 0 && model != NULL)
    {

        int i = model->rowCount();
        QStandardItem *first =
                new QStandardItem(QString("%1").arg(list_music->get_model(sheet_id)->rowCount() + 1,
                2, 10,QLatin1Char('0')));
        first->setTextAlignment(Qt::AlignCenter);
        model->setItem(i, 0, first);
        QStandardItem *item = new QStandardItem("");
        favor? item->setIcon(QPixmap(":/images/favor.png")) : item->setIcon(QPixmap(":/images/unfavor.png"));
        favor? item->setToolTip("取消收藏") : item->setToolTip("添加收藏");
        model->setItem(i, 1, item);
        //My_FavorBtn *button = new My_FavorBtn();
        //list_music->setIndexWidget(model->index(i, 1), button);
        model->setItem(i, 2, new QStandardItem(title));
        model->setItem(i, 3, new QStandardItem(performer));
        model->setItem(i, 4, new QStandardItem(album));
        duration = stringHandle(duration);
        model->setItem(i, 5, new QStandardItem(duration));
        model->setItem(i, 6, new QStandardItem(path));
        model->setItem(i, 7, new QStandardItem(QString("%1").arg(favorFrom)));
        result = true;
    }
    return result;
}

bool Frame_List::add_music(int sheet_id, int favorFrom ,bool favor, QString path)
{
    if(sheet_id == 0)
        favor = true;

    Tool_Entity::Music_Entity music = Tool_Entity::Music_Entity(path);
    int rowCount = list_music->get_model(sheet_id)==NULL?0 : list_music->get_model(sheet_id)->rowCount();
    emit signal_music_add(sheet_id,  rowCount,favorFrom, favor, music.Title,music.Performer, music.Album ,music.Duration, music.FileSize, music.Path);
    add_music(sheet_id, favorFrom, favor, music.Title,music.Performer,  music.Album, music.Duration,music.Path);
    list_music->setVisible(true);
    return true;
}

//void Frame_List::add_LotsMusic(QStringList path)
//{
//    if(path.isEmpty())
//        return ;
//    list_music->setUpdatesEnabled(false);  //暂停界面刷新
//    std::thread thr([=]()
//    {
//        for(auto url : path)
//        {
//            QString path = url;
//            QString format = path.mid( path.lastIndexOf('.') + 1, path.length() - path.lastIndexOf('.'));
//            if(format == "mp3")
//            {
//                Tool_Entity::Music_Entity m_entity = Tool_Entity::Music_Entity(path);
//                qDebug()<< "Music_Entity size:"<<sizeof (QString);
//                add_music(currentSheet(), 0, false, path);
//            }
//        }
//    });
//    thr.detach();
//    list_music->setUpdatesEnabled(true);  //界面刷新


//}

void Frame_List::add_LotsMusic(QList<QUrl> urls)
{
    if(urls.isEmpty())
        return ;
    list_music->setUpdatesEnabled(false);  //暂停界面刷新
    std::thread thr([=]()
    {
        for(auto url : urls)
        {
            QString path = url.toLocalFile();
            QString format = path.mid( path.lastIndexOf('.') + 1, path.length() - path.lastIndexOf('.'));
            if(format == "mp3")
            {
                Tool_Entity::Music_Entity m_entity = Tool_Entity::Music_Entity(path);
                qDebug()<< "Music_Entity size:"<<sizeof (QString);
                add_music(currentSheet(), 0, false, path);
            }
        }

    });
    thr.detach();
    list_music->setUpdatesEnabled(true);  //界面刷新

}



void Frame_List::delete_music(int sheet_id, int music_id, QString path)
{
    My_MessageBox box;
    box.setText("是否从歌单中移除该歌曲?");
    box.setKind(My_MessageBox::ENUM_KIND::KIND_YESON);
    int result = box.exec();
    if(result == QDialog::Accepted)
    {
        if(sheet_id >= 0 &&music_id>=0)
        {
            list_music->get_model(sheet_id)->removeRow(music_id);
            list_music->get_playList(sheet_id)->removeMedia(music_id);
            emit signal_music_delete(sheet_id, music_id, path);
        }

    }else if(result == 0)
    {

    }
}

void Frame_List::delete_sheet(int row)
{
    My_MessageBox box;
    box.setText("是否移除该歌单?");
    box.setKind(My_MessageBox::ENUM_KIND::KIND_YESON);
    int result = box.exec();
    if(QDialog::Accepted)
    {
        QString str = list_sheet->item(row)->text();
        My_Item *item = (My_Item*)list_sheet->item(row);
        int Sheet_Id = item->Sheet_Id;
        //item = list_sheet->takeItem(sheetId);
        list_sheet->removeItemWidget(item);
        delete item;
        list_music->delete_model(Sheet_Id);
        list_music->delete_playList(Sheet_Id);
        emit signal_sheet_delete(Sheet_Id, str);
    }

}

void Frame_List::delete_musicFromFavor(int sheet_id, int music_id, QString path)
{


}

void Frame_List::update_sheet()
{
    My_MessageBox box;
    QString str = "";
    box.setText("歌单修改为:");
    box.setValue(&str);
    box.setKind(My_MessageBox::ENUM_KIND::KIND_NEW);
    int result = box.exec();
    if(result == QDialog::Accepted)
    {
        signal_sheet_update(currentSheet(), str);
        list_sheet->currentItem()->setText(str);
    }


}



void Frame_List::test_add_music()
{

}

void Frame_List::slot_add_music(QString path)
{


}

void Frame_List::slot_readyToPlay(QModelIndex row)
{
    if (row.isValid())
    {
        int sheetId = currentSheet();
        QStandardItemModel *model = list_music->get_model(sheetId);


         QStandardItem *item =  list_music->get_model(sheetId)->item(row.row(),
                                list_music->get_model(sheetId)->columnCount() - 1);
         QStandardItem *item2 = list_music->get_model(sheetId)->itemFromIndex(row); //选中单元格
         QString path = item->text();
         QString pat2h = item2->text();
         //list_music->get_playList(i)->
         //emit signal_music_play(item->text());
         //emit signal_list_set(list_music->get_playList(i));
         QMediaPlaylist *l = new QMediaPlaylist(list_music);

         for(int i = 0; i< model->rowCount(); i++)
         {
            QString str =  model->item(i, 6)->text();
            l->addMedia(QMediaContent(str));
         }
//         QFile file("plist.m3u");
//         file.open(QIODevice::WriteOnly);
//         list->save(&file,"m3u");
//            file.close();
//         QFile file1("plist1.m3u");
//         file1.open(QIODevice::WriteOnly);
//         l->save(&file1,"m3u");
//            file1.close();

         emit signal_list_play(l, row.row());

//         emit signal_list_play(list_music->get_playList(i), row.row());
    }

}

void Frame_List::slot_readtToDelete(QModelIndex row)
{

    QString path = list_music->get_model(currentSheet())->item(row.row(), 6)->text();
    delete_music(currentSheet(), row.row(),path);
}

void Frame_List::slot_music_favorChanged(bool favor, int music_id)
{
    QString favorPath = "";
    favorPath = list_music->get_model(currentSheet())->item(music_id, 6)->text();
    int favorFrom = currentSheet();
    if(favor)
    {
        add_music(10000, favorFrom, favor, favorPath);
    }else
    {
        //删除收藏歌单的DB数据
        emit signal_music_delete(10000, music_id, favorPath);
        //获取收藏歌单的内容
        QStandardItemModel *model  = list_music->get_model(10000);
        //收藏列表取消收藏，关联歌单对应歌曲取消收藏
        //set_musicUnfavor(sheet, music, path);
        //如果被修改的是收藏歌单内的歌曲则，在其他歌单中的对应歌曲修改收藏状态
        if(currentSheet() == 10000 && model->rowCount() > 0)
        {
            int sourceSheet = model->item(music_id, 7)->text().toInt();
            if(sourceSheet >10000)
            {
                QStandardItemModel *model  = list_music->get_model(sourceSheet);
                if(model->findItems(favorPath, Qt::MatchWildcard, 6).size() > 0)
                {
                    int sourceRow = model->findItems(favorPath, Qt::MatchWildcard, 6).first()->row();
                    QStandardItem *item = model->item(sourceRow,1);
                    item->setIcon(QPixmap(":/images/unfavor.png"));
                    item->setToolTip("添加收藏");

                    emit signal_music_favorChanged(favor, sourceSheet, item->row(), favorPath);

                }
            }
        }
        //在收藏歌单中删除对应行
        foreach (const QStandardItem *item, model->findItems(favorPath, Qt::MatchWildcard, 6))
        {
            QString dest = item->text();
                if (dest == favorPath)
                {
                    model->removeRow(item->index().row());
                    break;
                }
        }
    }
    emit signal_music_favorChanged(favor, currentSheet(), music_id, favorPath);
}


void Frame_List::slot_ListSheetChanged(int row)
{
     list_music->setModel(NULL);
     if(currentSheet() >= 10000)
        list_music->set_model(currentSheet());
    list_music->setColumnWidth(0,60);
    list_music->setColumnWidth(1,20);
    list_music->setColumnWidth(2,300);
    list_music->setColumnWidth(3,200);
    list_music->setColumnWidth(4,200);
    list_music->setColumnWidth(5,100);
    list_music->setColumnWidth(6,0);
    list_music->setColumnHidden(6, true);
    list_music->setColumnWidth(7,0); //收藏来源歌单ID
    list_music->setColumnHidden(7, true);

    //区分收藏歌单
    if(row == 0)
    {
    }

}

void Frame_List::slot_ListMusicChanged(int row)
{

}
void Frame_List::resizeEvent(QResizeEvent *event)
{
    //if()
    {
        back_sheet->move(10,10);
        back_sheet->resize(300, this->height() - 20);

        back_music->move(back_sheet->width() + 30, 10);
        back_music->resize(this->width() - (back_sheet->width() + back_sheet->x() + 30), this->height() - 20);
        btn_add_sheet->move(back_sheet->x() + back_sheet->width() - 70, back_sheet->y() + 24);
        btn_add_music->move(back_music->x() + 100, back_music->y() + 60);

        list_sheet->resize(back_sheet->width() - 40, back_sheet->height() - 95);
        list_music->resize(back_music->width() - 40, back_music->height() - 165);

    }
}




QString Frame_List::stringHandle(QString str)
{
    QString result = "";
    int index = str.indexOf('.');
    result = str.remove(index, str.length() - index);
    if(result.startsWith("00"))
    {
        int i = result.indexOf(':');
        result = result.remove(0, i + 1);
    }
    return result;
}


void Frame_List::paintEvent(QPaintEvent *event)
{
//    QPainter paint(back_sheet);
//    QPen pen = QPen(QColor(255,255,255),5,Qt::SolidLine); //211,220,213 //233,195,130
//    paint.save();
//    paint.setPen(pen);
//    paint.drawLine(pen.width(), 60, back_sheet->width() - pen.width(), 120);
//    paint.restore();
//    paint.begin(back_music);
//    paint.save();
//    paint.drawLine(pen.width(), 150, back_music->width() - pen.width(), 60);
//    paint.restore();

//        QPainter p(this);
//        p.begin(this);
//        p.setBrush(QBrush(QColor(255,255,255,200)));
//        p.drawRect(0, 0, this->width(), this->height());
//        p.save();

    if(list_music->model()!=nullptr)
    {
        if(list_music->model()->rowCount() == 0)
        {
            list_music->setVisible(false);
            QPainter paint(this);
            QPen pen = QPen(QColor(255,255,255),14,Qt::SolidLine); //211,220,213 //233,195,130
            paint.save();
            paint.setPen(pen);
            QString str = "点击按钮或拖动添加歌曲";
            QFont f;
            f.setUnderline(true);
            f.setFamily("微软雅黑");
            f.setPointSize(20);
            QFontMetrics fm(f);
            QRect rec = fm.boundingRect( str);
            paint.drawText(back_music->x() + (back_music->width() - rec.width())/2,back_music->y() + (back_music->height()/2) + 100,str);

        }
        else
        {
            list_music->setVisible(true);
        }
    }

}
void Frame_List::UI_Init()
{
    back_music = new My_ListBack(this);
    back_sheet = new My_ListBack(this);
    back_music->set_height(150);
    btn_add_sheet = new My_Btn(this);
    btn_add_sheet->resize(32,32);
    btn_add_sheet->set_img_formal(":/images/addSheet.png");
    btn_add_sheet->move(back_sheet->x() + back_sheet->width() - 70,back_sheet->y() + 24);
    btn_add_music = new My_Btn(this);
    btn_add_music->resize(26,26);
    btn_add_music->set_img_formal(":/images/addFile2.png");
    btn_add_music->move(back_music->x() + 100, 60);

    list_sheet = new My_ListWidget(back_sheet);
    list_sheet->move(20, 85);
    list_sheet->resize(back_sheet->width() - 40, back_sheet->height() - 95);
    list_sheet->setContextMenuPolicy(Qt::CustomContextMenu);//设置为自定义菜单

    list_music = new My_Table(back_music);
    list_music->move(20, 155);
    list_music->resize(back_music->width() - 40, back_music->height() - 165);
//    list_music->_model->setColumnCount(7);
//    list_music->_model->setHeaderData(0,Qt::Horizontal,QString(""));
//    list_music->_model->setHeaderData(1,Qt::Horizontal,QString(""));
//    list_music->_model->setHeaderData(2,Qt::Horizontal,QString("音乐标题"));
//    list_music->_model->setHeaderData(3,Qt::Horizontal,QString("歌手"));
//    list_music->_model->setHeaderData(4,Qt::Horizontal,QString("专辑"));
//    list_music->_model->setHeaderData(5,Qt::Horizontal,QString("时长"));
//    list_music->_model->setHeaderData(6,Qt::Horizontal,QString(""));

//    list_music->setModel(list_music->_model);
    list_music->setColumnWidth(0,20);
    list_music->setColumnWidth(1,20);
    list_music->setColumnWidth(2,200);
    list_music->setColumnWidth(3,100);
    list_music->setColumnWidth(4,100);
    list_music->setColumnWidth(5,100);
    list_music->setColumnWidth(6,0);
    list_music->setColumnHidden(6, true);
//    list_music->setColumnWidth(7,0);
//    list_music->setColumnHidden(7, true);

    //_dbtn = new My_DelegateBtn(this);
    //list_music->setItemDelegateForColumn(1,_dbtn);
    Menu_Init();

}

void Frame_List::Menu_Init()
{
    menu_sheet = new QMenu();
    menu_sheet->setMinimumWidth(180);
    menu_sheet->setStyleSheet("background-color:rgb(255,255,255);");
    QFile styleSheet;
    QString style;
        styleSheet.setFileName(":/Style/style_menu.css");
        if(styleSheet.open(QFile::ReadOnly)) {
            style = styleSheet.readAll();
            styleSheet.close();
            menu_sheet->setStyleSheet(style);
        }
    QAction *act_play = new QAction("播放(&T)",this);
    QAction *act_edit = new QAction("编辑(&N)",this);
    QAction *act_delete = new QAction("删除(&X)",this);
    act_play->setShortcut(Qt::Key_F1);


//    this->connect(act_edit,SIGNAL(triggered()),this,SLOT(hide()));
//    this->connect(act_delete,SIGNAL(triggered()),this,SLOT(show()));
//    this->connect(act_lock,&QAction::triggered,this,&Frame_List::set_lock);
//////////////////////////////////
    menu_sheet->addAction(act_play);
    menu_sheet->addSeparator();     //加入一个分离符
    menu_sheet->addAction(act_edit);
    menu_sheet->addSeparator();     //加入一个分离符
    menu_sheet->addAction(act_delete);

    menu_music = new QMenu();
    menu_music->setMinimumWidth(180);
    menu_music->setStyleSheet("background-color:rgb(255,255,255);");
    menu_music->setStyleSheet(style);
    QAction *act_play1 = new QAction("播放(&T)",this);
    QAction *act_favor1 = new QAction("收藏到歌单(&N)",this);
    QAction *act_copy = new QAction("复制(&X)",this);
    QAction *act_open = new QAction("打开所在文件夹(&T)",this);
    QAction *act_delete2 = new QAction("删除(&X)",this);

    QMenu *menu_list = new QMenu(menu_music);
    menu_list->setMaximumWidth(220);
    act_favor1->setMenu(menu_list);
    My_ListWidget *list = new My_ListWidget(list_sheet, menu_list);
    list->setFont(menu_music->font());
    //list->setMaximumWidth(100);
    QWidgetAction *act_list= new QWidgetAction(menu_list);

    act_list->setDefaultWidget(list);
    menu_list->addAction(act_list);

//    this->connect(act_edit,SIGNAL(triggered()),this,SLOT(hide()));
//    this->connect(act_delete,SIGNAL(triggered()),this,SLOT(show()));
//    this->connect(act_lock,&QAction::triggered,this,&Frame_List::set_lock);

    menu_music->addAction(act_play1);
    menu_music->addSeparator();     //加入一个分离符
    menu_music->addAction(act_favor1);
    menu_music->addAction(act_copy);
    menu_music->addAction(act_open);
    menu_music->addAction(act_delete2);


    connect(list_sheet, &QListWidget::customContextMenuRequested, [=](const QPoint& pos)
    {
        QModelIndex index = list_sheet->indexAt(pos);
        if (index.isValid())
        {
            list_sheet->setCurrentIndex(index);
            menu_sheet->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
        }
    });
    connect(act_delete,&QAction::triggered,[=]()
    {
        delete_sheet(list_sheet->currentRow());
    });
    connect(act_edit,&QAction::triggered,[=]()
    {
        update_sheet();
    });
    connect(list_music, &My_Table::customContextMenuRequested, [=](const QPoint& pos)
    {
        QModelIndex index = list_music->indexAt(pos);
        if (index.isValid())
        {
            list->Init(list_sheet);
            list_music->setCurrentIndex(index);
            menu_music->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
        }
    });

    connect(act_delete2,&QAction::triggered,[=]()
    {
        QModelIndex index = list_music->currentIndex();
        slot_readtToDelete(index);
    });



}

void Frame_List::mouseMoveEvent(QMouseEvent *event)
{
    event->accept();
}
void Frame_List::mousePressEvent(QMouseEvent *event)
{
    event->accept();
}

void Frame_List::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
}

void Frame_List::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->ignore();
}
