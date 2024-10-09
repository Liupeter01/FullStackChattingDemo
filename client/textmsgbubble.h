#ifndef TEXTMSGBUBBLE_H
#define TEXTMSGBUBBLE_H

#include "msgbubblebase.h"
#include <QFont>
#include <QTextEdit>

class TextMsgBubble : public MsgBubbleBase {
  Q_OBJECT
public:
  TextMsgBubble(ChattingRole role, const QString &text,
                QWidget *parent = nullptr);
  ~TextMsgBubble();

  /* set text for textedit
   * we have to calculate the max width for the textedit
   */
  void setupTextMsg(const QString &str);

protected:
  bool eventFilter(QObject *object, QEvent *event) override;

private:
  /*if window size changed then adjust bubble size*/
  void adjustBubbleSize();

  /*add style sheet*/
  void addStyleSheet();

  /*using a parameter to adjust height for user experience*/
  static constexpr std::size_t COMPENSATION_VALUE = 30;

private:
  QFont m_font;
  QTextEdit *m_text;
};

#endif // TEXTMSGBUBBLE_H
