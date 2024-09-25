#include "addusernamecarddialog.h"
#include "ui_addusernamecarddialog.h"
#include "adduserrequestdialog.h"

AddUserNameCardDialog::AddUserNameCardDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddUserNameCardDialog)
{
    ui->setupUi(this);

    /*register signal slot*/
    registerSignal();

    /*set window style*/
    setWindowsStatus();
}

AddUserNameCardDialog::~AddUserNameCardDialog()
{
    delete ui;
}

void AddUserNameCardDialog::setupUserInfo(std::unique_ptr<UserNameCard> info)
{
    m_info = std::move(info);
    ui->user_name->setText(m_info->m_nickname);
}

void AddUserNameCardDialog::setWindowsStatus()
{
    setWindowTitle("Add friend");
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

    /*set it to modal*/
    setModal(true);
}

void AddUserNameCardDialog::registerSignal()
{

}

void AddUserNameCardDialog::on_add_friend_button_clicked()
{
    qDebug() << "Add friend button clicked!";

    /*hide current AddUserNameCardDialog*/
    this->hide();

    /*display AddUserRequestDialog UI*/
    AddUserRequestDialog* dialog(new AddUserRequestDialog(this));
    dialog->setModal(true);
    dialog->show();
}

