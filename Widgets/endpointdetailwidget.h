#ifndef ENDPOINTDETAILWIDGET_H
#define ENDPOINTDETAILWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "paramstable.h"

namespace Ui {
class EndpointDetailWidget;
}

struct RequestData {
    RequestData(QString url, QString method, QString displayName, QList<Param> queryParams,
                QList<Param> dataParams, QList<Param> headers, QString rawData,
                QString contentType) {
        this->url = url;
        this->queryParams = queryParams;
        this->dataParams = dataParams;
        this->headers = headers;
        this->rawData = rawData;
        this->method = method;
        this->displayName = displayName;
        this->contentType = contentType;
    }

    QString url;
    QList<Param> queryParams;
    QList<Param> dataParams;
    QList<Param> headers;
    QString rawData;
    QString method;
    QString displayName;
    QString contentType;
};

class EndpointDetailWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EndpointDetailWidget(QWidget *parent = nullptr);
    ~EndpointDetailWidget();

    void setRequestData(RequestData data);

private:
    Ui::EndpointDetailWidget *ui;
    QNetworkAccessManager *networkAccessManager;

    void makeRequest();
    void setupMethodComboBox();

private slots:
    void makeRequestButtonPressed();
    void requestFinished(QNetworkReply *reply);
    void paramsChanged(QString newUrlencoded);
    void urlEdited(const QString &newUrl);
    void tabChanged(int index);
};

#endif // ENDPOINTDETAILWIDGET_H
