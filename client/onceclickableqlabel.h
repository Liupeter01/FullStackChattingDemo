#ifndef ONCECLICKABLEQLABEL_H
#define ONCECLICKABLEQLABEL_H

#include <QLabel>

class OnceClickableQLabel : public QLabel {
  Q_OBJECT

public:
  OnceClickableQLabel(QWidget *parent = nullptr);
  ~OnceClickableQLabel();

signals:
  void clicked(QString);

protected:
  virtual void mousePressEvent(QMouseEvent *event) override;
};

#endif // ONCECLICKABLEQLABEL_H
