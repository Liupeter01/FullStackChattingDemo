#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <stack>
#include <QMainWindow>
#include "registerinterface.h"
#include "logininterface.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setFramelessWindow(QDialog *dialog);
    void displayDefaultWindow(QWidget *window);
    void registerSignalSlots();
    void interfaceSwitchingHandler();

private:
    Ui::MainWindow *ui;
    //std::stack<QDialog *> m_dialogSwitch;
    registerinterface* m_register;
    LoginInterface* m_login;
};
#endif // MAINWINDOW_H
