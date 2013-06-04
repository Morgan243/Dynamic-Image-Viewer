#ifndef XML_HANDLER_H
#define XML_HANDLER_H
#include <QFileSystemModel>
#include <QtXml/QXmlStreamReader>
#include <QDomDocument>

struct Configuration
{
    QString config_path;
    QString watch_dir;
    QString priority_dir;
    bool copy_priority;     //use sym-links if false
    bool reverse_sort;
};

class xml_Handler
{
private:
    Configuration config;

    QFile *file;

    QFile *config_file;

    QDomDocument config_domDoc;
public:

    xml_Handler();

    xml_Handler(QString config_path);

    void init_config(Configuration init_me);

    bool check_root(QDomDocument doc, QString expectedRoot);

    bool file_into_DOM(QDomDocument &doc, QString file_path);

    void parse_config();
};

#endif // XML_HANDLER_H
