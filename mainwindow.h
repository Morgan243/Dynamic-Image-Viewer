#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
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
    void ShowPicture();
    
private slots:
    void on_chkBx_FitToWindow_stateChanged(int arg1);

    void on_pushBtn_ZoomIn_clicked();

    void on_pushBtn_ZoomOut_clicked();

    void on_actionOpen_Image_triggered();

    void on_doubleSpinBx_scaleFactor_valueChanged(double arg1);

    void on_listView_availImages_activated(const QModelIndex &index);

    void fitToWindow(bool fitToWindow);

    void normalSize();

    void scaleImage(double factor);

    double zoomIn();

    double zoomOut();

     void adjustScrollBar(QScrollBar *scrollBar, double factor);
private:
    //ImageViewer imageviewer;
    QFileSystemModel *dirModel;
    QFileSystemModel *fileModel;

    //QLabel *label_imageView;
   // QScrollArea *scrollArea;

    double scaleFactor;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
