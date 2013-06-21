#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtGui>
#include <QMainWindow>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QSplitter>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QScrollBar>
#include <qlistview.h>
#include "config_parser.h"
#include "image_analyzer.h"
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <QFileSystemWatcher>
#include <marble/MarbleWidget.h>
#include <marble/GeoDataDocument.h>
#include <marble/GeoDataPlacemark.h>
#include <marble/GeoDataLineString.h>
#include <marble/GeoDataTreeModel.h>
#include <marble/MarbleModel.h>
#include "divfilesystemmodel.h"
#include "Descriptors.h"
//#define FILE_WATCHER_ON 1
#define GPS_FROM_FILENAME 1
// #include <QExifImageHeader>
//#include "xml_handler.h"

class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;

enum ImageSource {watch, priority};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0, CLI_options *options = 0);
    ~MainWindow();
    
private slots:

    void load_config();

    void setup_paths(QString watch_path, QString priority_path);

    void init_fileModels();

    void init_view();

    void init_table();

    void init_marble();

    void init_user_options();

    void init_slots_signals();

    //general methods: UI and basic options
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    void on_actionOpen_Directory_triggered();

    void on_pushBtn_add_clicked();

    void on_pushBtn_remove_clicked();

    void on_chkBx_autSelectLatest_stateChanged(int arg1);

    void on_listView_availImages_indexesMoved(const QModelIndexList &indexes);

    void availImageList_selectionChange(const QItemSelection & selected, const QItemSelection & deselected);

    void filesInserted(const QModelIndex & parent, int start, int end );

    void priorityImageList_selectionChange(const QItemSelection & selected, const QItemSelection & deselected);

    void priorityFilesInserted(const QModelIndex & parent, int start, int end );

    void splitterResize(int pos, int index);

    void resizeEvent(QResizeEvent *);

    void on_chkBox_reverseSort_stateChanged(int arg1);

    void on_listView_availImages_activated(const QModelIndex &index);


    //Image manipulation etc.
    void on_chkBx_FitToWindow_stateChanged(int arg1);

    void on_pushBtn_ZoomIn_clicked();

    void on_pushBtn_ZoomOut_clicked();

    void on_actionOpen_Image_triggered();

    void on_doubleSpinBx_scaleFactor_valueChanged(double arg1);

    void addPriorityLink(QString watchFilename);

    void removePriorityLink(QString priorityFilename);

    void on_listView_availImages_clicked(const QModelIndex &index);

    void putSelectedImageToDisplay(ImageSource source);

    void on_listView_priorityImages_clicked(const QModelIndex &index);

    void on_actionFrom_ALL_triggered();


    void fileChangedSlot(QString path);

    void dirChangedSlot(QString path);

    void showPriorityImagesContext(const QPoint & pos);

    void showAvailImagesContext(const QPoint & pos);

    void handleListViewContext(QAction *selectedItem);

    void updateFileLists();

    void geoUpdate(const QString st);

    //void geoClick(qreal lon, qreal lat, GeoDataCoordinates::Unit unit);

    void imageFinishedLoading(QImage item, QString filename, bool priorityImage);
    void on_doubleSpinBox_lat_valueChanged(double arg1);

    void on_doubleSpinBox_lon_valueChanged(double arg1);

    void on_mouseMoveGeoPosition(QString st);

    void on_pushButton_grabFromGlobe_clicked();

    void on_comboBox_priorityImageSelect_currentIndexChanged(const QString &arg1);

    void on_pushButton_WriteTarget_clicked();

    void on_actionExit_triggered();

    void on_pushButton_writeToCSV_clicked();

    void on_pushButton_selectCSV_clicked();

    void on_pushButton_reload_clicked();

    void appendRowToTable(QStringList row);

    void appendRowToFile(QStringList row);
    void on_pushButton_updateCSV_clicked();

    void on_lineEdit_csvOutput_editingFinished();

    void writeHeaderToFile();

    void writeHeaderToTable();

    void on_pushButton_updateGPS_clicked();

private:

    bool auto_select_top, csv_selected;
    int current_row;

    QTimer *file_poll;

    Marble::MarbleWidget *mapWidget;

    QMenu *contextMenu_listView;
    QMenu *contextMenu_availView, *contextMenu_priorityView;

    QString image_in_view;
    ImageSource source_of_view;
    QString imagePath;
    QString priorityPath;

#ifdef FILE_WATCHER_ON
    QFileSystemWatcher *file_watcher;
#endif

    QFileSystemModel *dirModel;
    QFileSystemModel *fileModel;

    DIVFileSystemModel *availFileModel;
    QFileSystemModel *priorityFileModel;

    Image_Analyzer *imageView;

    //selected index for available images
    QModelIndex availImagesIndex;

    QSplitter *splitter;

    QStandardItemModel *stdModel;

    QItemSelectionModel *availImg_sm;
    QItemSelectionModel *priorityImg_sm;

    Ui::MainWindow *ui;

    Configuration main_config;

    Config_Parser config_parser;

    QString csvFilename;
    QVector<QStringList> rows;
    QStringList header;

};

#endif // MAINWINDOW_H
