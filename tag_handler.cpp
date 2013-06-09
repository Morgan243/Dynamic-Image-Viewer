#include "tag_handler.h"

Tag_Handler::Tag_Handler()
{
    dumpAllExif = true;
}

void Tag_Handler::loadTag(QString fileName)
{
    try
    {
        image = Exiv2::ImageFactory::open(fileName.toStdString());

        image->readMetadata();

        exif_data = image->exifData();
    }
    catch(Exiv2::AnyError& e)
    {
        std::cout<<e.what()<<std::endl;
        clearGPSdata();
    }

    if(exif_data.empty())
    {
        std::cout<<"Exif tag empty!\n";
        return;
    }
}

void Tag_Handler::parseTag()
{
    bool gps_found = false;
    Exiv2::ExifData::const_iterator end = exif_data.end();

    for(Exiv2::ExifData::const_iterator i = exif_data.begin(); i != end; ++i)
    {
        if(dumpAllExif)
        {
            const char* tn = i->typeName();
            std::cout << std::setw(44) << std::setfill(' ') << std::left
                      << i->key() << " "
                      << "0x" << std::setw(4) << std::setfill('0') << std::right
                      << std::hex << i->tag() << " "
                      << std::setw(9) << std::setfill(' ') << std::left
                      << (tn ? tn : "Unknown") << " "
                      << std::dec << std::setw(3)
                      << std::setfill(' ') << std::right
                      << i->count() << "  "
                      << std::dec << i->value()
                      << "\n";
        }

        if(i->key() == "Exif.GPSInfo.GPSLatitude" )
        {
            gps.lat_ratio = QString::fromStdString(i->value().toString());
            convert_degMinSec(gps.lat_ratio, gps.lat_coord);
            gps_found = true;
        }
        else if(i->key() == "Exif.GPSInfo.GPSLongitude" )
        {
            gps.long_ratio = QString::fromStdString(i->value().toString());
            convert_degMinSec(gps.long_ratio, gps.long_coord);
            gps_found = true;
        }
        else if(i->key() == "Exif.GPSInfo.GPSLatitudeRef")
        {
            gps.lat_coord.ref = QString::fromStdString(i->value().toString());
        }
        else if(i->key() == "Exif.GPSInfo.GPSLongitudeRef")
        {
            gps.long_coord.ref = QString::fromStdString(i->value().toString());
        }
    }

    if(!gps_found)
    {
        clearGPSdata(gps);
    }
}

void Tag_Handler::convert_degMinSec(QString ratio, GPS_CoOrd &coord)
{
    Exiv2::RationalValue::AutoPtr rv(new Exiv2::RationalValue);
    rv->read(ratio.toStdString());

    coord.degrees = rv->toFloat(0);
    coord.minutes = rv->toFloat(1);
    coord.seconds = rv->toFloat(2);

}

QString Tag_Handler::getDegMinSec(GPS_CoOrd coord)
{
    QString parsed;
    QString deg = QString::number(coord.degrees);
    QString min = QString::number(coord.minutes);
    QString sec = QString::number(coord.seconds);

    parsed = deg + "  " + min + "  " + sec;
    //parsed.append(QString("%l").arg(coord.degrees));
    //parsed.append(QString("%l  %l  %l").arg(coord.degrees, coord.minutes, coord.seconds));
    return parsed;
}

void Tag_Handler::clearGPSdata()
{
    clearGPSdata(gps);
}

void Tag_Handler::clearGPSdata(GPS_data &gpsData)
{

    gpsData.lat_coord.ref = '-';
    gpsData.lat_coord.degrees = 0.0;
    gpsData.lat_coord.minutes = 0.0;
    gpsData.lat_coord.seconds = 0.0;

    gpsData.long_coord.ref = '-';
    gpsData.long_coord.degrees = 0.0;
    gpsData.long_coord.minutes = 0.0;
    gpsData.long_coord.seconds = 0.0;

    gpsData.lat_ratio = "--NOT SET--";
    gpsData.long_ratio = "--NOT SET--";
}
