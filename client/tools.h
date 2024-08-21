#ifndef TOOLS_H
#define TOOLS_H

#include <QUrl>
#include <QLabel>
#include <QDebug>
#include <QString>
#include <QWidget>
#include <QLineEdit>
#include <type_traits>

template<typename Widget, class = void>
struct has_settext_function: std::false_type
{};

template<typename Widget>
struct has_settext_function<Widget,
                            std::void_t<decltype(std::declval<std::decay_t<Widget>>().setText(QString()))>>
    : std::true_type
{};

struct Tools
{
    static void refreshQssStyle(QWidget *widget);

    template<typename Widget,
        typename std::enable_if<has_settext_function<std::remove_pointer_t<Widget>>::value, int>::type = 0
    >
    static void setWidgetAttribute(Widget widget, const QString &message, bool status){
        widget->setText(message);
        widget->setProperty("state", (status ? "correct" : "incorrect"));
        refreshQssStyle(widget);
    }

    /*handling url info*/
    static void readConfigrationFile(QString file_name = QT_DEMO_HOME"config.ini");
    static QUrl getTargetUrl(QString param = "");   //for public access
    static QString url_info;                //store url info
    static bool url_init_flag;              //url is init or not?

    /*check validation*/
    static bool checkUsername(QLineEdit *edit, QLabel *label);
    static bool checkEmail(QLineEdit *edit, QLabel *label);
    static bool checkPassword(QLineEdit *edit, QLabel *label);
    static bool checkSimilarity(QLineEdit *edit_pass, QLineEdit *edit_confirm, QLabel *label);
    static bool checkCaptcha(QLineEdit *edit, QLabel *label);
};

#endif // TOOLS_H
