#ifndef LOGININTERFACE_H
#define LOGININTERFACE_H

#include <QDialog>

namespace Ui {
class LoginInterface;
}

class LoginInterface : public QDialog {
  Q_OBJECT

public:
  explicit LoginInterface(QWidget *parent = nullptr);
  ~LoginInterface();

private:
  void registerSignal();
  void setLoginAttribute();
  void slot_forgot_passwd();

signals:
  /*switch to register interface*/
  void switchWindow();

  /*switch to reset interface*/
  void switchReset();

private:
  Ui::LoginInterface *ui;
};

#endif // LOGININTERFACE_H
