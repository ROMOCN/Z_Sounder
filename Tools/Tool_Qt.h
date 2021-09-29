#ifndef TOOL_QT_H
#define TOOL_QT_H

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



}


#endif // TOOL_QT_H
