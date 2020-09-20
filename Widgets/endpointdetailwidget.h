#ifndef ENDPOINTDETAILWIDGET_H
#define ENDPOINTDETAILWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class EndpointDetailWidget;
}

class EndpointDetailWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EndpointDetailWidget(QWidget *parent = nullptr);
    ~EndpointDetailWidget();

private:
    Ui::EndpointDetailWidget *ui;
    QNetworkAccessManager *networkAccessManager;

    void makeRequest();

private slots:
    void makeRequestButtonPressed();
    void requestFinished(QNetworkReply *reply);
    void paramsChanged(QString &newUrlencoded);
    void urlEdited(const QString &newUrl);
};

#endif // ENDPOINTDETAILWIDGET_H
