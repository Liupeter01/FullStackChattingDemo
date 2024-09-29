#ifndef USERTAGWIDGET_H
#define USERTAGWIDGET_H

#include "tools.h"
#include <QSize>
#include <QWidget>

namespace Ui {
class UserTagWidget;
}

class UserTagWidget : public QWidget {
  Q_OBJECT

public:
  explicit UserTagWidget(QWidget *parent = nullptr);
  ~UserTagWidget();

public:
  /*load image parameters*/
  static const QSize getImageSize();

  /*user set tag info*/
  void setTagName(const QString &text);

  /*when request dialog loading user tag widget*/
  virtual QSize sizeHint() const;

signals:
  void signal_close(QString str, LabelState state);

public slots:
  void slot_close(QString str, LabelState state);

private:
  /*using a parameter to adjust height for user experience*/
  static constexpr std::size_t COMPENSATION_HEIGHT = 2;
  static constexpr std::size_t COMPENSATION_WIDTH = 4;

  /*reserve for image*/
  static constexpr std::size_t image_width = 15;
  static constexpr std::size_t image_height = 15;

  /*add image*/
  void loadImage();

  /*style sheet*/
  void addStyleSheet();

  /*signal<->slot*/
  void registerSignal();

  /*update tag size by considering font size*/
  void updateTagSize();

private:
  Ui::UserTagWidget *ui;
  QSize m_tagSize;
};

#endif // USERTAGWIDGET_H
