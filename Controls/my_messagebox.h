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
    My_Btn *btn_close;
    QLabel *label_content;
    QString *value = nullptr;
public:
    enum ENUM_KIND
    {
        KIND_YESON,
        KIND_OK,
        KIND_ERROR,
        KIND_QUESTION,
        KIND_NEW
    };

    My_MessageBox();
    void setKind(ENUM_KIND kind);
    void setValue(QString *str);
    void setText(QString str);
    void closeEvent(QCloseEvent *) override;
protected:
    void UiInit();
    void keyPressEvent(QKeyEvent *) override;
};

#endif // MY_MESSAGEBOX_H
