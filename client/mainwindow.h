#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// #include <stack>
#include "logininterface.h"
#include "registerinterface.h"
#include "resetpasswdinterface.h"
#include "chattingdlgmainframe.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
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
  void swithcingToChattingInf();

private:
  Ui::MainWindow *ui;
  registerinterface *m_register;
  LoginInterface *m_login;
  ResetPasswdInterface *m_reset;
  ChattingDlgMainFrame *m_chattingMainFrame;
};
#endif // MAINWINDOW_H
