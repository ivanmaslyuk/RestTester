#include "endpointdetailwidget.h"
#include "ui_endpointdetailwidget.h"
#include <QJsonDocument>


EndpointDetailWidget::EndpointDetailWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EndpointDetailWidget)
{
    ui->setupUi(this);

    this->networkAccessManager = new QNetworkAccessManager(this);
    connect(this->networkAccessManager, &QNetworkAccessManager::finished, this, &EndpointDetailWidget::requestFinished);

    connect(ui->makeRequestButton, &QPushButton::clicked, this, &EndpointDetailWidget::makeRequestButtonPressed);
    connect(ui->urlInput, &QLineEdit::returnPressed, this, &EndpointDetailWidget::makeRequestButtonPressed);
    connect(ui->paramsTable, &ParamsTable::urlencodedChanged, this, &EndpointDetailWidget::paramsChanged);
    connect(ui->urlInput, &QLineEdit::textEdited, this, &EndpointDetailWidget::urlEdited);
}

EndpointDetailWidget::~EndpointDetailWidget()
{
    delete ui;
}

void EndpointDetailWidget::makeRequest()
{
    ui->makeRequestButton->setDisabled(true);
    ui->responseText->clear();
    this->networkAccessManager->get(QNetworkRequest(QUrl(ui->urlInput->text())));
}

void EndpointDetailWidget::makeRequestButtonPressed()
{
    this->makeRequest();
}

void EndpointDetailWidget::requestFinished(QNetworkReply *reply)
{
    ui->makeRequestButton->setDisabled(false);
    QString text = reply->readAll();

    QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString().split(';')[0];
    if (contentType == "application/json") {
        QJsonDocument doc = QJsonDocument::fromJson(text.toUtf8());
        text = doc.toJson(QJsonDocument::Indented);
    }

    ui->responseText->setPlainText(text);
}

void EndpointDetailWidget::paramsChanged(QString &newUrlencoded)
{
    auto currentUrl = ui->urlInput->text().split('?')[0];
    if (newUrlencoded.isEmpty()) {
        ui->urlInput->setText(currentUrl);
    } else{
        ui->urlInput->setText(currentUrl + "?" + newUrlencoded);
    }
}

void EndpointDetailWidget::urlEdited(const QString &newUrl)
{
    auto urlParts = newUrl.split('?');
    if (urlParts.length() == 1)
        urlParts.append("");

    ui->paramsTable->setUrlencoded(urlParts[1]);
}
