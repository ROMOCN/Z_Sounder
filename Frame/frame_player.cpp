#include "frame_player.h"

#include <QString>

Frame_Player::Frame_Player(QWidget *parent) : QWidget(parent)
{
    this->resize(1200,100);
    this->setMinimumHeight(100);
    this->setMinimumWidth(1000);
    this->setAcceptDrops(true);
    UI_Init();
    QString path = "D:/ALLproject/QT/Z_Sounder/music/关大洲-水龙吟.mp3";
    music = new Tool_Music_Qt();
    music->Music_Inital(path);
    music->Music_Set_Voice(50);
    music->Music_Set_Rate(1);
    thread_MInfo =new std::thread(&Frame_Player::Music_Thread,this);
    thread_MInfo->detach();
    this->setObjectName("player");
    this->setWindowFlag(Qt::FramelessWindowHint);
    Signal_Init();
}
void Frame_Player::UI_Init()
{
    box = new QDoubleSpinBox(this);
    box->setMaximum(2.0);
    box->setMinimum(0.5);
    box->setSingleStep(0.25);
    box->setValue(1.0);
    box->setFocus(Qt::FocusReason::NoFocusReason);
    box->move(600,40);

    slider = new My_Slider(this);
    slider->setOrientation(Qt::Horizontal);
    slider->setSingleStep(1);  // 步长
    slider->setMinimum(0);
    slider->resize(480,20);
    slider->setMinimumSize(480,20);

//    btn_stop = new My_Btn(this);
//    btn_stop->move(220,35);
//    btn_stop->resize(30,30);
//    btn_stop->set_img_formal(":/images/stop.png");

    btn_before = new My_Btn(this);
    btn_before->resize(20,20);
    btn_before->set_img_formal(":/images/before.png");

    btn_play = new My_Btn(this);
    btn_play->resize(30,30);
    btn_play->set_img_formal(":/images/play.png");

    btn_next = new My_Btn(this);
    btn_next->resize(20,20);
    btn_next->set_img_formal(":/images/next.png");

    btn_mode = new My_BtnPlanMode(this);
    btn_mode->resize(30,20);

//    picture = new My_Btn(this);
//    picture->resize(40,40);
//    picture->move(this->width() - 60, 20);
//    picture->setText("666666");

    //num_current = new QLCDNumber(this);
    num_len = new QLabel(this);
    //num_len->move(100,10);
    num_len->resize(40,20);
    num_len->setText("00:00");
    num_current = new QLabel(this);
    //num_current->move(10,20);
    num_current->resize(40,20);
    num_current->setText("00:00");


    label_curTitle = new QLabel(this);
    label_curTitle->setStyleSheet("color:black");
    label_curPerFormer = new QLabel(this);
    label_curPerFormer->setStyleSheet("color:black");


}

void Frame_Player::Signal_Init()
{
    connect(btn_play,&My_Btn::clicked, this, &Frame_Player::Play);
    //connect(btn_stop,&My_Btn::clicked, this, &Frame_Player::Stop);
    connect(box, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=](double value)
        {
            music->Music_Set_Rate(value);
        });

    connect(music, &Tool_Music_Qt::Music_Signal_Length, this , &Frame_Player::slot_set_len);
    connect(slider, &My_Slider::signal_mouse_press, this, &Frame_Player::slot_slider_press );
    connect(slider, &My_Slider::signal_mouse_release, this, &Frame_Player::slot_slider_release );
    connect(music, &Tool_Music_Qt::Signal_State_Changed, this, &Frame_Player::slot_state_change );
    connect(btn_before, &My_Btn::clicked, this, &Frame_Player::Previous);
    connect(btn_next, &My_Btn::clicked, this, &Frame_Player::Next);
    connect(music, &Tool_Music_Qt::Signal_MediaChanged, this, &Frame_Player::slot_currentMediaChanged);
    connect(btn_mode, &My_BtnPlanMode::signal_modeChanged, [=](int mode){
        //QMediaPlaylist::Loop
        _list->setPlaybackMode((QMediaPlaylist::PlaybackMode)mode);

    });
}

void Frame_Player::resizeEvent(QResizeEvent *event)
{
    slider->move( (this->width() - slider->width())/2 , this->height() - 30);

    btn_play->move( (this->width() - btn_play->width())/2 , slider->y() - btn_play->height() - 10);
    btn_before->move(btn_play->x() -  btn_before->width() - 40, btn_play->y() + 5);
    btn_next->move(btn_play->x() + btn_play->width() + 40 , btn_play->y() + 5);
    num_current->move(slider->x() - num_current->width() - 20, slider->y());
    num_len->move(slider->x() + slider->width() + 20, slider->y());

    label_curTitle->move(20, (this->height()/2) - label_curTitle->height() + 10);
    label_curPerFormer->move(20, this->height()/2 + 10);

    box->move(this->width() - box->width() - 40, (this->height() - box->height()) - 10);
    btn_mode->move(box->x() - btn_mode->width() - 100, (this->height() - box->height()) - 10);


}
void Frame_Player::Init(QString path)
{
    music->Music_Inital(path);
}

void Frame_Player::Play()
{
    if(music->status != 1)
    {
        music->Music_Play();

    }else
    {
        music->Music_Pause();
    }

}

void Frame_Player::Stop()
{
    music->Music_Stop();
    slider->setValue(0);

}

void Frame_Player::Next()
{
    if(!_list->isEmpty())
    {
        _list->setCurrentIndex(_list->nextIndex());
        //_list->next();
    }

}

