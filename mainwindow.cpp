#include "mainwindow.h"
#include "ui_mainwindow.h"

 #include <QtGui>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    imgLoaded = false;
    ui->setupUi(this);
    imagePath = "/home/morgan/Documents/Pictures/DynImgTest/WatchFolder";
    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::AllDirs);
    fileModel->setRootPath(imagePath);

    ui->listView_availImages->setModel(fileModel);
    ui->listView_availImages->setRootIndex(fileModel->setRootPath(imagePath));

    QGraphicsScene *scene = new QGraphicsScene();
    graphicsView_imageView = new QGraphicsView(scene);
    ui->horizontalLayout->addWidget(graphicsView_imageView);

    splitter = new QSplitter();
    splitter->addWidget(ui->groupBox_options);
    splitter->addWidget(graphicsView_imageView);

    stdModel = new QStandardItemModel(this);

    scaleToWindow = false;
    scaleFactor = 1.0;

    setCentralWidget(splitter);

    availImagesIndex = ui->listView_availImages->model()->index(0,0);
    ui->listView_availImages->selectionModel()->setCurrentIndex(availImagesIndex,QItemSelectionModel::SelectCurrent);
    int something = -1;

    availImg_sm = ui->listView_availImages->selectionModel();
    connect(availImg_sm,SIGNAL(selectionChanged(const QItemSelection & , const QItemSelection & )),this, SLOT(availImageList_selectionChange(const QItemSelection & , const QItemSelection & )));

    bool isConnect = connect(fileModel,SIGNAL(rowsInserted(const QModelIndex & , int , int  )), this, SLOT(filesInserted(QModelIndex,int,int)));

    if(isConnect)
    {
        something = 0;
    }
    else
    {
        something =1;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_chkBx_autSelectLatest_stateChanged(int arg1)
{
    if(arg1)
    {
        //availImagesIndex = ui->listView_availImages->
        //availImagesIndex = ui->listView_availImages->rootIndex();

       // ui->listView_availImages->setCurrentIndex(availImagesIndex);
         //ui->listView_availImages->setCurrentIndex();
        //availImagesIndex = ui->listView_availImages->model()->index(0,0);
        //ui->listView_availImages->setCurrentIndex(availImagesIndex);
        ui->listView_availImages->setRootIndex(fileModel->index(fileModel->rootPath()));
        ui->listView_availImages->setCurrentIndex(fileModel->index(0,0,ui->listView_availImages->rootIndex()));
    }

}

void MainWindow::on_chkBx_FitToWindow_stateChanged(int arg1)
{
    if(arg1)
    {
        if(imgLoaded)
            fitToWindow(true);
        scaleToWindow = true;
     //imageviewer.fitToWindow(true);
    }
    else
    {
        if(imgLoaded)
            fitToWindow(false);
        scaleToWindow = true;
     //imageviewer.fitToWindow(false);
    }
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
    scaleImage(arg1/scaleFactor);
}

void MainWindow::openImage(QString fileName)
{
    QImage image(fileName);
    //imageLabel->setPixmap(QPixmap::fromImage(image));
    //ui->label_imageView->setPixmap(QPixmap::fromImage(image));
    //scaleFactor = 1.0;
    imgLoaded = true;


    //is this a possible memory leak? (not freeing pointer memory)
    item = new QGraphicsPixmapItem(QPixmap::fromImage(image));

    imgWidth = image.width();
    imgHeight = image.height();

    graphicsView_imageView->scene()->clear();
    graphicsView_imageView->scene()->addItem(item);
    graphicsView_imageView->show();
    fitToWindow(scaleToWindow);
    ui->textBrowser_ImageInfo->setText(fileName);
    ui->textBrowser_ImageInfo->append("GPS CoOrd:");
    ui->textBrowser_ImageInfo->append("Time:");
    ui->textBrowser_ImageInfo->append("Altitude:");
}

void MainWindow::fitToWindow(bool fitToWindow)
{
    scaleToWindow = fitToWindow;
    //QRect geometry = graphicsView_imageView->geometry();
    if(fitToWindow)
    {
        graphicsView_imageView->fitInView(0,0, imgWidth, imgHeight);
        //graphicsView_imageView->ensureVisible(0,0,imgWidth,imgHeight,0,0);
    }

#if 0
    if (!fitToWindow)
    {
        normalSize();
    }
    else
    {
        normalSize();

        float imgRatio = (float)imgWidth/(float)imgHeight;
        float viewRatio = (float)geometry.width()/(float)geometry.height();
        double tmpFactor = 0.0;

        if(imgRatio<viewRatio)
        {
            //scale to the height
            tmpFactor = (double)geometry.height()/(double)imgHeight;
            scaleImage(tmpFactor/scaleFactor);
        }
        else
        {
            //scale to the width
            tmpFactor = (double)geometry.width()/(double)imgWidth;
            scaleImage(tmpFactor/scaleFactor);
        }
       //graphicsView_imageView->adjustSize();

    }
#endif
    //graphicsView_imageView->ensureVisible(item,50,50);
    //graphicsView_imageView->setSceneRect(0,0, geometry.width(), geometry.height());

    //graphicsView_imageView->scroll(-10000, -10000);
}

void MainWindow::normalSize()
{
    scaleImage(1.0/scaleFactor);
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
   // QString select = imagePath +"/" + ui->listView_availImages->currentIndex().data().toString();
    //openImage(select);
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

void MainWindow::on_listView_availImages_indexesMoved(const QModelIndexList &indexes)
{
    //ui->listView_availImages->selectionModel()->selectedIndexes()

    QString select = ui->listView_availImages->model()->index(0,0,indexes.first()).data(Qt::DisplayRole).toString();
    ui->textBrowser_imageInfo_priority->append(select);

}

void MainWindow::availImageList_selectionChange(const QItemSelection & selected, const QItemSelection & deselected)
{
    QString select = imagePath +"/" + ui->listView_availImages->currentIndex().data().toString();
    openImage(select);
}

void MainWindow::filesInserted(const QModelIndex &parent, int start, int end)
{
    ui->listView_availImages->setRootIndex(fileModel->index(fileModel->rootPath()));
    ui->listView_availImages->setCurrentIndex(fileModel->index(0,0,ui->listView_availImages->rootIndex()));
}
