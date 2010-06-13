#include <QtGui/QApplication>
#include <QLocale>
#include <QTextCodec>
#include <QTranslator>
#include "mainwindow.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    // Translation encoding
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    QTranslator t;
    t.load("./po/imageguess_" + QLocale::system().name());
    a.installTranslator(&t);

    w.show();
    return a.exec();
}
