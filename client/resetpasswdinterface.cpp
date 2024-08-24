#include "resetpasswdinterface.h"
#include "ui_resetpasswdinterface.h"
#include "httpnetworkconnection.h"

ResetPasswdInterface::ResetPasswdInterface(QWidget *parent)
    : m_countdown(5/*seconds*/)
    , m_timer(new QTimer(this))
    , QDialog(parent)
    , ui(new Ui::ResetPasswdInterface)
{
    ui->setupUi(this);

    /*set password edit echo mode*/
    setResetAttribute();

    /*
     * control back to login timeout setting
     * returning back to login page within 5s
     */
    registerTimeoutSetting();

    /*switch reset info page*/
    switchResetInfoPage();

    /*register lineedit event*/
    registerEditFinishedEvent();
}

ResetPasswdInterface::~ResetPasswdInterface()
{
    delete ui;
}

void ResetPasswdInterface::registerNetworkEvent()
{
    /*
    connect(HttpNetworkConnection::get_instance().get(),
            &HttpNetworkConnection::signal_registeration_finished,
            this, &registerinterface::signal_registeration_finished
            );

    connect(HttpNetworkConnection::get_instance().get(),
            &HttpNetworkConnection::signal_verification_finished,
            this, &registerinterface::signal_verification_finished
            );
    */
}

void ResetPasswdInterface::setResetAttribute()
{
    /*set password editing attribute*/
    this->ui->newpasswd_edit->setEchoMode(QLineEdit::Password);
    this->ui->newconfirm_edit->setEchoMode(QLineEdit::Password);
}

void ResetPasswdInterface::on_go_back_login_2_clicked()
{
    emit switchToLogin();
    return;
}

void ResetPasswdInterface::on_go_back_login_1_clicked()
{
    emit switchToLogin();
    return;
}

void ResetPasswdInterface::on_go_back_to_login3_clicked()
{
    emit switchToLogin();
    return;
}

void ResetPasswdInterface::registerTimeoutSetting()
{
    m_counter = m_countdown;

    connect(m_timer, &QTimer::timeout, [this](){
        ui->timeout_label->setText(QString("returning back to login page within ") + QString::number(m_counter) + 's');
        --m_counter;

        /*reset data display*/
        if(m_counter <= 0){
            m_timer->stop();
            emit switchToLogin();
            return;
        }
    });
}

void ResetPasswdInterface::registerEditFinishedEvent()
{
    /* when user finished editing username info */
    connect(ui->username_edit, &QLineEdit::editingFinished, this, [this](){
        [[maybe_unused]] auto ret = Tools::checkUsername(ui->username_edit, ui->status_label_1);
    });

    connect(ui->email_edit, &QLineEdit::editingFinished, this, [this](){
        [[maybe_unused]] auto ret = Tools::checkEmail(ui->email_edit, ui->status_label_1);
    });

    /* when user finished editing password info */
    connect(ui->newpasswd_edit, &QLineEdit::editingFinished, this, [this](){
        [[maybe_unused]] auto ret = Tools::checkPassword(ui->email_edit, ui->status_label_2);
    });

    connect(ui->newconfirm_edit, &QLineEdit::editingFinished, this, [this](){
        [[maybe_unused]] auto ret = Tools::checkSimilarity(ui->newpasswd_edit, ui->newconfirm_edit, ui->status_label_2);
    });
}

/*
 * switch to registeration successful page
 * by using stackedWidget switch to successful_page
 */
void ResetPasswdInterface::switchResetInfoPage()
{
    ui->stackedWidget->setCurrentWidget(ui->reset_page);
}

void ResetPasswdInterface::switchResetPasswordPage()
{
    ui->stackedWidget->setCurrentWidget(ui->passwd_page);
}

void ResetPasswdInterface::switchResetSuccessfulPage()
{
    ui->stackedWidget->setCurrentWidget(ui->successful_page);
    m_timer->start(1000/*default time interval = 1000ms(1s)*/);
}
