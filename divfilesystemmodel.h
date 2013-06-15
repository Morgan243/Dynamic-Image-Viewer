#ifndef DIVFILESYSTEMMODEL_H
#define DIVFILESYSTEMMODEL_H

#include <QFileSystemModel>

class DIVFileSystemModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit DIVFileSystemModel(QObject *parent = 0);
    void callReset(QString path);
    
signals:
    
public slots:

};

#endif // DIVFILESYSTEMMODEL_H
