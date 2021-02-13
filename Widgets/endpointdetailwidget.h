#ifndef ENDPOINTDETAILWIDGET_H
#define ENDPOINTDETAILWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QElapsedTimer>
#include <QLabel>
#include "Models/requestmodel.h"

namespace Ui {
class EndpointDetailWidget;
}

class EndpointDetailWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EndpointDetailWidget(QWidget *parent = nullptr);
    ~EndpointDetailWidget();

    void setRequestData(RequestModel data);

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
