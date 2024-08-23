#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <stack>
#include <QMainWindow>
#include "registerinterface.h"
#include "logininterface.h"
#include "resetpasswdinterface.h"

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
    void switchingToRegInterface();
    void switchingToLoginDialog();
    void switchingToResetDialog();

private:
    Ui::MainWindow *ui;
    registerinterface* m_register;
    LoginInterface* m_login;
    ResetPasswdInterface* m_reset;
};
#endif // MAINWINDOW_H
