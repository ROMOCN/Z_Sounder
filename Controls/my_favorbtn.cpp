#include "my_favorbtn.h"


My_FavorBtn::My_FavorBtn(QWidget *parent):My_Btn(parent)
{

    pix_favor = QPixmap(":/images/favor.png");
    pix_unfavor = QPixmap(":/images/unfavor.png");
    this->set_img_formal(pix_unfavor.toImage());
    favor = false;
    update();
}

void My_FavorBtn::mouseReleaseEvent(QMouseEvent *event)
{

    if(favor)
    {
        this->set_img_formal(pix_unfavor.toImage());
    }else
    {
        this->set_img_formal(pix_favor.toImage());
    }
    favor = !favor;
    emit signal_favor_changed(favor);
}
