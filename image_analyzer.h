#ifndef IMAGE_ANALYZER_H
#define IMAGE_ANALYZER_H
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QPointF>
#include <QMouseEvent>
#include <QHash>
#include "tag_handler.h"
#include <thread>
#include <mutex>
#include <QQueue>
#include <unistd.h>
#include <QLabel>
#define MAX_MARKS 5

enum Image_Event {load};

struct ImgEvent
{
    Image_Event event;
    QString option;
    bool priorityImage;
};

class Image_Analyzer : public QGraphicsView
{

    Q_OBJECT
public:
    double scaleFactor;
    bool scaleToWindow, imgLoaded, done;

    //image filenames as keys, list of points (for marks) as value
    QHash<QString, QVector<QPointF> > image_marks;

    Tag_Handler tagger;

    explicit Image_Analyzer();
    explicit Image_Analyzer(QGraphicsScene* scene);
    virtual ~Image_Analyzer();

    void addEvent(ImgEvent event);

    void launchLoaderThread();

    void runLoaderThread();

    QImage openImage(QString fileName);

    void applyImage(QImage item, QString fileName);

    void fitToWindow(bool fitToWindow);

    void normalSize();

    double zoomIn();

    double zoomOut();

    void scaleImage(double factor);

    QString getFormattedTag();

    void drawMarks(QVector<QPointF> points);

    void drawMark(QPointF point);

    QLabel *status_label;

signals:
        void imageLoaded(QImage item, QString fileName, bool priorityImg);

    public slots:
        void mousePressEvent(QMouseEvent * e);

        void wheelEvent(QWheelEvent *event);

private:
    int imgWidth, imgHeight;
    double mark_rad;

    std::thread *loader_thread;
    std::mutex event_lock;
    QQueue<ImgEvent> events;

    QString file_in_view;

    //pointer to images as loaded
    QGraphicsPixmapItem *item;

    QGraphicsScene *image_scene;



};

#endif // IMAGE_ANALYZER_H
