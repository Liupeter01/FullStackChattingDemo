#ifndef ONCECLICKABLEQLABEL_H
#define ONCECLICKABLEQLABEL_H
#include "tools.h"
#include <QLabel>

class OnceClickableQLabel : public QLabel {
  Q_OBJECT

public:
  OnceClickableQLabel(QWidget *parent = nullptr);
  virtual ~OnceClickableQLabel();

public:
  void setCurrentState(LabelState::VisiableStatus status);

signals:
  void clicked(QString, LabelState);
  void update_display(QString, LabelState);

protected:
  virtual void mousePressEvent(QMouseEvent *event) override;

private:
  LabelState m_state;
};

#endif // ONCECLICKABLEQLABEL_H
