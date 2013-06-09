#include "image_analyzer.h"

Image_Analyzer::Image_Analyzer(QGraphicsScene *scene)
    :QGraphicsView(scene) //init parent class
{
    image_scene = scene;
    imgLoaded = scaleToWindow = false;
    scaleFactor = 1.0;
    mark_rad = 75;
}

void Image_Analyzer::openImage(QString fileName)
{
    QImage image(fileName);

    file_in_view = fileName;

    imgLoaded = true;

    //is this a possible memory leak? (not freeing pointer memory)
    item = new QGraphicsPixmapItem(QPixmap::fromImage(image));

    //for quick access
    imgWidth = image.width();
    imgHeight = image.height();

    //clear the scene, add the image, and show it
    this->scene()->clear();
    this->scene()->addItem(item);
    this->show();

    //scale window if needed
    fitToWindow(scaleToWindow);

    tagger.loadTag(fileName);
    tagger.parseTag();

    //if file has been marked, load the marks
    if(image_marks.contains(fileName))
        drawMarks(image_marks[fileName]);

}

void Image_Analyzer::fitToWindow(bool fitToWindow)
{
    scaleToWindow = fitToWindow;
    //QRect geometry = graphicsView_imageView->geometry();
    if(fitToWindow)
    {
        this->fitInView(0,0, imgWidth, imgHeight);
    }

#if 0
    if (!fitToWindow)
    {
        normalSize();
    }
    else
    {
        normalSize();

        float imgRatio = (float)imgWidth/(float)imgHeight;
        float viewRatio = (float)geometry.width()/(float)geometry.height();
        double tmpFactor = 0.0;

        if(imgRatio<viewRatio)
        {
            //scale to the height
            tmpFactor = (double)geometry.height()/(double)imgHeight;
            scaleImage(tmpFactor/scaleFactor);
        }
        else
        {
            //scale to the width
            tmpFactor = (double)geometry.width()/(double)imgWidth;
            scaleImage(tmpFactor/scaleFactor);
        }
       //graphicsView_imageView->adjustSize();

    }
#endif
    //graphicsView_imageView->ensureVisible(item,50,50);
    //graphicsView_imageView->setSceneRect(0,0, geometry.width(), geometry.height());

    //graphicsView_imageView->scroll(-10000, -10000);
}

void Image_Analyzer::normalSize()
{
    scaleImage(1.0/scaleFactor);
}

double Image_Analyzer::zoomIn()
{
    scaleImage(1.25);
    return this->scaleFactor;
}

double Image_Analyzer::zoomOut()
{
    scaleImage(.8);
    return this->scaleFactor;
}

void Image_Analyzer::scaleImage(double factor)
{
    scaleFactor *= factor;
    this->scale(factor, factor);
}

QString Image_Analyzer::getFormattedTag()
{
    QString formattedTag;

    formattedTag = "LAT:" + tagger.gps.lat_ratio;
    formattedTag = formattedTag + "\n>> (" + tagger.gps.lat_coord.ref
                    +") [" + tagger.getDegMinSec(tagger.gps.lat_coord) + "]";

    formattedTag = formattedTag + "\n\nLONG: " + tagger.gps.long_ratio;
    formattedTag = formattedTag + "\n>> (" + tagger.gps.long_coord.ref +
            +") [" + tagger.getDegMinSec(tagger.gps.long_coord) + "]";

    return formattedTag;
}

void Image_Analyzer::mousePressEvent(QMouseEvent *e)
{
    double rad = 75;

    QPointF pt = mapToScene(e->pos());

    if(e->buttons() & Qt::LeftButton)
    {
        image_marks[file_in_view].push_back(pt);

        drawMark(pt);
    }
}

void Image_Analyzer::drawMark(QPointF point)
{
    for(double i = 0; i < 15; i+=.25)
    {
        image_scene->addEllipse(point.x()-(mark_rad+i), point.y()-(mark_rad+i), (mark_rad+i)*2.0, (mark_rad+i)*2.0,
                                QColor(255, (int)i<<2, 0),
                                QBrush());
    }
}

void Image_Analyzer::drawMarks(QVector<QPointF> points)
{
    for(int i = 0; i < points.count(); i++)
    {
        drawMark(points.at(i));
    }
}
