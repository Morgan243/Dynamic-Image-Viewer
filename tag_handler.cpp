#include "tag_handler.h"

Tag_Handler::Tag_Handler()
{
    dumpAllExif = true;
    gps_from_filename = false;
}

bool Tag_Handler::loadTag(QString fileName)
{

    //are we avoiding exif data?
    if(gps_from_filename)
        gps = getGPS_fromFilename(fileName);

    try
    {
        image = Exiv2::ImageFactory::open(fileName.toStdString());

        image->readMetadata();

        exif_data = &image->exifData();

        if(exif_data->empty())
        {
            std::cout<<"Exif tag empty!\n";
            return false;
        }
    }
    catch(Exiv2::AnyError& e)
    {
        std::cout<<e.what()<<std::endl;
        clearGPSdata();

        return false;
    }

    return true;
}

void Tag_Handler::parseTag()
{
    if(!gps_from_filename)
        gps_found = false;

    Exiv2::ExifData::const_iterator end = exif_data->end();

    for(Exiv2::ExifData::const_iterator i = exif_data->begin(); i != end; ++i)
    {
        if(0)//(dumpAllExif)
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

        if(i->key() == "Exif.GPSInfo.GPSLatitude" && !gps_from_filename)
        {
            gps.lat_ratio = QString::fromStdString(i->value().toString());
            convert_degMinSec(gps.lat_ratio, gps.lat_coord);
            gps.lat_coord.dec_form = getDecForm(gps.lat_coord, gps.lat_coord.ref);
            gps_found = true;
        }
        else if(i->key() == "Exif.GPSInfo.GPSLongitude" && !gps_from_filename)
        {
            gps.long_ratio = QString::fromStdString(i->value().toString());
            convert_degMinSec(gps.long_ratio, gps.long_coord);
            gps.long_coord.dec_form = getDecForm(gps.long_coord, gps.long_coord.ref);

            gps_found = true;
        }
        else if(i->key() == "Exif.GPSInfo.GPSLatitudeRef" && !gps_from_filename)
        {
            gps.lat_coord.ref = QString::fromStdString(i->value().toString());
        }
        else if(i->key() == "Exif.GPSInfo.GPSLongitudeRef" && !gps_from_filename)
        {
            gps.long_coord.ref = QString::fromStdString(i->value().toString());
        }
        else if(i->key() == "Exif.Photo.UserComment")
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

            //need to reset user comment if hasn't been set before
            if(i->count() == 264)
            {
                 (*exif_data)["Exif.Photo.UserComment"] = "";
                  image->writeMetadata();
            }
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

qreal Tag_Handler::getDecForm(GPS_CoOrd CoOrd, QString ref)
{
    qreal dec_form = CoOrd.degrees + (CoOrd.minutes/60.0) + (CoOrd.seconds/3600.0);

    if((ref == "W") || (ref == "S"))
        dec_form *= -1;

    return dec_form;
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

void Tag_Handler::clearComment(QString fileName)
{
    //load the file metadata
    loadTag(fileName);

    //reset the comment section
    (*exif_data)["Exif.Photo.UserComment"] = "";

    //write it back to tag
    image->writeMetadata();
}

QVector<QPointF> Tag_Handler::readPointsFromComment(QString fileName)
{
    //load up the images tag
    loadTag(fileName);

    //get the current contents
    QString comment = QString::fromStdString((*exif_data)["Exif.Photo.UserComment"].toString());

    int left_paren, comma, right_paren = 0;
    float temp_x, temp_y;
    QVector<QPointF> points;

    //while we are working from a valid left paren
    while((left_paren = comment.toStdString().find('(', right_paren)) != std::string::npos)
    {
        //find comma after left paren
        comma = comment.toStdString().find(',', left_paren);

        //find right paren after comma
        right_paren = comment.toStdString().find(')', comma);

        //grab x value out
        temp_x = atof(comment.toStdString().substr(left_paren+1, comma-1).c_str());

        //grab y value out
        temp_y = atof(comment.toStdString().substr(comma+1, right_paren-1).c_str());

        points.push_back(QPointF(temp_x,temp_y));

    }

    return points;
}

void Tag_Handler::addPointToComment(QString fileName, qreal x, qreal y)
{
    //load up the images tag
    loadTag(fileName);

    //get the current contents
    QString comment = QString::fromStdString((*exif_data)["Exif.Photo.UserComment"].toString());

    //append the new point to current contents
    comment = comment + "(" + QString::number(x) +"," + QString::number(y) + ")";

    std::cout<<"Updating comment to: "<< qPrintable(comment);

    //set updated contents
    (*exif_data)["Exif.Photo.UserComment"]
          = comment.toStdString().c_str();

    //write back contents
    image->writeMetadata();
}


void Tag_Handler::removePointInComment(QString fileName, qreal x, qreal y)
{
    //load up the images tag
    loadTag(fileName);

    //get the current contents
    QString comment = QString::fromStdString((*exif_data)["Exif.Photo.UserComment"].toString());

    if(!comment.contains("("+QString::number(x) + "," + QString::number(y)+")"))
    {
        std::cout<<"Could not remove point ("<<x<<","<<y<<"), it doesnt exist!"<<std::endl;
    }
    else
    {
        comment.remove("("+QString::number(x) + "," + QString::number(y)+")");
        std::cout<<"Removing ("<<x<<","<<y<<")"<<std::endl;

        //set updated contents
        (*exif_data)["Exif.Photo.UserComment"]
              = comment.toStdString().c_str();

        //write back contents
        image->writeMetadata();
    }
}

void Tag_Handler::addPointToComment(QString fileName, QPointF point)
{
    //load up the images tag
    loadTag(fileName);

    //get the current contents
    QString comment = QString::fromStdString((*exif_data)["Exif.Photo.UserComment"].toString());

    //append the new point to current contents
    comment = comment + "(" + QString::number(point.x()) +"," + QString::number(point.y()) + ")";

    std::cout<<"Updating comment to: "<< qPrintable(comment);

    //set updated contents
    (*exif_data)["Exif.Photo.UserComment"]
          = comment.toStdString().c_str();

    //write back contents
    image->writeMetadata();
}

void Tag_Handler::removePointInComment(QString fileName, QPointF point)
{
    //load up the images tag
    loadTag(fileName);

    //get the current contents
    QString comment = QString::fromStdString((*exif_data)["Exif.Photo.UserComment"].toString());

    qreal x = point.x(), y = point.y();

    if(!comment.contains("("+QString::number(x) + "," + QString::number(y)+")"))
    {
        std::cout<<"Could not remove point ("<<x<<","<<y<<"), it doesnt exist!"<<std::endl;
    }
    else
    {
        comment.remove("("+QString::number(x) + "," + QString::number(y)+")");
        std::cout<<"Removing ("<<x<<","<<y<<")"<<std::endl;

        //set updated contents
        (*exif_data)["Exif.Photo.UserComment"]
              = comment.toStdString().c_str();

        //write back contents
        image->writeMetadata();
    }
}

GPS_data Tag_Handler::getGPS_fromFilename(QString filename)
{
    gps_found = false;
    GPS_data gps_ret;
    QString word;
    int start, colon, underscore_left = 0, underscore_right = 0;

    float lat = 0, lng = 0;

    start = filename.toStdString().find_last_of('/');

    underscore_right = underscore_left = start;

    while((underscore_left = filename.toStdString().find('_',underscore_right)) != -1
          && underscore_right != -1 && colon != -1)
    {
        colon = filename.toStdString().find(':',underscore_left);

        underscore_right = filename.toStdString().find('_',colon);

        word = QString::fromStdString(filename.toStdString().substr(underscore_left+1, 3));

        if(word == "LAT")
        {
            lat = atof(filename.toStdString().substr(colon+1, underscore_right -1 -colon).c_str());
            gps_found = true;
        }
        else if(word == "LON")
        {
            lng = atof(filename.toStdString().substr(colon+1, underscore_right -1 -colon).c_str());
            gps_found = true;
        }
    }

    clearGPSdata(gps_ret);
    gps_ret.lat_coord.dec_form = lat;
    gps_ret.long_coord.dec_form = lng;

    return gps_ret;
}
