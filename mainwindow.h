#ifndef MAINWINDOW_H
#define MAINWINDOW_H
 #include <QtGui>
#include <QMainWindow>
#include <QFileSystemModel>
#include <QSplitter>>
//#include "imageviewer.h"
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_chkBx_FitToWindow_stateChanged(int arg1);

    void on_pushBtn_ZoomIn_clicked();

    void on_pushBtn_ZoomOut_clicked();

    void on_actionOpen_Image_triggered();

    void on_doubleSpinBx_scaleFactor_valueChanged(double arg1);

    void on_listView_availImages_activated(const QModelIndex &index);

    void openImage(QString fileName);

    void fitToWindow(bool fitToWindow);

    void normalSize();

    void scaleImage(double factor);

    double zoomIn();

    double zoomOut();

    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    void on_listView_availImages_clicked(const QModelIndex &index);

private:
    double scaleFactor;
    QString imagePath;

    QFileSystemModel *dirModel;
    QFileSystemModel *fileModel;

    //pointer to images as loaded
    QGraphicsPixmapItem *item;

    //where images are shown
    QGraphicsView *graphicsView_imageView;

    QSplitter *splitter;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
