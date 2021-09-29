#ifndef MY_MESSAGEBOX_H
#define MY_MESSAGEBOX_H

#include <QDialog>
#include <QObject>
#include <QWidget>

#include <Controls/my_btn.h>
#include <QLabel>
class My_MessageBox : public QDialog
{
    Q_OBJECT
    QString style = "background-color:black; color:white;";
    QSize size_btn;
    QFont f;
public:
    enum ENUM_KIND
    {
        KIND_YESON,
        KIND_OK,
        KIND_ERROR,
        KIND_QUESTION
    };

    My_MessageBox();
    void setKind(ENUM_KIND kind);
    void setText(QString str);
protected:
    My_Btn *btn_close;
    QLabel *content;
    void UiInit();
    void keyPressEvent(QKeyEvent *) override;
};

#endif // MY_MESSAGEBOX_H
