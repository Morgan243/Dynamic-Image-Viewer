#ifndef IMAGE_ANALYZER_H
#define IMAGE_ANALYZER_H
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include "tag_handler.h"

class Image_Analyzer : public QGraphicsView
{
private:
    int imgWidth, imgHeight;


    //pointer to images as loaded
    QGraphicsPixmapItem *item;

public:
    double scaleFactor;
    bool scaleToWindow, imgLoaded;

    Tag_Handler tagger;

    Image_Analyzer();
    Image_Analyzer(QGraphicsScene* scene);

    void openImage(QString fileName);

    void fitToWindow(bool fitToWindow);

    void normalSize();

    double zoomIn();

    double zoomOut();

    void scaleImage(double factor);
};

#endif // IMAGE_ANALYZER_H