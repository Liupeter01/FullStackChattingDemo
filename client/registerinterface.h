#ifndef REGISTERINTERFACE_H
#define REGISTERINTERFACE_H

#include <QJsonObject>
#include <map>
#include <functional>
#include <QDialog>
#include <QLabel>

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

private:
    void setRegisterAttribute();
    void registerNetworkEvent();
    void regisrerCallBackFunctions();

private:
    std::map<ServiceType, CallBackFunc> m_callbacks;
    Ui::registerinterface *ui;
};

#endif // REGISTERINTERFACE_H
