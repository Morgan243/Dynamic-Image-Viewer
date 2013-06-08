#include "image_analyzer.h"

Image_Analyzer::Image_Analyzer(QGraphicsScene *scene)
    :QGraphicsView(scene) //init parent class
{
    imgLoaded = scaleToWindow = false;
    scaleFactor = 1.0;
}

void Image_Analyzer::openImage(QString fileName)
{
    QImage image(fileName);

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

    //show some example text in the text box
//    ui->textBrowser_ImageInfo->setText(fileName);
//    ui->textBrowser_ImageInfo->append("GPS CoOrd:");
//    ui->textBrowser_ImageInfo->append("Time:");
//    ui->textBrowser_ImageInfo->append("Altitude:");

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
