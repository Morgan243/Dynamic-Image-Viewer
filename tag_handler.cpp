#include "tag_handler.h"

Tag_Handler::Tag_Handler()
{
}

void Tag_Handler::loadTag(QString fileName)
{
    image = Exiv2::ImageFactory::open(fileName.toStdString());

    image->readMetadata();

    exif_data = image->exifData();

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

        if(i->key() == "Exif.GPSInfo.GPSLatitude" )
            //gps.lat_ratio.fromStdString(i->value().toString());
            gps.lat_ratio = QString::fromStdString(i->value().toString());
        else if(i->key() == "Exif.GPSInfo.GPSLongitude" )
            gps.long_ratio = QString::fromStdString(i->value().toString());

    }

    std::cout<<"LAT: "<<qPrintable(gps.lat_ratio)<<std::endl;
    std::cout<<"LONG: "<<qPrintable(gps.long_ratio)<<std::endl;
}
