#include "serverapi.h"

#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkReply>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "Widgets/endpointdetailwidget.h"

ServerAPI::ServerAPI(QObject *parent) : QObject(parent)
{
    this->serverBaseUrl = "http://localhost:8000/";
    this->username = "admin";
    this->password = "admin";
}

QList<Api> ServerAPI::getApisListForCurrentUser()
{
    this->checkCredentials();

    QNetworkAccessManager* qnam = new QNetworkAccessManager(this);
    connect(qnam, &QNetworkAccessManager::finished, [=](QNetworkReply *reply){
        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200) {
            qDebug() << QString("Could not reach server");
            return;
        }

        QJsonDocument jsonDoc(QJsonDocument::fromJson(reply->readAll()));
        QList<Api> apis;
        for (QJsonValueRef apiRef : jsonDoc.array()) {
            QJsonObject apiObject = apiRef.toObject();
            int id = apiObject["id"].toInt();
            QString name = apiObject["name"].toString();
            apis.append(Api(id, name));
        }
        qDebug() << apis;
    });

    QNetworkRequest request(this->serverBaseUrl + "api/apis/");
    // Basic auth
    QString concatenated = this->username + ':' + this->password;
    QByteArray authData = concatenated.toLocal8Bit().toBase64();
    QString authHeader = "Basic " + authData;
    request.setRawHeader("Authorization", authHeader.toLocal8Bit());

    qnam->get(request);

    return QList<Api>();
}

QList<QString> ServerAPI::getApiHierarchy(int apiId)
{
    this->checkCredentials();
    return QList<QString>();
}

void ServerAPI::checkCredentials()
{
    if (this->serverBaseUrl.isEmpty()) {
        throw "Server url not set";
    }
    if (this->username.isEmpty() || this->password.isEmpty()) {
        throw "Username and password are not set";
    }
}
