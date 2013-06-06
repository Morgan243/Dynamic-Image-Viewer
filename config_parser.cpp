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
}

Configuration Config_Parser::ParseConfig()
{
    QDomElement docElem = domDoc.documentElement();

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

    return config;
}
