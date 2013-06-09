#ifndef IMAGE_ANALYZER_H
#define IMAGE_ANALYZER_H
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QPointF>
#include <QMouseEvent>
#include "tag_handler.h"

class Image_Analyzer : public QGraphicsView
{

    Q_OBJECT
public:
    double scaleFactor;
    bool scaleToWindow, imgLoaded;

    Tag_Handler tagger;

    explicit Image_Analyzer();
    explicit Image_Analyzer(QGraphicsScene* scene);
    //virtual ~Image_Analyzer();

    void openImage(QString fileName);

    void fitToWindow(bool fitToWindow);

    void normalSize();

    double zoomIn();

    double zoomOut();

    void scaleImage(double factor);

    QString getFormattedTag();

signals:
    public slots:
        void mousePressEvent(QMouseEvent * e);

private:
    int imgWidth, imgHeight;

    //pointer to images as loaded
    QGraphicsPixmapItem *item;

    QGraphicsScene *image_scene;
};

#endif // IMAGE_ANALYZER_H
