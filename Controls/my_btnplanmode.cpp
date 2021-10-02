#include "my_btnplanmode.h"

My_BtnPlanMode::My_BtnPlanMode(QWidget *parent) : My_Btn(parent)
{

    map_random = QPixmap(":/images/playMode/random.png");
    map_curloop = QPixmap(":/images/playMode/curloop.png");
    map_listloop = QPixmap(":/images/playMode/listloop.png");
    map_sequential = QPixmap(":/images/playMode/sequential.png");
    currentMode = 1;
    maps.push_back(map_sequential);
    maps.push_back(map_listloop);
    maps.push_back(map_curloop);
    maps.push_back(map_random);

    this->setIcon(maps[0]);

}

void My_BtnPlanMode::mousePressEvent(QMouseEvent *event)
{

}

void My_BtnPlanMode::mouseReleaseEvent(QMouseEvent *event)
{
    currentMode++;
    if(currentMode == 5)
    {
        currentMode = 1;
    }
    this->setIcon(maps[currentMode - 1]);
    emit signal_modeChanged(currentMode);
}
