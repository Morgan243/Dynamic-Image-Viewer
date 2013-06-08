#ifndef TAG_HANDLER_H
#define TAG_HANDLER_H
#include <exiv2/exiv2.hpp>
#include <iostream>
#include "libexif/exif-data.h"
#include "libexif/exif-ifd.h"
#include "libexif/exif-loader.h"

#include <string>
#include <QString>
struct GPS_data
{
    QString lat_ratio;
    QString long_ratio;
};

class Tag_Handler
{
private:
    Exiv2::ExifData exif_data;
    Exiv2::Image::AutoPtr image;

public:
    GPS_data gps;

    Tag_Handler();
    void loadTag(QString fileName);
    void parseTag();
    GPS_data getGPS();
};

#endif // TAG_HANDLER_H
