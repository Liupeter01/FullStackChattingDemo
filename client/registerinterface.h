#ifndef REGISTERINTERFACE_H
#define REGISTERINTERFACE_H

#include <QLabel>
#include <QImage>
#include "tools.h"
#include <QDialog>
#include <functional>
#include <QJsonObject>

namespace Ui {
class registerinterface;
}

/*declaration of network events*/
enum class ServiceType : uint8_t;
enum class ServiceStatus : uint8_t;

class registerinterface : public QDialog
{
    Q_OBJECT

    using CallBackFunc = std::function<void(QJsonObject &&json)>;

public:
    explicit registerinterface(QWidget *parent = nullptr);
    ~registerinterface();

signals:
    void switchToLogin();

private slots:
    void on_verification_button_clicked();
    void signal_registeration_finished(
        ServiceType srv_type,
        QString json_data,
        ServiceStatus srv_status
    );
    void signal_verification_finished(
        ServiceType srv_type,
        QString json_data,
        ServiceStatus srv_status
    );

    void on_confirm_button_clicked();

private:
    void registerSignal();

    /*
     * control back to login timeout setting
     * returning back to login page within 5s
     */
    void registerTimeoutSetting();

    void setRegisterAttribute();
    void registerEditFinishedEvent();
    void registerNetworkEvent();
    void regisrerCallBackFunctions();

    /*
     * switch to registeration successful page
     * by using stackedWidget switch to
    */
    void switchRegInfoPage();
    void switchRegSucessfulPage();

private:
    std::map<ServiceType, CallBackFunc> m_callbacks;
    Ui::registerinterface *ui;

    /*when registeration successful, user might have 5 seconds to close the registeration page*/
    QTimer* m_timer;
    const std::size_t m_countdown;
    std::size_t m_counter;
};

#endif // REGISTERINTERFACE_H
