#include "mainwindow.h"
#include "ui_mainwindow.h"
 #include <QtGui>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString path = "/home/morgan/Documents/Pictures";
    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::AllDirs);
    fileModel->setRootPath(path);

    //ui->label_imageView = new QLabel;
    ui->label_imageView->setBackgroundRole(QPalette::Base);
    ui->label_imageView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    //ui->label_imageView->setScaledContents(true);


   // scrollArea = new QScrollArea;
    ui->scrollArea_imageView->setBackgroundRole(QPalette::Dark);
    //ui->scrollArea_imageView->setWidget(ui->label_imageView);
    //ui->scrollArea_imageView->setWidgetResizable(true);
    //setCentralWidget(ui->scrollArea_imageView);
    //setWindowTitle("Morgan's Dynamic Image Viewer");
    //ui->label_imageView->show();
    //ui->scrollArea_imageView->show();

    ui->listView_availImages->setModel(fileModel);
    ui->listView_availImages->setRootIndex(fileModel->setRootPath(path));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ShowPicture()
{
    //this->imageviewer.show();
    ui->label_imageView->show();
}

void MainWindow::on_chkBx_FitToWindow_stateChanged(int arg1)
{
    if(arg1)
        fitToWindow(true);
     //imageviewer.fitToWindow(true);
    else
        fitToWindow(false);
     //imageviewer.fitToWindow(false);
}

void MainWindow::on_pushBtn_ZoomIn_clicked()
{
    ui->doubleSpinBx_scaleFactor->setValue(zoomIn());
}

void MainWindow::on_pushBtn_ZoomOut_clicked()
{
    ui->doubleSpinBx_scaleFactor->setValue(zoomOut());
}

void MainWindow::on_actionOpen_Image_triggered()
{
    //QString imageName = imageviewer.open();
    QString fileName = QFileDialog::getOpenFileName(this,
                                    tr("Open File"), QDir::currentPath());

    QImage image(fileName);
    //imageLabel->setPixmap(QPixmap::fromImage(image));
    ui->label_imageView->setPixmap(QPixmap::fromImage(image));
    scaleFactor = 1.0;
    fitToWindow(false);

    ui->textBrowser_ImageInfo->setText(fileName);
    ui->textBrowser_ImageInfo->append("GPS CoOrd:");
    ui->textBrowser_ImageInfo->append("Time:");
    ui->textBrowser_ImageInfo->append("Altitude:");

}

void MainWindow::on_doubleSpinBx_scaleFactor_valueChanged(double arg1)
{
    //imageviewer.setScale(arg1);
}

void MainWindow::on_listView_availImages_activated(const QModelIndex &index)
{

}

void MainWindow::fitToWindow(bool fitToWindow)
{
    ui->scrollArea_imageView->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
        normalSize();
    }
}

void MainWindow::normalSize()
{
    ui->label_imageView->adjustSize();
    scaleFactor = 1.0;
}

double MainWindow::zoomIn()
{
    scaleImage(1.25);
    return this->scaleFactor;
}

double MainWindow::zoomOut()
{
    scaleImage(.8);
    return this->scaleFactor;
}

void MainWindow::scaleImage(double factor)
{
    Q_ASSERT(ui->label_imageView->pixmap());
    scaleFactor *= factor;

    ui->label_imageView->resize(scaleFactor * ui->label_imageView->pixmap()->size());

    adjustScrollBar(ui->scrollArea_imageView->horizontalScrollBar(), factor);
    adjustScrollBar(ui->scrollArea_imageView->verticalScrollBar(), factor);
}

void MainWindow::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}
