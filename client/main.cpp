#include "mainwindow.h"
#include <QApplication>
#include <QString>
#include <QDebug>
#include <QFile>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString root = QCoreApplication::applicationDirPath();
    QString css_file = QDir::toNativeSeparators(
        root + QDir::separator() + QString("style") + QDir::separator() + QString("theme.qss")
    );

    /*Theme css file*/
    QFile qss(css_file);
    if(qss.open(QFile::ReadOnly)){
        qDebug() << "Open Success!\n";
        a.setStyleSheet(QString(qss.readAll()));
        qss.close();
    }
    else{
        qDebug() << "Open Failed!\n";
    }

    MainWindow w;
    w.show();
    return a.exec();
}
