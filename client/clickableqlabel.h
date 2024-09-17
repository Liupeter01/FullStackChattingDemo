#ifndef CLICKABLEQLABEL_H
#define CLICKABLEQLABEL_H
#include "tools.h"
#include <QLabel>
#include <QWidget>
#include <Qt>

class ClickableQLabel : public QLabel {
  Q_OBJECT

public:
  ClickableQLabel(QWidget *parent = nullptr,
                  Qt::WindowFlags f = Qt::WindowFlags());
  ~ClickableQLabel();

  const LabelState &getState() const;

signals:
  void update_display();
  void clicked();

protected:
  virtual void mouseReleaseEvent(QMouseEvent *event) override;

  virtual void mousePressEvent(QMouseEvent *event) override;

  /*mouse enter selected section*/
  virtual void enterEvent(QEnterEvent *event) override;

  /*mouse leave*/
  virtual void leaveEvent(QEvent *event) override;

protected:
  LabelState m_state;
};

#endif // ClickableQLabel_H
