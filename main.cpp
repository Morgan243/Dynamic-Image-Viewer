#include "mainwindow.h"
#include <QApplication>
//#include "imageviewer.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
 //  ImageViewer imageviewer;
//   imageviewer.show();
    MainWindow w;

    w.show();

    //w.ShowPicture();

    return a.exec();
}
