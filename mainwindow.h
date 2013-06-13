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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:

    void load_config();

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
private:

    bool auto_select_top;

    QString image_in_view;
    ImageSource source_of_view;
    QString imagePath;
    QString priorityPath;

    QFileSystemWatcher *file_watcher;

    QFileSystemModel *dirModel;
    QFileSystemModel *fileModel;

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
};

#endif // MAINWINDOW_H
