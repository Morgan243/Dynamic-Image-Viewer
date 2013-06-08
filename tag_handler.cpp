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
    }

    if(exif_data.empty())
    {
        std::cout<<"Exif tag empty!\n";
        return;
    }
}

void Tag_Handler::parseTag()
{
    Exiv2::ExifData::const_iterator end = exif_data.end();

    std::cout<<"HI\n";

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
            gps.lat_coord = convert_degMinSec(gps.lat_ratio);
        }
        else if(i->key() == "Exif.GPSInfo.GPSLongitude" )
        {
            gps.long_ratio = QString::fromStdString(i->value().toString());
            gps.long_coord = convert_degMinSec(gps.long_ratio);
        }

    }
}

GPS_CoOrd Tag_Handler::convert_degMinSec(QString ratio)
{
    GPS_CoOrd converted;
    Exiv2::RationalValue::AutoPtr rv(new Exiv2::RationalValue);
    rv->read(ratio.toStdString());

    converted.degrees = rv->toFloat(0);
    converted.minutes = rv->toFloat(1);
    converted.seconds = rv->toFloat(2);

    return converted;
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
