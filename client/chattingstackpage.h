#ifndef CHATTINGSTACKPAGE_H
#define CHATTINGSTACKPAGE_H

#include <QWidget>

namespace Ui {
class ChattingStackPage;
}

class ChattingStackPage : public QWidget {
  Q_OBJECT

public:
  explicit ChattingStackPage(QWidget *parent = nullptr);
  ~ChattingStackPage();

private slots:
  void on_send_message_clicked();

  private:
  Ui::ChattingStackPage *ui;
};

#endif // CHATTINGSTACKPAGE_H
