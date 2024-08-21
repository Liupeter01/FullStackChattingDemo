#include <QDir>
#include "tools.h"
#include <QStyle>
#include <QString>
#include <QObject>
#include <QSettings> /*handle ini file*/
#include <QApplication>
#include <QRegularExpression>

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

bool Tools::checkUsername(QLineEdit *edit, QLabel *label)
{
    return true;
}

bool Tools::checkEmail(QLineEdit *edit, QLabel *label)
{
    QRegularExpression reg_email(QObject::tr("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+.[a-zA-Z]{2,}$"));
    if(!reg_email.match(edit->text()).hasMatch()){
        Tools::setWidgetAttribute(label, QString("Invalid E-mail address"), false);
        return false;
    }
     Tools::setWidgetAttribute(label, QString("E-mail Check Passed!"), true);
    return true;
}

bool Tools::checkPassword(QLineEdit *edit, QLabel *label)
{
    /* at least one capital letter(A-Z)
     * at least one lower case letter(a-z)
     * at least one number(0-z)
     * at least 8 character
     */
    QRegularExpression reg_password(QObject::tr("(?=.*[A-Z])(?=.*[a-z])(?=.*[0-9]).{8,}"));

    /*does not match the regular expression critiera!*/
    if(!reg_password.match(edit->text()).hasMatch()){
        Tools::setWidgetAttribute(label, QString("Invalid Password!"), false);
        return false;
    }
    Tools::setWidgetAttribute(label, QString("Password Check Passed!"), true);
    return true;
}

bool Tools::checkSimilarity(QLineEdit *edit_pass, QLineEdit *edit_confirm, QLabel *label)
{
    /*does not match origin password*/
    if(!checkPassword(edit_pass, label)){
        return false;
    }

    if(edit_pass->text() != edit_confirm->text()){
        Tools::setWidgetAttribute(label, QString("Password does not matched!"), false);
        return false;
    }
    Tools::setWidgetAttribute(label, QString("Password matched!"), true);
    return true;
}


bool Tools::checkCaptcha(QLineEdit *edit, QLabel *label)
{
    if(edit->text().isEmpty()){
        Tools::setWidgetAttribute(label, QString("CAPTCHA can not be empty!"), false);
        return false;
    }
    return true;
}
