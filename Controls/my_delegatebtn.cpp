#include "my_delegatebtn.h"
#include <QApplication>
#include <QCheckBox>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include "my_btn.h"
const int CONTROL_COLUMN = 1;//按键所在列
My_DelegateBtn::My_DelegateBtn(QObject *parent):QItemDelegate(parent)
{
    this->installEventFilter(this);
}
void My_DelegateBtn::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionButton* box = _btns.value(index);//在特定的索引处绘制按键，这个变量临时存储绘制的按键信息
    //如果为按键列绘制按键
    if (index.column() == CONTROL_COLUMN)
    {
        //非常重要！该条件最好为判断索引与需要代理的列！因为这样绘制的按键会随滑动条进行重绘！
        box = new QStyleOptionButton();//绘制存button的参数的对象
        box->features = QStyleOptionButton::None;//设置为pushButton类型，可以按下
        box->rect = option.rect.adjusted(10, 5, -5, -5);//绘制尺寸
        QPixmap mapUnFavor =  QPixmap(":/images/unfavor.png");
        QPixmap mapFavor =  QPixmap(":/images/favor.png");
        box->iconSize = QSize(20,20);
        box->icon = QIcon(mapUnFavor);
        (const_cast<My_DelegateBtn *>(this))->_btns.insert(index, box);//将绘制的按键放入Qmap
    }
    painter->save();
    //被选中时按原先背景绘制
    if (option.state & QStyle::State_Selected)
    {
        painter->fillRect(option.rect, option.palette.highlight());
    }
    painter->restore();
    QApplication::style()->drawControl(QStyle::CE_CheckBoxLabel, box, painter);//CE_CheckBoxLabel
}

QWidget *My_DelegateBtn::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QPushButton *btn = new QPushButton(parent);
    return btn;
}

void My_DelegateBtn::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QPushButton *cob = static_cast<QPushButton *>(editor);
    QPixmap mapFavor =  QPixmap(":/images/favor.png");
    cob->setIconSize(QSize(20,20));
    cob->setIcon(QIcon(mapFavor));

}

void My_DelegateBtn::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // 将编辑器设置为矩形属性
        editor->setGeometry(option.rect);
}



bool My_DelegateBtn::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* e =(QMouseEvent*)event;
    }
    if (event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* e =(QMouseEvent*)event;
    }
    return true;

}


bool My_DelegateBtn::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* e =(QMouseEvent*)event;
        //确定鼠标点击的位置在画的按键中
        if (option.rect.adjusted(1, 1, -1, -1).contains(e->x(), e->y()) && _btns.contains(index))
        {
             QStyleOptionButton* button = _btns.value(index);
            button->state |= QStyle::State_Sunken;
            QPixmap map =  QPixmap(":/images/favor.png");
            button->icon = QIcon(map);
            button->iconSize = QSize(20,20);
        }
    }

    if (event->type() == QEvent::MouseButtonRelease)
    {

        QMouseEvent* e =(QMouseEvent*)event;

    }
    return false;
}
