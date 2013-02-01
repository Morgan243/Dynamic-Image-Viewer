#include "mainwindow.h"
#include "ui_mainwindow.h"
 #include <QtGui>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    imagePath = "/home/morgan/Documents/Pictures";
    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::AllDirs);
    fileModel->setRootPath(imagePath);

    ui->listView_availImages->setModel(fileModel);
    ui->listView_availImages->setRootIndex(fileModel->setRootPath(imagePath));

    QGraphicsScene *scene = new QGraphicsScene();
    graphicsView_imageView = new QGraphicsView(scene);
    ui->horizontalLayout->addWidget(graphicsView_imageView);

    //graphicsView_imageView->

    splitter = new QSplitter();
    splitter->addWidget(ui->groupBox_options);
    splitter->addWidget(graphicsView_imageView);

    stdModel = new QStandardItemModel(this);

    setCentralWidget(splitter);
}

MainWindow::~MainWindow()
{
    delete ui;
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
    //graphicsView_imageView->resize();
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


    openImage(fileName);
}

void MainWindow::on_doubleSpinBx_scaleFactor_valueChanged(double arg1)
{
    //imageviewer.setScale(arg1);
}

void MainWindow::openImage(QString fileName)
{
    QImage image(fileName);
    //imageLabel->setPixmap(QPixmap::fromImage(image));
    //ui->label_imageView->setPixmap(QPixmap::fromImage(image));
    scaleFactor = 1.0;
    fitToWindow(false);

    //is this a possible memory leak? (not freeing pointer memory)
    item = new QGraphicsPixmapItem(QPixmap::fromImage(image));

    //ui->graphicsView_imageView->
    graphicsView_imageView->scene()->clear();
    graphicsView_imageView->scene()->addItem(item);
    graphicsView_imageView->show();

    ui->textBrowser_ImageInfo->setText(fileName);
    ui->textBrowser_ImageInfo->append("GPS CoOrd:");
    ui->textBrowser_ImageInfo->append("Time:");
    ui->textBrowser_ImageInfo->append("Altitude:");
}

void MainWindow::fitToWindow(bool fitToWindow)
{
    //ui->scrollArea_imageView->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
        normalSize();
    }
}

void MainWindow::normalSize()
{
    //ui->label_imageView->adjustSize();
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
    scaleFactor *= factor;
    graphicsView_imageView->scale(factor, factor);
}

void MainWindow::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

void MainWindow::on_listView_availImages_activated(const QModelIndex &index)
{
    QString select = ui->listView_availImages->model()->index(0,0,index).data(Qt::DisplayRole).toString();
    ui->textBrowser_imageInfo_priority->append(select);
    ui->textBrowser_imageInfo_priority->append("hello");
}

void MainWindow::on_listView_availImages_clicked(const QModelIndex &index)
{
    QString select = imagePath +"/" + ui->listView_availImages->currentIndex().data().toString();
    openImage(select);
}

void MainWindow::on_actionOpen_Directory_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                "/home",
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);

    imagePath = dir;
    fileModel->setRootPath(imagePath);

    ui->listView_availImages->setModel(fileModel);
    ui->listView_availImages->setRootIndex(fileModel->setRootPath(imagePath));
}

void MainWindow::on_pushBtn_add_clicked()
{
    QString select = ui->listView_availImages->currentIndex().data().toString();
    QStandardItem* items = new QStandardItem(select);
    stdModel->appendRow(items);
    ui->listView_priorityImages->setModel(stdModel);
}

void MainWindow::on_pushBtn_remove_clicked()
{
    ui->listView_priorityImages->model()->removeRow(
                ui->listView_priorityImages->currentIndex().row());
}
