#ifndef RESETPASSWDINTERFACE_H
#define RESETPASSWDINTERFACE_H

#include "tools.h"
#include <QDialog>
#include <QTimer>

namespace Ui {
class ResetPasswdInterface;
}

class ResetPasswdInterface : public QDialog {
  Q_OBJECT

public:
  explicit ResetPasswdInterface(QWidget *parent = nullptr);
  ~ResetPasswdInterface();

private:
  void setResetAttribute();
  void registerTimeoutSetting();
  void registerEditFinishedEvent();
  void registerNetworkEvent();

  /*
   * switch to registeration successful page
   * by using stackedWidget switch to
   */
  void switchResetInfoPage();
  void switchResetPasswordPage();
  void switchResetSuccessfulPage();

private slots:
  void on_go_back_login_2_clicked();
  void on_go_back_login_1_clicked();
  void on_go_back_to_login3_clicked();

signals:
  void switchToLogin();

private:
  Ui::ResetPasswdInterface *ui;

  QTimer *m_timer;
  const std::size_t m_countdown;
  std::size_t m_counter;
};

#endif // RESETPASSWDINTERFACE_H
