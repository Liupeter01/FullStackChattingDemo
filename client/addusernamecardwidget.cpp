#include "addusernamecardwidget.h"
#include "ui_addusernamecardwidget.h"

AddUserNameCardWidget::AddUserNameCardWidget(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::AddUserNameCardWidget)
{
    ui->setupUi(this);

    /*registerSignal*/
    registerSignal();
}

AddUserNameCardWidget::~AddUserNameCardWidget()
{
    delete ui;
}

void AddUserNameCardWidget::setFriendName(const QString &name)
{

}

void AddUserNameCardWidget::setFriendAvator(const QPixmap &pic)
{

}

void AddUserNameCardWidget::setRequestMsg(const QString &msg)
{

}

QSize AddUserNameCardWidget::sizeHint() const
{
    return QSize(width, height);
}

void AddUserNameCardWidget::registerSignal()
{
    connect(ui->close_label, &OnceClickableQLabel::clicked, this, &AddUserNameCardWidget::signal_close_clicked);
}

void AddUserNameCardWidget::slot_close_clicked()
{

}
