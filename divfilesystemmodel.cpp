#include "divfilesystemmodel.h"

DIVFileSystemModel::DIVFileSystemModel(QObject *parent) :
    QFileSystemModel(parent)
{
}

void DIVFileSystemModel::callReset(QString path)
{
    //this->reset();
    //this->resetInternalData();
    this->rootPathChanged(path);
}
