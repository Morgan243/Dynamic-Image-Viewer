#ifndef IMAGE_ANALYZER_H
#define IMAGE_ANALYZER_H
#include <QGraphicsView>
class Image_Analyzer : public QGraphicsView
{
public:
    Image_Analyzer();
    Image_Analyzer(QGraphicsScene* scene);
};

#endif // IMAGE_ANALYZER_H
