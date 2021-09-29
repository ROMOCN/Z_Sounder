#ifndef TOOL_QT_H
#define TOOL_QT_H

#include <QDateTime>
#include <QImage>
#include <QObject>

namespace TOOL
{
    static QImage color_change(QString path)
    {
        QImage image(path);
        QRgb rgb = image.pixel(image.width()/2,image.height()/2);
        QVector<QRgb> rgbVector = image.colorTable();
        for (int i = 0; i < rgbVector.size(); ++i)
        {
    //        if(rgbVector.at(i) == rgb)
    //        {
    //            QRgb rgb2 = QColor(252,183,120).rgba();//替换的颜色可以是透明的，比如QColor(0,255,0，0)。
    //            image.setColor(i,rgb2);
    //        }
            QRgb rgb2 = QColor(252,183,120).rgba();//替换的颜色可以是透明的，比如QColor(0,255,0，0)。
            image.setColor(i,rgb2);
        }

        return image;
    }

    static QDateTime int_to_time(int t)
    {
        return QDateTime::fromTime_t(t);  //时间戳转为日历时间
    }

    static int time_to_int(QDateTime t)
    {
        return t.toTime_t(); //日历时间转为时间戳
    }


}


#endif // TOOL_QT_H
