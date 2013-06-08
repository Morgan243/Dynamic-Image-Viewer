#ifndef TAG_HANDLER_H
#define TAG_HANDLER_H
#include <exiv2/exiv2.hpp>
#include <iostream>
#include "libexif/exif-data.h"
#include "libexif/exif-ifd.h"
#include "libexif/exif-loader.h"

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
    Exiv2::ExifData exif_data;
    Exiv2::Image::AutoPtr image;

public:
    bool dumpAllExif;
    GPS_data gps;

    Tag_Handler();
    Tag_Handler(bool dumpExif); //true to output entire exif on parse
    void loadTag(QString fileName);
    void parseTag();

    GPS_CoOrd convert_degMinSec(QString ratio);
    QString getDegMinSec(GPS_CoOrd coord);
};

#endif // TAG_HANDLER_H
