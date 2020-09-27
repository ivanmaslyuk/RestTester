#ifndef ENDPOINTDETAILWIDGET_H
#define ENDPOINTDETAILWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QElapsedTimer>
#include <QLabel>
#include "paramstable.h"

namespace Ui {
class EndpointDetailWidget;
}

struct RequestData {
    RequestData(QString url, QString method, QString displayName, QList<Param> queryParams,
                QList<Param> dataParams, QList<Param> headers, QString rawData,
                QString contentType, QString documentation) {
        this->url = url;
        this->queryParams = queryParams;
        this->dataParams = dataParams;
        this->headers = headers;
        this->rawData = rawData;
        this->method = method;
        this->displayName = displayName;
        this->contentType = contentType;
        this->documentation = documentation;
    }

    QString url;
    QList<Param> queryParams;
    QList<Param> dataParams;
    QList<Param> headers;
    QString rawData;
    QString method;
    QString displayName;
    QString contentType;
    QString documentation;
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
    QElapsedTimer *timer;
    QLabel *responseCodeLabel;
    QLabel *responseTimeLabel;
    QLabel *responseSizeLabel;

    void makeRequest();
    void setupMethodComboBox();
    void setupResponseTabWidget();
    void updateResponseHeadersTable(QList<QPair<QByteArray, QByteArray>> headerPairs);

private slots:
    void makeRequestButtonPressed();
    void requestFinished(QNetworkReply *reply);
    void paramsChanged(QString newUrlencoded);
    void urlEdited(const QString &newUrl);
    void tabChanged(int index);
    void documentationButtonPressed();
};

#endif // ENDPOINTDETAILWIDGET_H
