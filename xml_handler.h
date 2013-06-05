#ifndef XML_HANDLER_H
#define XML_HANDLER_H
#include <QFileSystemModel>
//#include <QtXml/QXmlStreamReader>
#include <QDomDocument>

//base class for anything that parses XML files
class xml_Handler
{
protected:
    //xml file to be loaded
    QFile *file;

    //keeps xml doc in ram
    QDomDocument domDoc;

public:

    xml_Handler();

    xml_Handler(QString config_path);

    bool check_root(QDomDocument doc, QString expectedRoot);

    bool file_into_DOM(QDomDocument &doc, QString file_path);
};

#endif // XML_HANDLER_H
