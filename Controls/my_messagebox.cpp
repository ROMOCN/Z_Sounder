#include "my_messagebox.h"

#include <QLineEdit>
#include <QTextEdit>

My_MessageBox::My_MessageBox():btn_close(new My_Btn(this))
{
    UiInit();
    size_btn = QSize(80,36);
    label_content = new QLabel(this);
    f.setBold(true);
    f.setFamily("微软雅黑");
    f.setPointSize(12);
}

void My_MessageBox::setKind(My_MessageBox::ENUM_KIND kind)
{
    switch(kind)
    {
    case KIND_OK:
    {
        My_Btn *ok = new My_Btn(this);
        ok->setText("知道了");
        ok->resize(size_btn);
        ok->setStyleSheet(style);
        ok->move((this->width() - ok->width())/2 , this->height() - ok->height() - 40);
        connect(ok, &My_Btn::clicked, [=](){
            this->accept();
            this->close();
        });

    }
    break;
    case KIND_YESON:
    {
        My_Btn *yes = new My_Btn(this);
        My_Btn *no = new My_Btn(this);
        QPixmap map = QPixmap(":/images/btn/btnBack4.png");
        yes->resize(size_btn);
        yes->setStyleSheet(style);
        //yes->setFont(f);
        yes->move(this->width()/2 - yes->width() - 20, this->height() - yes->height() - 40);
        //yes->set_img_formal(map);
        yes->setIconSize(this->size());
        yes->setText("是");
        yes->setFont(f);

        no->setText("否");
        no->resize(size_btn);
        no->setStyleSheet(style);
        //no->set_img_formal(map);
        no->setFont(f);
        no->setIconSize(this->size());
        no->move(this->width()/2 + 20, this->height() - yes->height() - 40);
        connect(yes, &My_Btn::clicked, [=](){
           this->accept();
            this->close();
        });
        connect(no, &My_Btn::clicked, [=](){
           this->reject();
            this->close();
        });


    }
    break;
    case KIND_ERROR:
    {
    }
    break;
    case KIND_QUESTION:
    {
    }
    break;
    case KIND_NEW:
    {
        My_Btn *ok = new My_Btn(this);
        ok->setText("接受");
        ok->resize(size_btn);
        ok->setStyleSheet(style);
        ok->move((this->width() - ok->width())/2 , this->height() - ok->height() - 40);
        connect(ok, &My_Btn::clicked, [=](){
           this->accept();
            this->close();
        });
        label_content->move((this->width() - label_content->width())/2 , (this->height() - label_content->height())/2 - 50);
        QLineEdit *edit = new QLineEdit(this);
        edit->resize(this->width()/3, 14);
        edit->move((this->width() - edit->width())/2, (this->height() - edit->height())/2 +10);
        connect(edit, &QLineEdit::textChanged, [=]()
        {
            *value = edit->text();
        });

    }
    break;
    }
}

void My_MessageBox::setValue(QString *str)
{
    value = str;
}

void My_MessageBox::setText(QString str)
{
    label_content->setText(str);
    QFontMetrics fm(f);
    QRect rec = fm.boundingRect( str);
    label_content->resize(rec.width() + 20, rec.height() + 4);
    label_content->setFont(f);
    label_content->setAlignment(Qt::AlignCenter);
    label_content->move((this->width() - label_content->width())/2 , (this->height() - label_content->height())/2 - 30);
}

void My_MessageBox::closeEvent(QCloseEvent *)
{
    //this->reject();
}



void My_MessageBox::UiInit()
{
    this->resize(520,240);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowModality(Qt::ApplicationModal);
    btn_close->resize(20,20);
    btn_close->set_img_formal(":/images/close_black.png");
    btn_close->move(this->width() - 10 - btn_close->width(), 10);
    connect(btn_close, &My_Btn::clicked, this, &My_MessageBox::close);
}

void My_MessageBox::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if(key == Qt::Key::Key_Enter)
    {
        this->accept();
         this->close();
    }else if(key  == Qt::Key::Key_Escape)
    {
        this->reject();
        this->close();
    }
}
