#ifndef CHATTINGRECORDVIEW_H
#define CHATTINGRECORDVIEW_H

#include <QEvent>
#include <QListWidget>
#include <QPaintEvent>
#include <QWidget>

class ChattingRecordView : public QListWidget {
  Q_OBJECT
public:
  explicit ChattingRecordView(QWidget *parent = nullptr);

public:
  /*back insert*/
  void pushBackItem(QWidget *item);

  /*front insert*/
  void pushFrontItem(QWidget *item);

  /*insert randomly*/
  void randomInsertItem(int pos, QWidget *item);

  static constexpr std::size_t max_size = 16777215;

protected:
  bool eventFilter(QObject *object, QEvent *event) override;
  void paintEvent(QPaintEvent *event) override;

private:
  void registerSignal();

private slots:
  void slot_rangeChanged(int min, int max);

private:
  bool m_newdataarrived;
};

#endif // CHATTINGRECORDVIEW_H
