#include "httpnetworkconnection.h"
#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkRequest>

HttpNetworkConnection::HttpNetworkConnection()
{
    registerNetworkEvent();
}

void HttpNetworkConnection::registerNetworkEvent()
{
    connect(this, &HttpNetworkConnection::signal_http_finished, this, &HttpNetworkConnection::slot_http_finished);
}

HttpNetworkConnection::~HttpNetworkConnection()
{

}

void HttpNetworkConnection::postHttpRequest(QUrl url, QJsonObject json, ServiceType srv_type)
{
    auto json_buffer = QJsonDocument(json).toJson();
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, json_buffer.length());

    QNetworkReply *result = m_network.post(request, json_buffer);

    /*we have to guarantee HttpNetworkConnection class won't be destroyed
     * while we are going to handle callback functions
    */
    std::shared_ptr<HttpNetworkConnection> self = shared_from_this();
    connect(result, &QNetworkReply::finished, [this, result, self, srv_type](){
        bool network_status = result->error() != QNetworkReply::NoError;
        if(network_status){
            qDebug() << "result error: " << result->errorString();
        }

        /*emit request success signal*/
        emit this->signal_http_finished(
            srv_type,
            (!network_status ? QString(result->readAll()) : QString("")),
            (!network_status ? ServiceStatus::SERVICE_SUCCESS : ServiceStatus::NETWORK_ERROR)
        );

        result->deleteLater();
        return;
    });
}

void HttpNetworkConnection::slot_http_finished(ServiceType srv_type, QString json_data, ServiceStatus srv_status)
{
    if(srv_type == ServiceType::SERVICE_VERIFICATION){
        emit this->signal_verification_finished(srv_type, json_data, srv_status);
    }
}
