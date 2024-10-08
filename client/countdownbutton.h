#ifndef COUNTDOWNBUTTON_H
#define COUNTDOWNBUTTON_H

#include <QMouseEvent>
#include <QPushButton>
#include <QTimer>

/*default time interval = 1000ms(1s)*/
class CountDownButton : public QPushButton {
public:
  CountDownButton(QWidget *parent = nullptr, std::size_t seconds = 10);
  virtual ~CountDownButton();

private:
  void registerTimer(std::size_t seconds);
  void mouseReleaseEvent(QMouseEvent *e) override;

private:
  QTimer *m_timer;
  const std::size_t m_countdown;
  std::size_t m_counter;
};

#endif // COUNTDOWNBUTTON_H
