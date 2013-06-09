#ifndef IMAGE_ANALYZER_H
#define IMAGE_ANALYZER_H
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QPointF>
#include <QMouseEvent>
#include <QHash>
#include "tag_handler.h"

#define MAX_MARKS 5

class Image_Analyzer : public QGraphicsView
{

    Q_OBJECT
public:
    double scaleFactor;
    bool scaleToWindow, imgLoaded;

    //image filenames as keys, list of points (for marks) as value
    QHash<QString, QVector<QPointF> > image_marks;

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

    void drawMarks(QVector<QPointF> points);

    void drawMark(QPointF point);

signals:
    public slots:
        void mousePressEvent(QMouseEvent * e);

        void wheelEvent(QWheelEvent *event);

private:
    int imgWidth, imgHeight;
    double mark_rad;
    QString file_in_view;

    //pointer to images as loaded
    QGraphicsPixmapItem *item;

    QGraphicsScene *image_scene;
};

#endif // IMAGE_ANALYZER_H
