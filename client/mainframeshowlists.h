#ifndef MAINFRAMESHOWLISTS_H
#define MAINFRAMESHOWLISTS_H

#include <QEvent>
#include <QListWidget>

class MainFrameShowLists : public QListWidget {
  Q_OBJECT

public:
  MainFrameShowLists(QWidget *parent = nullptr);
  virtual ~MainFrameShowLists();

protected:
  /*customlized functions*/
  bool eventFilter(QObject *object, QEvent *event) override;

signals:
  void signal_load_more_record();

private:
};

#endif // MainFrameShowLists_H
