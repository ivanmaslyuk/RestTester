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
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &EndpointDetailWidget::tabChanged);
}

EndpointDetailWidget::~EndpointDetailWidget()
{
    delete ui;
}

void EndpointDetailWidget::setRequestData(RequestData data)
{
    ui->paramsTable->setData(data.queryParams);
    ui->endpointTitle->setText(data.displayName);
    ui->headersTable->setData(data.headers);
    ui->requestDataWidget->setParams(data.dataParams);
    ui->requestDataWidget->setRawData(data.rawData);
    // data.method
    ui->urlInput->setText(data.url);
    ui->requestDataWidget->setContentType(data.contentType);
}

void EndpointDetailWidget::makeRequest()
{
    ui->makeRequestButton->setDisabled(true);
    ui->responseText->clear();

    auto request = QNetworkRequest(QUrl(ui->urlInput->text()));
    request.setHeader(QNetworkRequest::UserAgentHeader, "RestTester/0.1");

    if (true) { // TODO: only if not GET or OPTIONS
        request.setHeader(QNetworkRequest::ContentTypeHeader, ui->requestDataWidget->contentType());
    }

    QMap<QString, QString> headers = ui->headersTable->getHeaders();
    for (auto header: headers.keys()) {
        request.setRawHeader(header.toUtf8(), headers[header].toUtf8());
    }

    QByteArray data = ui->requestDataWidget->data();
    this->networkAccessManager->post(request, data);
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

void EndpointDetailWidget::paramsChanged(QString newUrlencoded)
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

void EndpointDetailWidget::tabChanged(int index)
{
    // Auto resize QTabWidget based on tab'c content size
    for(int i=0; i < ui->tabWidget->count(); i++)
        if(i != index)
            ui->tabWidget->widget(i)->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    ui->tabWidget->widget(index)->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    ui->tabWidget->widget(index)->resize(ui->tabWidget->widget(index)->minimumSizeHint());
    ui->tabWidget->widget(index)->adjustSize();
}
