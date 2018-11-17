#include "mainwindow.h"
#include <QApplication>
#include "BNode.h"
#include "BTree.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    return a.exec();
}
