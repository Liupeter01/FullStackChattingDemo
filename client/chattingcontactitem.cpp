#include "tools.h"
#include "chattingcontactitem.h"
#include "ui_chattingcontactitem.h"

ChattingContactItem::ChattingContactItem(QWidget *parent)
    : static_text("Add New Friend")
    , ListItemWidgetBase(parent)
    , ui(new Ui::ChattingContactItem)
{
    ui->setupUi(this);

    /*set list item type, default value = ContactHistory*/
    setItemType(ListItemType::ContactHistory);
}

ChattingContactItem::~ChattingContactItem()
{
}

QSize ChattingContactItem::sizeHint() const
{
    return m_size;
}

void ChattingContactItem::setAddUserWidget()
{
    /*set list item type, AddUserWidget*/
    setItemType(ListItemType::AddUserWidget);

    /*setup avator by static label*/
    Tools::setQLableImage(ui->notification_label, "add_friend_clicked.png");

    /*setup text by static text*/
    ui->display_label->setText(static_text);

    /*hide group zone*/
    ui->contact_zone->show();
    ui->group_zone->hide();

    /*set qss style*/
    //ui->group_zone->setStyleSheet(
    //    QString("#group_zone{color:while;font-size:14px;"
    //            "font-family: \"Microsoft YaHei\""));

    /*update contact zone's size*/
    m_size = ui->contact_zone->size();
}

void ChattingContactItem::setChattingContact(const QString &target_picture, const QString &text)
{
    /*set list item type, ContactHistory*/
    setItemType(ListItemType::ContactHistory);

    /*setup avator by static label*/
    Tools::setQLableImage(ui->notification_label, target_picture);

    /*setup text by static text*/
    ui->display_label->setText(text);

    /*hide group zone*/
    ui->contact_zone->show();
    ui->group_zone->hide();

    /*set qss style*/
    //ui->contact_zone->setStyleSheet(
    //    QString("#group_label{color:while;font-size:14px;"
    //            "font-family: \"Microsoft YaHei\""));

    /*update contact zone's size*/
    m_size = ui->contact_zone->size();
}

void ChattingContactItem::setGroupSeperator(const QString& text)
{
    /*set list item type, ShowContactByGroup*/
    setItemType(ListItemType::ShowContactByGroup);

    /*setup text*/
    ui->group_label->setText(text);

    /*hide contact zone*/
    ui->contact_zone->hide();
    ui->group_zone->show();

   /*set qss style*/
    ui->group_zone->setStyleSheet(
        QString("#group_zone Qwidget{background-color:while;font-size:14px;"
                "font-family: \"Microsoft YaHei\""));

    /*update group zone's size*/
    m_size = ui->group_zone->size();
}
