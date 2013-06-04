#include "xml_handler.h"

xml_Handler::xml_Handler()
    : config_domDoc("ConfigML")
{

}

xml_Handler::xml_Handler(QString config_path)
    : config_domDoc("ConfigML")
{

    //load file into DOM
    if(!file_into_DOM(config_domDoc, config_path))
    {
        printf("Unable to associate file with DOM!\n");
        return;
    }

    //check that the root is correct
    if(!check_root(config_domDoc, "configuration"))
    {
        printf("Document has wrong root!\n");
    }

    init_config(config);
}

void xml_Handler::init_config(Configuration init_me)
{
    init_me.config_path
            = "//home/morgan/Projects/QT/Dynamic_Image_Viewer.git/div_config.xml";

    init_me.watch_dir = "/home/morgan/Projects/UAV/pics_field_lowRes_run1_4-26";
    init_me.priority_dir = "/home/morgan/Projects/UAV/pics_field_lowRes_run1_4-26/priority";
    init_me.reverse_sort = true;
    init_me.copy_priority = false;
}

bool xml_Handler::check_root(QDomDocument doc, QString expectedRoot)
{
    QDomElement root = doc.documentElement();

    if(root.tagName() != "configuration")
        return false;

    return true;
}

bool xml_Handler::file_into_DOM(QDomDocument &doc, QString file_path)
{
    file = new QFile(file_path);

    //associate the xml file with the DOM
    if(!doc.setContent(file))
    {
        file->close();
        return false;
    }

    file->close();
    return true;
}
