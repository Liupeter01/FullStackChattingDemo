#ifndef FORGOTPASSWORD_H
#define FORGOTPASSWORD_H

#include "multiclickableqlabel.h"

class ForgotPassword : public MultiClickableQLabel {
  Q_OBJECT;

public:
  ForgotPassword();

public:
  ForgotPassword(QWidget *parent = nullptr);
  virtual ~ForgotPassword();

protected:
  virtual void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // FORGOTPASSWORD_H
