#include <QtGui/QApplication>
#include <QLocale>
#include <QTextCodec>
#include <QTranslator>
#include <QDir>
#include "mainwindow.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    // Translation encoding
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    QTranslator t;
    t.load(QApplication::applicationDirPath() + "/po/imageguess_" + QLocale::system().name());
    a.installTranslator(&t);
    a.addLibraryPath(QApplication::applicationDirPath());

    w.show();
    return a.exec();
}