void Frame_Player::Previous()
{
    if(!_list->isEmpty())
        _list->previous();
}

void Frame_Player::set_list(QMediaPlaylist *list)
{
    _list = NULL;
    this->_list = list;
    music->Music_SetList(list);
    this->Play();
}

void Frame_Player::play_list(QMediaPlaylist *list, int musicId)
{
    _list = nullptr;
    this->_list = list;
    music->Music_SetList(list);
    _list->setCurrentIndex(musicId);
    int i = _list->mediaCount();
    this->Play();

//    for(int i = 0 ; i<list->mediaCount(); i++)
//    {
//        QUrl url= list->media(i).canonicalUrl();
//        QString str = url.url();
//        qDebug()<<str;
//    }
}

int Frame_Player::Music_Thread()
{
    QString tempPath = "";
    while(music!=nullptr&&music->status != -1)
    {
        if(music->status == 1)
        {
            if(!press_slider)
            {
                int cur = music->Music_Get_Current_Position();
                slider->setValue(cur/MUSIC_DOCK);
                set_time(cur, *num_current);

                int meida_state = music->Music_Get_Media_Status();
                 //qDebug()<<meida_state;
                if(meida_state == QMediaPlayer::MediaStatus::LoadedMedia)
                {
                    qDebug()<<"LoadedMedia";
                }
                //music->Music_Get_Info();

                if(!_list->isEmpty())
                {
                    QString path = _list->media(_list->currentIndex()).canonicalUrl().toString();
                    if(tempPath != path)
                    {
                        _entity.MediaInfo(path);
                        label_curTitle->setText(_entity.Title);
                        label_curPerFormer->setText(_entity.Performer);
                        QFont f;
                        f.setFamily("微软雅黑");
                        f.setPointSize(12);

                        QFontMetrics fm(f);
                        label_curTitle->setFont(f);
                        label_curPerFormer->setFont(f);
                        label_curTitle->resize( fm.boundingRect(label_curTitle->text()).width() + 10, 20);
                        label_curPerFormer->resize( fm.boundingRect(label_curPerFormer->text()).width() + 10 , 20);

                        tempPath = path;
                    }
                }
            }

        }
//        int status = music->Music_Get_Status();
//        qDebug()<<"state:"<<status;
//        QPoint pos_mouse = QCursor::pos(), pos_widget = this->pos();
//        qDebug()<<"窗口位置:"<<pos_widget;//窗口位置
//        qDebug()<<"鼠标位置:"<<pos_mouse; //鼠标位置

        std::this_thread::sleep_for(std::chrono::milliseconds(MUSIC_DOCK));
    }
    return 0;
}





void Frame_Player::dragMoveEvent(QDragMoveEvent *event)
{
    event->ignore();
    event->acceptProposedAction();
}

void Frame_Player::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->ignore();
}

void Frame_Player::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();

}

void Frame_Player::dropEvent(QDropEvent *event)
{
    event->ignore();
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty())
        return ;
    for(auto url : urls)
    {
        QString path = url.toLocalFile();
        QString format = path.mid( path.lastIndexOf('.') + 1, path.length() - path.lastIndexOf('.'));
        if(format == "mp3")
        {
            music->Music_Inital(path);
            emit signal_add_music(path);
//            MP3::MP3Header header = MP3::MP3Header();
//            MP3::MP3INFO INFO = header.GetAllInfo(path.toStdWString().c_str(),1);

//            QPixmap pixmap = INFO.picture;
//            QPalette palette(this->palette());
//            palette.setBrush(QPalette::Background, QBrush(pixmap));
//            this->setPalette(palette);
        }

    }
    qDebug()<<"end drop";
}

void Frame_Player::paintEvent(QPaintEvent *event)
{

//    QPainter p(this);
//    p.begin(this);
//    p.setBrush(QBrush(QColor(255,255,255,200)));
//    p.drawRect(10, 10, this->width() - 10, this->height() - 10);
//    p.save();
}




void Frame_Player::slot_set_len(qint64 len)
{
    slider->setMaximum( len/MUSIC_DOCK );
    set_time(len, *num_len);
}

void Frame_Player::slot_slider_press()
{
    press_slider = true;
}

void Frame_Player::slot_slider_release()
{
    int value = slider->value();
    music->Music_Set_Position(value * MUSIC_DOCK);
    press_slider = false;
}

void Frame_Player::slot_state_change(qint64 state)
{
    switch (state)
    {
    case 0:
        btn_play->set_img_formal(":/images/play.png");
        break;
    case 2:
        btn_play->set_img_formal(":/images/play.png");
        break;
    case 1:
        btn_play->set_img_formal(":/images/pause.png");
        break;
    }
}

void Frame_Player::slot_rate_change(double value)
{
    music->Music_Set_Rate(value);
}

void Frame_Player::set_time(int time, QLabel &label)
{
    int t = time/1000;
    int seconds = t % 60;
    int minutes = 0;
    int hour = 0;
    if( t / 60 > 0)
    {
        minutes = t / 60;
        if(minutes / 60 > 0)
        {
            hour = minutes / 60;
        }
    }



    label.setText(QString("%1:%2").arg(minutes, 2, 10, QLatin1Char('0')).arg(seconds, 2, 10, QLatin1Char('0')));



}

void Frame_Player::slot_currentMediaChanged(QMediaContent con)
{
    label_curTitle->setText( con.canonicalUrl().url() );
    music->Music_Get_Info();
}

