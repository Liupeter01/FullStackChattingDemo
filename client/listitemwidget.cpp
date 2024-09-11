#include "listitemwidget.h"
#include "ui_listitemwidget.h"
#include "tools.h"

ListItemWidget::ListItemWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ListItemWidget)
{
    ui->setupUi(this);
}

ListItemWidget::~ListItemWidget()
{
    delete ui;
}

QSize ListItemWidget::sizeHint() const
{
    return QSize(width, height);
}

void ListItemWidget::setItemDisplay(const QString &_username, const QString &_avator, const QString &_last_message)
{
    m_username = _username;
    m_avatorpath = _avator;
    m_lastmsg = _last_message;

    QSize size = ui->user_avator->size();
    auto image = Tools::loadImages(m_avatorpath, size.width(), size.height()).value();
    ui->user_avator->setPixmap(QPixmap::fromImage(image));
    ui->last_message->setText(m_lastmsg);
    ui->user_name->setText(m_username);
}
