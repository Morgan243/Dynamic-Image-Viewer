#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H
#include "xml_handler.h"
#include <QDebug>
#include <QVector>
#include <iostream>

#define root_element "configuration"

struct WatchDir
{
    QString path;

};

struct ProrityDir
{
    QString path;
};

struct Configuration
{
    QString config_path;
    QVector<QString> watch_dir;
    QVector<QString> priority_dir;
    bool copy_priority;     //use sym-links if false
    bool fit_to_window;
    bool reverse_sort;
    bool auto_select;
};

class Config_Parser : public xml_Handler
{
private:
    Configuration config;

    void Parse_watchDir(QDomElement docElem);
    void Parse_priorityDir(QDomElement docElem);
    void Parse_options(QDomElement docElem);
public:

    Config_Parser();
    Config_Parser(QString config_file);

    void init_config(Configuration &init_me);

    Configuration ParseConfig();
};

#endif // CONFIG_PARSER_H
