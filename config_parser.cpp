#include "config_parser.h"

Config_Parser::Config_Parser()
    : xml_Handler("div_config.xml")
{
}

Config_Parser::Config_Parser(QString config_path)
    : xml_Handler(config_path), config()
{
    if(!check_root(domDoc, root_element))
    {
        qDebug() << "Root appears to be invalid!\n";
    }
}

void Config_Parser::init_config(Configuration &init_me)
{
    init_me.config_path
            = "//home/morgan/Projects/QT/Dynamic_Image_Viewer.git/div_config.xml";

    init_me.watch_dir.push_back("/home/morgan/Projects/UAV/pics_field_lowRes_run1_4-26");
    init_me.priority_dir.push_back("/home/morgan/Projects/UAV/pics_field_lowRes_run1_4-26/priority");
    init_me.reverse_sort = true;
    init_me.copy_priority = false;
    init_me.auto_select = false;
    init_me.fit_to_window = false;
}

Configuration Config_Parser::ParseConfig()
{
    QDomElement docElem = domDoc.documentElement();

    Parse_watchDir(docElem);

    Parse_priorityDir(docElem);

    Parse_options(docElem);

    return config;
}

void Config_Parser::Parse_watchDir(QDomElement docElem)
{
    QDomNodeList watchDir_list = docElem.elementsByTagName("watchDir");

    for(int i = 0; i < watchDir_list.count(); i++)
    {
        QDomElement el = watchDir_list.at(i).toElement();
        //docElem = watchDir_list.at(i).toElement();

        QDomNode entries = el.firstChild();

        while(!entries.isNull())
        {
            QDomElement peData = entries.toElement();
            QString tagName = peData.tagName();

            if(tagName == "path")
            {
                config.watch_dir.push_back(peData.text());
                std::cout<<"New Watch Dir: "<<qPrintable(config.watch_dir.back())<<std::endl;
            }
            entries = entries.nextSibling();
        }
    }
}

void Config_Parser::Parse_priorityDir(QDomElement docElem)
{
    QDomNodeList watchDir_list = docElem.elementsByTagName("priorityDir");

    for(int i = 0; i < watchDir_list.count(); i++)
    {
        QDomElement el = watchDir_list.at(i).toElement();
        //docElem = watchDir_list.at(i).toElement();

        QDomNode entries = el.firstChild();

        while(!entries.isNull())
        {
            QDomElement peData = entries.toElement();
            QString tagName = peData.tagName();

            if(tagName == "path")
            {
                config.priority_dir.push_back(peData.text());
                std::cout<<"New Priority Dir: "<<qPrintable(config.priority_dir.back())<<std::endl;
            }
            entries = entries.nextSibling();
        }
    }
}

void Config_Parser::Parse_options(QDomElement docElem)
{
    QDomNodeList watchDir_list = docElem.elementsByTagName("options");

    for(int i = 0; i < watchDir_list.count(); i++)
    {
        QDomElement el = watchDir_list.at(i).toElement();
        //docElem = watchDir_list.at(i).toElement();

        QDomNode entries = el.firstChild();

        while(!entries.isNull())
        {
            QDomElement peData = entries.toElement();
            QString tagName = peData.tagName();

            if(tagName == "auto_select")
            {
                if(peData.text() == "on")
                {
                    config.auto_select = true;
                }
                else
                {
                    config.auto_select = false;
                }

                std::cout<<"Auto select top image is "<<qPrintable(peData.text())<<std::endl;
            }
            else if(tagName == "reverse_sort")
            {
                if(peData.text() == "on")
                {
                    config.reverse_sort = true;
                }
                else
                {
                    config.reverse_sort = false;
                }
                std::cout<<"Reverse sorting is "<<qPrintable(peData.text())<<std::endl;
            }
            else if(tagName == "fit_to_window")
            {
                if(peData.text() == "on")
                {
                    config.fit_to_window = true;
                }
                else
                {
                    config.fit_to_window = false;
                }
                std::cout<<"Fit-toWindow is "<<qPrintable(peData.text())<<std::endl;
            }

            entries = entries.nextSibling();
        }
    }
}
