#include "my_table.h"

#include <QAbstractItemView>
#include <QPainter>

My_Table::My_Table(QWidget *parent) :QTableView(parent), _delegateBtn(new My_DelegateBtn(this))
{
    //this->setItemDelegateForColumn(1, _delegateBtn);
    ui_init();
    row_move_init();
    this->viewport()->setAttribute(Qt::WA_Hover,true);
    this->viewport()->installEventFilter(this);
    this->viewport()->setMouseTracking(true);
    connect(this, SIGNAL(clicked(const QModelIndex &index)), this, SLOT(itemClicked(const QModelIndex &index)));
    connect(this, SIGNAL(clicked(const QModelIndex &index)), _delegateBtn, SLOT(slot_rowClicked(const QModelIndex &index)));
    menuInit();
}
void My_Table::add_List(int sheetId)
{
    QMediaPlaylist *list = nullptr;
    list = new QMediaPlaylist(this);
    _playlists.insert(std::map<int,QMediaPlaylist*>::value_type(sheetId, list));

}

void My_Table::add_model(int sheetId)
{
    add_List(sheetId);

    QStandardItemModel *model = new QStandardItemModel();
    model->setColumnCount(8);
    model->setHeaderData(0,Qt::Horizontal,QString(""));
    model->setHeaderData(1,Qt::Horizontal,QString(""));
    model->setHeaderData(2,Qt::Horizontal,QString("音乐标题"));
    model->setHeaderData(3,Qt::Horizontal,QString("歌手"));
    model->setHeaderData(4,Qt::Horizontal,QString("专辑"));
    model->setHeaderData(5,Qt::Horizontal,QString("时长"));
    model->setHeaderData(6,Qt::Horizontal,QString(""));
    model->setHeaderData(7,Qt::Horizontal,QString(""));

    _models.insert(std::map<int,QStandardItemModel*>::value_type(sheetId, model));
}

void My_Table::set_model(int sheetId)
{
    if(sheetId >= 0 && !_models.empty())
    {
        currentModelId = sheetId;
        QStandardItemModel *model = _models.find(sheetId)->second;
        this->setModel(model);
    }
}


QStandardItemModel *My_Table::current_model()
{

}


QStandardItemModel *My_Table::get_model(int sheetId)
{
    return  _models.find(sheetId)->second;
}

QMediaPlaylist *My_Table::get_playList(int sheetId)
{
    return  _playlists.find(sheetId)->second;
}

void My_Table::delete_playList(int sheetId)
{
    QMediaPlaylist * list = get_playList(sheetId);
    _playlists.erase(sheetId);
    list = nullptr;
    delete list;
}

void My_Table::delete_model(int sheetId)
{
    QStandardItemModel * model = get_model(sheetId);
    _models.erase(sheetId);
    model = nullptr;
    delete model;


}

bool My_Table::eventFilter(QObject *obj, QEvent *event)
{
    event->ignore();
    if(event->type() == QEvent::MouseButtonPress)
    {
        int i = 0;

        QMouseEvent *mouse = (QMouseEvent*)event;
        if(mouse->button() == Qt::RightButton)
        {
            //menu_music->show();
        }else
        {
            //menu_music->hide();
        }
    }
    if(event->type() == QEvent::HoverEnter)
    {
        int i = 0;
        this->selectedIndexes();

    }
    if(event->type() == QEvent::MouseButtonDblClick)
    {
        QModelIndex index = currentIndex();
        emit doubleClicked(index);
    }
    return QObject::eventFilter(obj,event);
}


void My_Table::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{

    qDebug()<<"currentChanged";
}

void My_Table::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{

    if(this->selectedIndexes().count()>0)
    {
//        QModelIndex index = this->selectedIndexes().first();
//        this->selectRow(index.row());
//        this->selectionModel();
//        this->setCurrentIndex(index);
        //setSelectionModel();
    }

    qDebug()<<"selectionChanged";
}

void My_Table::itemClicked(const QModelIndex &index)
{
    if(index.isValid())
    {
        QString str = index.data().toString();
        qDebug()<<str;
    }
}

void My_Table::mousePressEvent(QMouseEvent *event)
{
    event->ignore();
    QModelIndex index = indexAt(event->pos());
    this->setCurrentIndex(index);
    this->selectRow(index.row());
    this->setStyleSheet(_style);

    if(index.isValid() && currentModelId >=0 && _models.size()>0 && index.row() >= 0 && index.column() == 1)
    {
        QStandardItemModel *curModel = _models.find(currentModelId)->second;
        if(curModel->item(index.row(), 1)->toolTip() == "添加收藏")
        {
            curModel->item(index.row(), 1)->setIcon(QPixmap(":/images/favor.png"));
            curModel->item(index.row(), 1)->setToolTip("取消收藏");
            signal_favorChanged(true, index.row());

        }else if(curModel->item(index.row(), 1)->toolTip() == "取消收藏")
        {
            curModel->item(index.row(), 1)->setIcon(QPixmap(":/images/unfavor.png"));
            curModel->item(index.row(), 1)->setToolTip("添加收藏");

            signal_favorChanged(false, index.row());

        }
//        for(int i = 0; i < curModel->columnCount(); i++)
//        {
//            curModel->item(index.row(), i)->setBackground(QColor(Qt::white));
//            curModel->item(index.row(), i)->setForeground(QColor(Qt::black));
//        }
    }

}

void My_Table::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();

}

void My_Table::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();
    QModelIndex index = indexAt(event->pos());
    this->setCurrentIndex(index);
}

void My_Table::menuInit()
{

//    connect(this, &My_Table::customContextMenuRequested, [=](const QPoint& pos)
//    {
//        menu_music->exec( QCursor::pos() );
//    });

}

void My_Table::ui_init()
{
    this->setStyleSheet(_style);
    this->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color:transparent; color:black; }");
    this->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    //this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setFrameShape(QTableWidget::NoFrame);
    setFocusPolicy(Qt::NoFocus);
    //需要打开右键菜单属性,则必须设置
    setContextMenuPolicy(Qt::CustomContextMenu);
    //设置最后一栏自适应长度
    this->horizontalHeader()->setStretchLastSection(true);
    //开启交替行背景色,在设置style为交替颜色时必须开启
    setAlternatingRowColors(true);
    //设置列不能拖动
    horizontalHeader()->setDisabled(true);
    //设置行不能拖动
    verticalHeader()->setDisabled(true);
    //setWindowFlags(Qt::FramelessWindowHint);  /* 去除标题栏 */
    setShowGrid(false); /* 去除QTableWidget组件中的线 */
    this->verticalHeader()->hide();  //隐藏行标
    //设置表格的单元为只读属性，即不能编辑
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void My_Table::row_move_init()
{
//    this->setDragEnabled(true);
//    this->setDragDropMode(QAbstractItemView::InternalMove);
//    this->setDefaultDropAction(Qt::MoveAction);
    //    this->setDragDropOverwriteMode(true);
}

//"QTableWidget::Item{border:0px solid rgb(255,0,0);border-bottom:1px solid rgb(255,0,0);}"
