#include "chattinghistorywidget.h"
#include "tools.h"
#include "ui_chattinghistorywidget.h"

ChattingHistoryWidget::ChattingHistoryWidget(QWidget *parent)
    : ListItemWidgetBase(parent), ui(new Ui::ChattingHistoryWidget) {
  ui->setupUi(this);

  /*set item type to chatting history*/
  this->setItemType(ListItemType::ChattingHistory);
}

ChattingHistoryWidget::~ChattingHistoryWidget() { delete ui; }

void ChattingHistoryWidget::setItemDisplay(const QString &_username,
                                           const QString &_avator,
                                           const QString &_last_message) {
  m_username = _username;
  m_avatorpath = _avator;
  m_lastmsg = _last_message;

  QSize size = ui->user_avator->size();
  auto image =
      Tools::loadImages(m_avatorpath, size.width(), size.height()).value();
  ui->user_avator->setPixmap(QPixmap::fromImage(image));
  ui->last_message->setText(m_lastmsg);
  ui->user_name->setText(m_username);
}
