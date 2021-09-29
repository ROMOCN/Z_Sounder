#include "my_listwidget.h"

My_ListWidget::My_ListWidget(QWidget *parent):QListWidget(parent)
{
    //this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->setVerticalScrollMode(ScrollMode::ScrollPerItem);
    this->setStyleSheet("background-color:transparent;font:22px;font-color:rgba(255,255,255,200);");
    this->setFrameShape(QListWidget::NoFrame);
    menuInit();
    connect(this, &My_ListWidget::itemClicked, this, &My_ListWidget::slot_itemClick);
}

My_ListWidget::My_ListWidget(My_ListWidget *list, QWidget *parent):QListWidget(parent)
{
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->setVerticalScrollMode(ScrollMode::ScrollPerItem);

    this->setStyleSheet("background-color:transparent;font:22px;font-color:rgba(255,255,255,200);");
    this->setFrameShape(QListWidget::NoFrame);
    menuInit();
    connect(this, &My_ListWidget::itemClicked, this, &My_ListWidget::slot_itemClick);

    Init(list);
}


void My_ListWidget::Init(My_ListWidget *list)
{
    for(int i = 0; i < list->count(); i++)
    {
         QListWidgetItem *item = new QListWidgetItem();
         item->setText(list->item(i)->text());
         item->setIcon(list->item(i)->icon());
         QString str = item->text();
         this->addItem(item);
    }
}

void My_ListWidget::slot_itemClick(QListWidgetItem *item)
{

}

void My_ListWidget::menuInit()
{


}
//QListWidget{ //去掉选中时的虚线框
//    outline:0px;
//}
//border-bottom-left-radius:10; border-bottom-right-radius:10;
