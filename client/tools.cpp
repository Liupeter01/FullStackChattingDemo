#include "tools.h"
#include <QStyle>
#include <QDir>
#include <QSettings> /*handle ini file*/
#include <QApplication>

QString Tools::url_info{};
bool Tools::url_init_flag{false};

void Tools::refreshQssStyle(QWidget *widget){
    widget->style()->unpolish(widget);
    widget->style()->polish(widget);
}

void Tools::readConfigrationFile(QString file_name)
{
    /*configuration file and using separator and toNativeSeparators*/
    QSettings setting(file_name, QSettings::IniFormat);

    /*generate Url*/
    auto host = setting.value("ServerSetting/host").toString();
    auto port = setting.value("ServerSetting/port").toUInt();

    qDebug() << "host = " << host << ", port = " << port << '\n';
    url_info = QString("http://") + host + QString(":") + QString::number(port);

    /*init success then set flag to true*/
    url_init_flag = true;
}

QUrl Tools::getTargetUrl(QString param)
{
    if(!url_init_flag){
        Tools::readConfigrationFile(/*config.ini*/);
    }
    return QUrl(url_info + param);
}
