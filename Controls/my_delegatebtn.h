#ifndef MY_DELEGATEBTN_H
#define MY_DELEGATEBTN_H

#include <QObject>
#include <QWidget>
#include <QItemDelegate>
class My_DelegateBtn : public QItemDelegate
{
    Q_OBJECT
public:
    explicit My_DelegateBtn(QObject *parent = nullptr);
    //重写绘图事件
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool eventFilter(QObject *object, QEvent *event) override;
    //重写事件过滤
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
signals:
    void signal_favorChanged(const QModelIndex &index, bool favorState);
private:
    QMap<QModelIndex, QStyleOptionButton*> _btns;
    QMap<QModelIndex, bool> _btnState;

};

#endif // MY_DELEGATEBTN_H
