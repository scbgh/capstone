//
// main.cpp
// Main entry point for the editor.
//

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Capstone");
    QCoreApplication::setApplicationName("PGEditor");

    MainWindow window;
    window.show();
    return app.exec();    
}