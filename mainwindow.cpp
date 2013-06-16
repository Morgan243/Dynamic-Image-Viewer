#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>


MainWindow::MainWindow(QWidget *parent, CLI_options *options) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    config_parser(options->path_to_config)
{

    ui->setupUi(this);

    load_config();

    setup_paths(main_config.watch_dir.at(0), main_config.priority_dir.at(0));

    init_fileModels();

    init_view();

    init_marble();

    init_user_options();

    init_slots_signals();

    imageView->launchLoaderThread();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::load_config()
{
    main_config = config_parser.ParseConfig();
}


void MainWindow::setup_paths(QString watch_path, QString priority_path)
{
    //path for the watch directory (just first watchDir in xml)
    imagePath = watch_path;

    //path for priority directory
    priorityPath = priority_path;

    //check if the paths are the same
    if(imagePath == priorityPath)
    {
        struct stat st;
        std::cout<<"Watch path and priority path are the same!"<<std::endl;
        std::cout<<"Checking for priority directory in "<<qPrintable(imagePath)<<std::endl;

        //set priority path
        priorityPath = priorityPath + "/priority";

        //check if the priority directory already exists
        if (stat(QString(imagePath+"/"+"priority").toStdString().c_str(), &st) == 0 && S_ISDIR(st.st_mode))
        {
            std::cout<<"Priority directory exists, priority links will be stored in:\n";
            std::cout<<qPrintable(priorityPath)<<std::endl;
        }
        else
        {
             std::cout<<"Creating priority directory in "<<qPrintable(imagePath)<<std::endl;

            //create directory
             if (mkdir(priorityPath.toStdString().c_str(), 0777) != 0 && errno != EEXIST)
             {
                 std::cout<<"Error making priority directory ("<<qPrintable(priorityPath)<<")"<<std::endl;
             }
             else
                 std::cout<<"Directory created!"<<std::endl;
        }
    }

    //set tooltips
    ui->label_AvailableImages
            ->setToolTip(imagePath);
    ui->label_priorityImages
            ->setToolTip(priorityPath);
}

void MainWindow::init_fileModels()
{
//    //Set up the file model for the directory view (avail images)
//    fileModel = new QFileSystemModel(this);
//    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
//    fileModel->setRootPath(imagePath);

//    //make the filemodel the available images listview's base model
//    ui->listView_availImages->setModel(fileModel);
//    ui->listView_availImages->setRootIndex(fileModel->setRootPath(imagePath));

    //Set up the file model for the directory view (avail images)
    availFileModel = new DIVFileSystemModel(this);
    availFileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    availFileModel->setRootPath(imagePath);

    //make the filemodel the available images listview's base model
    ui->listView_availImages->setModel(availFileModel);
    ui->listView_availImages->setRootIndex(availFileModel->setRootPath(imagePath));

    //set context menu to custom
    ui->listView_availImages->setContextMenuPolicy(Qt::CustomContextMenu);

    //setup a slot for the context menu
    connect(ui->listView_availImages, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(showAvailImagesContext(const QPoint &)));


    //setup priority directory fileModel
    priorityFileModel = new QFileSystemModel(this);
    priorityFileModel->setFilter((QDir::NoDotAndDotDot | QDir::Files));
    priorityFileModel->setRootPath(priorityPath);

    ui->listView_priorityImages->setModel(priorityFileModel);
    ui->listView_priorityImages->setRootIndex(priorityFileModel->setRootPath(priorityPath));

    //set context menu to custom
    ui->listView_priorityImages->setContextMenuPolicy(Qt::CustomContextMenu);

    //setup a slot for the context menu
    connect(ui->listView_priorityImages, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(showPriorityImagesContext(const QPoint &)));
}

void MainWindow::init_view()
{
    //Create graphics scene (where images are shown)
    QGraphicsScene *scene = new QGraphicsScene();
    imageView = new Image_Analyzer(scene);

#ifdef GPS_FROM_FILENAME
    imageView->tagger.gps_from_filename = true;
#endif

    //connect to the the signal that lets us know the image loading thread finished loading
    connect(imageView, SIGNAL(imageLoaded(QImage, QString, bool)),this, SLOT(imageFinishedLoading(QImage, QString, bool)));

    //add the graphics view to the layout
    ui->horizontalLayout->addWidget(imageView);

    //add a splitter between the ui and the image
    splitter = new QSplitter();
    //splitter->addWidget(ui->groupBox_options);
    splitter->addWidget(ui->tabWidget);
    splitter->addWidget(imageView);

    connect(splitter, SIGNAL(splitterMoved(int,int)), this, SLOT(splitterResize(int,int)));

    stdModel = new QStandardItemModel(this);

    //do this or nothing shows up!
    setCentralWidget(splitter);

    //create actions for available (watch) list view context menu
    contextMenu_availView = new QMenu(tr("Watch Context Menu"), this);
    contextMenu_availView->addAction(new QAction(tr("Go-To GPS"), this));
    contextMenu_availView->addAction(new QAction(tr("Add to priority"), this));

    //create actions for priority list view context menu
    contextMenu_priorityView = new QMenu(tr("Priority Context Menu"), this);
    contextMenu_priorityView->addAction(new QAction(tr("Go-To GPS"), this));
    contextMenu_priorityView->addAction(new QAction(tr("Remove from priority"), this));


}

void MainWindow::init_marble()
{


    // Create a Marble QWidget without a parent
    mapWidget = new Marble::MarbleWidget();

    // Load the OpenStreetMap map
    //mapWidget->setMapThemeId("earth/openstreetmap/openstreetmap.dgml");
    //mapWidget->setMapThemeId("earth/mapquest-open-aerial/mapquest-open-aerial.dgml");
    mapWidget->setMapThemeId("earth/googlesat/googlesat.dgml");

    //ui->gridLayout_Mrable->addWidget(mapWidget);
    ui->verticalLayout->addWidget(mapWidget);

    //mapWidget->projectionChanged();
//    connect(mapWidget,SIGNAL(mouseMoveGeoPosition(QString)), this, SLOT(geoUpdate(QString)));
    //connect(mapWidget, SIGNAL(mouseClickGeoPosition(qreal,qreal,GeoDataCoordinates::Unit)),thi, geoClick(qreal, qreal, GeoDataCoordinates::Unit);
}

void MainWindow::init_user_options()
{
    //set the auto-select check box to match the startup setting
    auto_select_top = main_config.auto_select;
    ui->chkBx_autSelectLatest->setChecked(auto_select_top);

    //set the fit to window check box and match the the data member
    imageView->scaleToWindow = main_config.fit_to_window;
    ui->chkBx_FitToWindow->setChecked(imageView->scaleToWindow);

    if(main_config.reverse_sort)
        availFileModel->sort(0, Qt::DescendingOrder);
    else
        availFileModel->sort(0, Qt::AscendingOrder);

    ui->chkBox_reverseSort->setChecked(main_config.reverse_sort);
}

void MainWindow::init_slots_signals()
{
#ifdef FILE_WATCHER_ON
    //setup watcher
    file_watcher = new QFileSystemWatcher();
    //file_watcher->addPath(imagePath);

    connect(file_watcher,SIGNAL(fileChanged(QString)),this,SLOT(fileChangedSlot(QString)));
    connect(file_watcher,SIGNAL(directoryChanged(QString)),this,SLOT(dirChangedSlot(QString)));
#endif

    //set up a selection model to be able to alter list view selection programmatically
    availImg_sm = ui->listView_availImages->selectionModel();
    connect(availImg_sm,SIGNAL(selectionChanged(const QItemSelection & , const QItemSelection & )),this, SLOT(availImageList_selectionChange(const QItemSelection & , const QItemSelection & )));

    //connect rowsInserted signal to the filesInserted slot
    //connect(fileModel,SIGNAL(rowsInserted(const QModelIndex & , int , int  )), this, SLOT(filesInserted(QModelIndex,int,int)));
    connect(availFileModel,SIGNAL(rowsInserted(const QModelIndex & , int , int  )), this, SLOT(filesInserted(QModelIndex,int,int)));

    priorityImg_sm = ui->listView_priorityImages->selectionModel();
    connect(priorityImg_sm, SIGNAL(selectionChanged(const QItemSelection & , const QItemSelection &)), this, SLOT(priorityImageList_selectionChange(const QItemSelection & , const QItemSelection & )));

    //connect(fileModel,SIGNAL(rowsInserted(const QModelIndex & , int , int  )), this, SLOT(priorityFilesInserted(QModelIndex,int,int)));
    connect(priorityFileModel,SIGNAL(rowsInserted(const QModelIndex & , int , int  )), this, SLOT(priorityFilesInserted(QModelIndex,int,int)));

    file_poll = new QTimer(this);
    connect(file_poll, SIGNAL(timeout()), this, SLOT(updateFileLists()));

    file_poll->start(800);
}

//--Signals and slots for gui interfaces--
void MainWindow::on_chkBx_autSelectLatest_stateChanged(int arg1)
{
    //if the box is checked
    if(arg1)
    {
        //select the top item in the list
        //ui->listView_availImages->setRootIndex(fileModel->index(fileModel->rootPath()));
        //ui->listView_availImages->setCurrentIndex(fileModel->index(0,0,ui->listView_availImages->rootIndex()));
        ui->listView_availImages->setRootIndex(availFileModel->index(availFileModel->rootPath()));
        ui->listView_availImages->setCurrentIndex(availFileModel->index(0,0,ui->listView_availImages->rootIndex()));

        auto_select_top = true;
    }
    else
        auto_select_top = false;

}


void MainWindow::on_chkBx_FitToWindow_stateChanged(int arg1)
{
    //if checked
    if(arg1)
    {
        if(imageView->imgLoaded)
            imageView->fitToWindow(true);

            imageView->scaleToWindow = true;
            ui->doubleSpinBx_scaleFactor->setEnabled(false);
    }
    //else, not checked
    else
    {
        if(imageView->imgLoaded)
           imageView->fitToWindow(false);

        imageView->scaleToWindow = false;

        ui->doubleSpinBx_scaleFactor->setEnabled(true);
        imageView->resetTransform();
        imageView->scale(imageView->scaleFactor,imageView->scaleFactor);
    }
}

void MainWindow::on_pushBtn_ZoomIn_clicked()
{
    ui->doubleSpinBx_scaleFactor->setValue(imageView->zoomIn());
    ui->chkBx_FitToWindow->setChecked(false);
}

void MainWindow::on_pushBtn_ZoomOut_clicked()
{
    ui->doubleSpinBx_scaleFactor->setValue(imageView->zoomOut());
    ui->chkBx_FitToWindow->setChecked(false);
}

void MainWindow::on_actionOpen_Image_triggered()
{
    //open of a file selection dialog
    QString fileName = QFileDialog::getOpenFileName(this,
                                    tr("Open File"), QDir::currentPath());

    //open the image
    imageView->openImage(fileName);

    //set the image information box
    ui->textBrowser_ImageInfo->setText(imageView->getFormattedTag());
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

void MainWindow::on_actionOpen_Directory_triggered()
{
     QString dir;

    if(!main_config.watch_dir.empty())
        dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                               main_config.watch_dir.at(0),
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
    else
        dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                        "/home",                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);


    imagePath = dir;
    //fileModel->setRootPath(imagePath);
    availFileModel->setRootPath(imagePath);

    //ui->listView_availImages->setModel(fileModel);
    //ui->listView_availImages->setRootIndex(fileModel->setRootPath(imagePath));

    ui->listView_availImages->setModel(availFileModel);
    ui->listView_availImages->setRootIndex(availFileModel->setRootPath(imagePath));

    //set both the same
    setup_paths(imagePath, imagePath);

    priorityFileModel->setRootPath(priorityPath);

    ui->listView_priorityImages->setModel(priorityFileModel);
    ui->listView_priorityImages->setRootIndex(priorityFileModel->setRootPath(priorityPath));
}

void MainWindow::on_pushBtn_add_clicked()
{
    //get image name
    QString select = ui->listView_availImages->currentIndex().data().toString();

    //add a sym link to it in the priority directory
    addPriorityLink(select);
}

void MainWindow::on_pushBtn_remove_clicked()
{
    //get image name
    QString select = ui->listView_priorityImages->currentIndex().data().toString();

    //unlink the image
    removePriorityLink(select);
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
        //fileModel->sort(0,Qt::DescendingOrder);
        availFileModel->sort(0,Qt::DescendingOrder);
    else
        //fileModel->sort(0,Qt::AscendingOrder);
        availFileModel->sort(0,Qt::AscendingOrder);
}

//remove userComment tag from all images
void MainWindow::on_actionFrom_ALL_triggered()
{
}



//WATCH list view specific signals
void MainWindow::on_listView_availImages_activated(const QModelIndex &index)
{
    QString select = ui->listView_availImages->model()->index(0,0,index).data(Qt::DisplayRole).toString();
    ui->textBrowser_imageInfo_priority->append(select);
    //ui->textBrowser_imageInfo_priority->append("hello");
}

void MainWindow::on_listView_availImages_indexesMoved(const QModelIndexList &indexes)
{
    //ui->listView_availImages->selectionModel()->selectedIndexes()

    QString select = ui->listView_availImages->model()->index(0,0,indexes.first()).data(Qt::DisplayRole).toString();
    ui->textBrowser_imageInfo_priority->append(select);

}

void MainWindow::availImageList_selectionChange(const QItemSelection & selected, const QItemSelection & deselected)
{
    putSelectedImageToDisplay(watch);
}

void MainWindow::on_listView_availImages_clicked(const QModelIndex &index)
{
    putSelectedImageToDisplay(watch);
}

void MainWindow::filesInserted(const QModelIndex &parent, int start, int end)
{
    if(auto_select_top)
    {
        ui->listView_availImages->setRootIndex(availFileModel->index(availFileModel->rootPath()));
        ui->listView_availImages->setCurrentIndex(availFileModel->index(0,0,ui->listView_availImages->rootIndex()));

        //ui->listView_availImages->setRootIndex(fileModel->index(fileModel->rootPath()));
        //ui->listView_availImages->setCurrentIndex(fileModel->index(0,0,ui->listView_availImages->rootIndex()));
    }
}


//PRIORITY list view specific signals
void MainWindow::priorityImageList_selectionChange(const QItemSelection & selected, const QItemSelection & deselected)
{
    putSelectedImageToDisplay(priority);
}

void MainWindow::on_listView_priorityImages_clicked(const QModelIndex &index)
{
    putSelectedImageToDisplay(priority);
}

void MainWindow::priorityFilesInserted(const QModelIndex & parent, int start, int end )
{
    ui->listView_priorityImages->setRootIndex(priorityFileModel->index(priorityFileModel->rootPath()));
    ui->listView_priorityImages->setCurrentIndex(priorityFileModel->index(0,0,ui->listView_priorityImages->rootIndex()));
}


//--misc code that gets used a lot--
void MainWindow::addPriorityLink(QString watchFilename)
{
    //keep same filenames across the paths
    QString watchFilePath =  imagePath +"/" + watchFilename;
    QString priorityLinkPath = priorityPath +"/" + watchFilename;
    if(symlink(watchFilePath.toStdString().c_str(), priorityLinkPath.toStdString().c_str()) != 0 && errno != EEXIST)
    {
        std::cout<<"Could not create symlink"<<qPrintable(priorityLinkPath)<<std::endl;
    }
    else if(errno == EEXIST)
        std::cout<<"Priority link already exists!"<<std::endl;
}

void MainWindow::removePriorityLink(QString priorityFilename)
{
    QString priorityLinkPath = priorityPath + "/" + priorityFilename;
    if(unlink(priorityLinkPath.toStdString().c_str()) != 0)
    {
        std::cout<<"Could not remove link at "<<qPrintable(priorityLinkPath)<<std::endl;
    }
}

void MainWindow::putSelectedImageToDisplay(ImageSource source)
{
    QString select;

    ImgEvent ev;
    ev.event = load;


    source_of_view = source;
    //decide where to grab image name from and where to put meta info
    if(source == watch)
    {
        ev.option = select = imagePath +"/" + ui->listView_availImages->currentIndex().data().toString();

        ev.priorityImage = false;

        //create event for loader thread
        imageView->addEvent(ev);

        //open the image
        //imageView->openImage(select);

        //set the image information box
        //ui->textBrowser_ImageInfo->setText(imageView->getFormattedTag());
    }
    else
    {
        ev.option = select = imagePath +"/" + ui->listView_priorityImages->currentIndex().data().toString();

        //give event to loader
        imageView->addEvent(ev);
        //open the image
        //imageView->openImage(select);

        //set the image information box
        //ui->textBrowser_imageInfo_priority->setText(imageView->getFormattedTag());
    }

    image_in_view = select;

#ifdef FILE_WATCHER_ON
    file_watcher->addPath(select);
#endif

//    //open the image
//    imageView->openImage(select);
}

void MainWindow::fileChangedSlot(QString path)
{
    if(path == image_in_view)
    {

//        QVector<QPointF> points =imageView->tagger.readPointsFromComment(image_in_view);

//        if(imageView->image_marks[image_in_view] !=points)
//        {
//            //putSelectedImageToDisplay(source_of_view);
//            imageView->tagger.loadTag(image_in_view);
//            imageView->tagger.parseTag();



//            //if(points.count())
//                imageView->drawMarks(imageView->tagger.readPointsFromComment(image_in_view));
//        }
//        else
            putSelectedImageToDisplay(source_of_view);
    }
}

void MainWindow::dirChangedSlot(QString path)
{
    if(path == image_in_view)
    {
        putSelectedImageToDisplay(source_of_view);
    }
}

void MainWindow::showPriorityImagesContext(const QPoint &pos)
{
    //contextMenu_listView->addAction(new QAction(tr("Remove from priority"), this));
    handleListViewContext
            (contextMenu_priorityView->exec(ui->listView_priorityImages->mapToGlobal(pos)));
}

void MainWindow::showAvailImagesContext(const QPoint &pos)
{
    //contextMenu_listView->addAction(new QAction(tr("Add to priority"), this));
    handleListViewContext
            (contextMenu_availView->exec(ui->listView_availImages->mapToGlobal(pos)));
}

void MainWindow::handleListViewContext(QAction *selectedItem)
{
    if(selectedItem)
    {
         std::cout<<"Selected: "<<qPrintable(selectedItem->text())<<std::endl;
         if(selectedItem->text() == "Go-To GPS")
         {
             //SOE
             //qreal lat = 37.54511833;
             //qreal lng = -77.45010667;


            // qreal lat = 37.336218;
            // qreal lng = -77.236913;

             //SUAS Competition Site
             qreal lat = 38.150475;
             qreal lng = -76.424932;

             if(imageView->tagger.gps_found)
             {
                 lat = imageView->tagger.gps.lat_coord.dec_form;
                 lng = imageView->tagger.gps.long_coord.dec_form;
             }

             mapWidget->setCenterLatitude(lat);
             ui->doubleSpinBox_lat->setValue(lat);

             mapWidget->setCenterLongitude(lng);
             ui->doubleSpinBox_lon->setValue(lng);

             mapWidget->zoomViewBy(2500);
         }
         else if(selectedItem->text() == "Add to priority")
         {
            addPriorityLink(ui->listView_availImages->currentIndex().data().toString());
         }
         else if(selectedItem->text() == "Remove from priority")
         {
            removePriorityLink(ui->listView_priorityImages->currentIndex().data().toString());
         }

    }
    else
    {
        std::cout<<"Nothing selected in context menu"<<std::endl;
    }
}

void MainWindow::updateFileLists()
{
   //init_fileModels();
    availFileModel->setRootPath("");
    availFileModel->setRootPath(imagePath);

    priorityFileModel->setRootPath("");
    priorityFileModel->setRootPath(priorityPath);
}

void MainWindow::geoUpdate(const QString st)
{
    std::cout<<"HERE: "<<qPrintable(st)<<std::endl;
}



void MainWindow::imageFinishedLoading(QImage item, QString filename, bool priorityImage)
{
    //convert to pixmap and apply
    imageView->applyImage(item, filename);

    if(priorityImage)
    {
        //ui->textBrowser_imageInfo_priority->setText(imageView->getFormattedTag());
    }
    else
    {
        //ui->textBrowser_ImageInfo->setText(imageView->getFormattedTag());
    }
}
