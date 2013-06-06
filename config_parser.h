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
    bool reverse_sort;

};

class Config_Parser : public xml_Handler
{
private:
    Configuration config;

    void Parse_watchDir();
    void Parse_priorityDir();
    void Parse_options();
public:

    Config_Parser();
    Config_Parser(QString config_file);

    void init_config(Configuration &init_me);

    Configuration ParseConfig();
};

#endif // CONFIG_PARSER_H
