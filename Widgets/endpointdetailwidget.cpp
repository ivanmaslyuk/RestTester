#include "endpointdetailwidget.h"
#include "ui_endpointdetailwidget.h"
#include <QJsonDocument>
#include <QStyledItemDelegate>


EndpointDetailWidget::EndpointDetailWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EndpointDetailWidget)
{
    ui->setupUi(this);

    ui->responseCodeLabel->setText("");
    ui->responseTimeLabel->setText("");
    ui->responseSizeLabel->setText("");
    this->timer = new QElapsedTimer();

    this->networkAccessManager = new QNetworkAccessManager(this);
    connect(this->networkAccessManager, &QNetworkAccessManager::finished, this, &EndpointDetailWidget::requestFinished);

    connect(ui->makeRequestButton, &QPushButton::clicked, this, &EndpointDetailWidget::makeRequestButtonPressed);
    connect(ui->urlInput, &QLineEdit::returnPressed, this, &EndpointDetailWidget::makeRequestButtonPressed);
    connect(ui->paramsTable, &ParamsTable::urlencodedChanged, this, &EndpointDetailWidget::paramsChanged);
    connect(ui->urlInput, &QLineEdit::textEdited, this, &EndpointDetailWidget::urlEdited);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &EndpointDetailWidget::tabChanged);

    setupMethodComboBox();
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
    ui->urlInput->setText(data.url);
    ui->requestDataWidget->setContentType(data.contentType);

    int methodIndex = ui->methodComboBox->findData(data.method);
    if (methodIndex != -1) {
        ui->methodComboBox->setCurrentIndex(methodIndex);
    }
}

void EndpointDetailWidget::makeRequest()
{
    ui->makeRequestButton->setDisabled(true);
    ui->responseText->clear();

    auto request = QNetworkRequest(QUrl(ui->urlInput->text()));
    request.setHeader(QNetworkRequest::UserAgentHeader, "RestTester/0.1");

    QString method = ui->methodComboBox->currentData().toString();
    if (method != "GET" && method != "OPTIONS" && method != "HEAD") {
        request.setHeader(QNetworkRequest::ContentTypeHeader, ui->requestDataWidget->contentType());
    }

    QMap<QString, QString> headers = ui->headersTable->getHeaders();
    for (auto header: headers.keys()) {
        request.setRawHeader(header.toUtf8(), headers[header].toUtf8());
    }

    timer->start();
    QByteArray data = ui->requestDataWidget->data();
    this->networkAccessManager->sendCustomRequest(request, method.toUtf8(), data);
}

void EndpointDetailWidget::setupMethodComboBox()
{
    QStyledItemDelegate* itemDelegate = new QStyledItemDelegate();
    ui->methodComboBox->setItemDelegate(itemDelegate);

    ui->methodComboBox->addItem("GET", "GET");
    ui->methodComboBox->addItem("POST", "POST");
    ui->methodComboBox->addItem("PUT", "PUT");
    ui->methodComboBox->addItem("PATCH", "PATCH");
    ui->methodComboBox->addItem("DELETE", "DELETE");
    ui->methodComboBox->addItem("HEAD", "HEAD");
    ui->methodComboBox->addItem("OPTIONS", "OPTIONS");
}

void EndpointDetailWidget::makeRequestButtonPressed()
{
    this->makeRequest();
}

void EndpointDetailWidget::requestFinished(QNetworkReply *reply)
{
    QByteArray body = reply->readAll();

    // Calculate approximate size of headers and body
    qint64 bodySize = body.size();
    qint64 headersSize = 0;
    for (auto headerPair: reply->rawHeaderPairs()) {
        auto header = headerPair.first + ": " + headerPair.second;
        headersSize += header.size();
    }

    // Prettify response if possible
    QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString().split(';')[0];
    if (contentType == "application/json") {
        QJsonDocument doc = QJsonDocument::fromJson(body);
        body = doc.toJson(QJsonDocument::Indented);
    }

    // Update UI
    ui->makeRequestButton->setDisabled(false);
    ui->responseText->setPlainText(body);
    ui->responseCodeLabel->setText(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
    ui->responseTimeLabel->setText(QString::number(this->timer->elapsed()) + " ms");
    ui->responseSizeLabel->setText(QString::number(bodySize + headersSize) + " B");
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
