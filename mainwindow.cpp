#include "mainwindow.h"
#include "ui_mainwindow.h"

 #include <QtGui>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    config_parser("div_config.xml")
{
    //easy way to keep things from happening until an image is loaded
    //imageView->imgLoaded = false;
    ui->setupUi(this);

    load_config();

    //Set up the file model for the directory view (avail images)
    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::AllDirs);
    fileModel->setRootPath(imagePath);

    //make the filemodel the available images listview's base model
    ui->listView_availImages->setModel(fileModel);
    ui->listView_availImages->setRootIndex(fileModel->setRootPath(imagePath));

    //Create graphics scene (where images are shown)
    QGraphicsScene *scene = new QGraphicsScene();
    imageView = new Image_Analyzer(scene);

    //add the graphics view to the layout
    ui->horizontalLayout->addWidget(imageView);

    //add a splitter between the ui and the image
    splitter = new QSplitter();
    splitter->addWidget(ui->groupBox_options);
    splitter->addWidget(imageView);

    connect(splitter, SIGNAL(splitterMoved(int,int)), this, SLOT(splitterResize(int,int)));

    stdModel = new QStandardItemModel(this);

    //scaleToWindow = false;
    //scaleFactor = 1.0;

    //do this or nothing shows up!
    setCentralWidget(splitter);

    //not sure what this was for....
    //availImagesIndex = ui->listView_availImages->model()->index(0,0);
    //ui->listView_availImages->selectionModel()->setCurrentIndex(availImagesIndex,QItemSelectionModel::SelectCurrent);

    //set up a selection model to be able to alter list view selection programmatically
    availImg_sm = ui->listView_availImages->selectionModel();
    connect(availImg_sm,SIGNAL(selectionChanged(const QItemSelection & , const QItemSelection & )),this, SLOT(availImageList_selectionChange(const QItemSelection & , const QItemSelection & )));

    //connect rowsInserted signal to the filesInserted slot
    connect(fileModel,SIGNAL(rowsInserted(const QModelIndex & , int , int  )), this, SLOT(filesInserted(QModelIndex,int,int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::load_config()
{
    main_config = config_parser.ParseConfig();

    //set tooltips
    ui->label_AvailableImages
            ->setToolTip(main_config.watch_dir.at(0));
    ui->label_priorityImages
            ->setToolTip(main_config.priority_dir.at(0));

    //path for the watch directory (just first watchDir in xml)
    imagePath = main_config.watch_dir.at(0);
}

void MainWindow::on_chkBx_autSelectLatest_stateChanged(int arg1)
{
    //if the box is checked
    if(arg1)
    {
        //select the top item in the list
        ui->listView_availImages->setRootIndex(fileModel->index(fileModel->rootPath()));
        ui->listView_availImages->setCurrentIndex(fileModel->index(0,0,ui->listView_availImages->rootIndex()));
    }

}

void MainWindow::on_chkBx_FitToWindow_stateChanged(int arg1)
{
    //if checked
    if(arg1)
    {
        if(imageView->imgLoaded)
            imageView->fitToWindow(true);

            imageView->scaleToWindow = true;
    }
    //else, not checked
    else
    {
        if(imageView->imgLoaded)
           imageView->fitToWindow(false);

        imageView->scaleToWindow = true;
    }
}

void MainWindow::on_pushBtn_ZoomIn_clicked()
{
    ui->doubleSpinBx_scaleFactor->setValue(imageView->zoomIn());
}

void MainWindow::on_pushBtn_ZoomOut_clicked()
{
    ui->doubleSpinBx_scaleFactor->setValue(imageView->zoomOut());
}

void MainWindow::on_actionOpen_Image_triggered()
{
    //open of a file selection dialog
    QString fileName = QFileDialog::getOpenFileName(this,
                                    tr("Open File"), QDir::currentPath());

    //open the image
    imageView->openImage(fileName);
}

void MainWindow::on_doubleSpinBx_scaleFactor_valueChanged(double arg1)
{
    //scale to the spin box value
    imageView->scaleImage(arg1/imageView->scaleFactor);
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
    imageView->openImage(select);
}

void MainWindow::filesInserted(const QModelIndex &parent, int start, int end)
{
    ui->listView_availImages->setRootIndex(fileModel->index(fileModel->rootPath()));
    ui->listView_availImages->setCurrentIndex(fileModel->index(0,0,ui->listView_availImages->rootIndex()));
}

void MainWindow::splitterResize(int pos, int index)
{
    imageView->fitToWindow(imageView->scaleToWindow);
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    imageView->fitToWindow(imageView->scaleToWindow);
}

void MainWindow::on_chkBox_reverseSort_stateChanged(int arg1)
{
    if(arg1)
        fileModel->sort(0,Qt::DescendingOrder);
    else
        fileModel->sort(0,Qt::AscendingOrder);
}
