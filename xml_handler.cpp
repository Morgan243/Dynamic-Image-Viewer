#include "xml_handler.h"
#include <iostream>

xml_Handler::xml_Handler()
    : domDoc("ConfigML")
{

}

xml_Handler::xml_Handler(QString config_path)
    : domDoc("configuration")
{

    printf("Creating XML handler...\n");

    //load file into DOM
    if(!file_into_DOM(domDoc, config_path))
    {
        printf("Unable to associate file with DOM!\n");
        return;
    }

    //check that the root is correct
    if(!check_root(domDoc, "configuration"))
    {
        printf("Document has wrong root!\n");
    }
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

    if(!file->open(QIODevice::ReadWrite))
    {
        std::cout<<"Unable to open file! ("<<qPrintable(file_path)<<")\n";

        return false;
    }

    std::cout<<qPrintable(file_path)<<" opened succesfully...\n";

    //associate the xml file with the DOM
    if(!doc.setContent(file))
    {
        std::cout<<"SetContent failed!\n";
        file->close();
        return false;
    }

    file->close();
    return true;
}
