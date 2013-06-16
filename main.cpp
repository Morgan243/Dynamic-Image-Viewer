#include "mainwindow.h"
#include <QApplication>
#include "Descriptors.h"
#include "stdlib.h"
#include <X11/Xlib.h>

bool handleArgs(int argc, char *argv[], CLI_options &options);
void printHelp();
QString GetEnv( const QString & var );

int main(int argc, char *argv[])
{
    int x_o = XInitThreads();

    if(!x_o)
    {
        std::cout<<"ERROR INITIALIZING X THREADS! EXITING!"<<std::endl;
        return 0;
    }

    CLI_options options;
    options.path_to_config = GetEnv("HOME") + "/.local/share/div_config.xml";

    std::cout<<"Default config path: "<<qPrintable(options.path_to_config)<<std::endl;

    if(!handleArgs(argc,argv,options))
        return 0;

    QApplication a(argc, argv);

    MainWindow w(0, &options);

    w.show();

    return a.exec();
}

bool handleArgs(int argc, char *argv[], CLI_options &options)
{
    for(int i = 0; i < argc; i++)
    {
        if(strcmp(argv[i],"-h") == 0)
        {
            printHelp();
            return false;
        }
        else if(!strcmp(argv[i],"-c") || !strcmp(argv[i],"--config"))
        {
            options.path_to_config = argv[i + 1];
            std::cout<<"Will attempt to parse config file: "<< qPrintable(options.path_to_config)<<std::endl;
        }
    }
}

void printHelp()
{
    std::cout<<"---Dynamic Image Viewer: VCU ARG '13 Edition---"<<std::endl;
    std::cout<<"-c <file> (--config)\t Path to xml configuration file."<<std::endl;
}

QString GetEnv( const QString & var ) {
     const char * val = ::getenv( var.toStdString().c_str() );
     if ( val == 0 ) {
         return "";
     }
     else {
         return QString(val);
     }
}
