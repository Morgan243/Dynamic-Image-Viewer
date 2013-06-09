#ifndef TAG_HANDLER_H
#define TAG_HANDLER_H
#include <exiv2/exiv2.hpp>
#include <iostream>
#include "libexif/exif-data.h"
#include "libexif/exif-ifd.h"
#include "libexif/exif-loader.h"

#include <QVector>
#include <QPointF>
#include <string>
#include <QString>

struct GPS_CoOrd
{
    QString ref;

    float degrees, minutes, seconds;
};

struct GPS_data
{
    QString lat_ratio;
    GPS_CoOrd lat_coord;

    QString long_ratio;
    GPS_CoOrd long_coord;
};

class Tag_Handler
{
private:
    Exiv2::ExifData *exif_data;
    Exiv2::Image::AutoPtr image;

public:
    bool dumpAllExif;
    GPS_data gps;

    Tag_Handler();
    Tag_Handler(bool dumpExif); //true to output entire exif on parse
    void loadTag(QString fileName);
    void parseTag();

    void convert_degMinSec(QString ratio, GPS_CoOrd &coord);
    QString getDegMinSec(GPS_CoOrd coord);

    void clearGPSdata();
    void clearGPSdata(GPS_data &gpsData);

    void clearComment(QString fileName);
    QVector<QPointF> readPointsFromComment(QString fileName);
    void addPointToComment(QString fileName, qreal x, qreal y);
    void removePointInComment(QString fileName, qreal x, qreal y);

    void addPointToComment(QString fileName, QPointF point);
    void removePointInComment(QString fileName, QPointF point);

};

#endif // TAG_HANDLER_H
