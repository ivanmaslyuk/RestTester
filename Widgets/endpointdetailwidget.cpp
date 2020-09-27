#include "endpointdetailwidget.h"
#include "ui_endpointdetailwidget.h"
#include <QJsonDocument>
#include <QStyledItemDelegate>
#include <QHBoxLayout>


EndpointDetailWidget::EndpointDetailWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EndpointDetailWidget)
{
    ui->setupUi(this);

    this->timer = new QElapsedTimer();

    this->networkAccessManager = new QNetworkAccessManager(this);
    connect(this->networkAccessManager, &QNetworkAccessManager::finished, this, &EndpointDetailWidget::requestFinished);

    connect(ui->makeRequestButton, &QPushButton::clicked, this, &EndpointDetailWidget::makeRequestButtonPressed);
    connect(ui->urlInput, &QLineEdit::returnPressed, this, &EndpointDetailWidget::makeRequestButtonPressed);
    connect(ui->paramsTable, &ParamsTable::urlencodedChanged, this, &EndpointDetailWidget::paramsChanged);
    connect(ui->urlInput, &QLineEdit::textEdited, this, &EndpointDetailWidget::urlEdited);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &EndpointDetailWidget::tabChanged);
    connect(ui->documentationButton, &QPushButton::clicked, this, &EndpointDetailWidget::documentationButtonPressed);

    setupMethodComboBox();
    setupResponseTabWidget();
    ui->responseHeadersTable->setColumnWidth(0, 200);

    ui->documentationSheet->hide();
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
    ui->documentationSheet->setContent(data.documentation);

    int methodIndex = ui->methodComboBox->findData(data.method);
    if (methodIndex != -1) {
        ui->methodComboBox->setCurrentIndex(methodIndex);
    }
}

void EndpointDetailWidget::makeRequest()
{
    ui->makeRequestButton->setDisabled(true);
    ui->responseText->clear();
    ui->responseHeadersTable->setRowCount(0);

    auto request = QNetworkRequest(QUrl(ui->urlInput->text()));
    request.setHeader(QNetworkRequest::UserAgentHeader, "RestTester/0.1");

    QString method = ui->methodComboBox->currentData().toString();
    bool sendData = method != "GET" && method != "OPTIONS" && method != "HEAD";
    if (sendData) {
        request.setHeader(QNetworkRequest::ContentTypeHeader, ui->requestDataWidget->contentType());
    }

    QMap<QString, QString> headers = ui->headersTable->getHeaders();
    for (auto header: headers.keys()) {
        request.setRawHeader(header.toUtf8(), headers[header].toUtf8());
    }

    timer->start();
    QByteArray data = ui->requestDataWidget->data();
    if (sendData) {
        this->networkAccessManager->sendCustomRequest(request, method.toUtf8(), data);
    } else {
        this->networkAccessManager->sendCustomRequest(request, method.toUtf8());
    }
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

void EndpointDetailWidget::setupResponseTabWidget()
{
    QLabel *label = new QLabel("Response", this);
    label->setStyleSheet("font-size: 16px; color: white; height: 42px; margin-right: 24px; margin-left: 16px; padding: 150px 0;");
    ui->responseTabWidget->setCornerWidget(label, Qt::TopLeftCorner);

    QWidget *responseStatsWidget = new QWidget(this);

    auto layout = new QHBoxLayout(responseStatsWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    responseStatsWidget->setLayout(layout);

    this->responseCodeLabel = new QLabel("", responseStatsWidget);
    this->responseCodeLabel->setStyleSheet("margin-right: 12px; color: rgba(255, 255, 255, 0.5); font-size: 12px;");
    layout->addWidget(this->responseCodeLabel);

    this->responseTimeLabel = new QLabel("", responseStatsWidget);
    this->responseTimeLabel->setStyleSheet("margin-right: 12px; color: rgba(255, 255, 255, 0.5); font-size: 12px;");
    layout->addWidget(this->responseTimeLabel);

    this->responseSizeLabel = new QLabel("", responseStatsWidget);
    this->responseSizeLabel->setStyleSheet("color: rgba(255, 255, 255, 0.5); font-size: 12px;");
    layout->addWidget(this->responseSizeLabel);

    responseStatsWidget->setStyleSheet("padding: 150px 0; margin-right: 16px;");

    ui->responseTabWidget->setCornerWidget(responseStatsWidget, Qt::TopRightCorner);
}

void EndpointDetailWidget::updateResponseHeadersTable(QList<QPair<QByteArray, QByteArray>> headerPairs)
{
    // Insert rows
    for (int i = 0; i < headerPairs.count(); ++i) {
        ui->responseHeadersTable->insertRow(i);

        auto headerName = new QTableWidgetItem(QString::fromUtf8(headerPairs[i].first));
        ui->responseHeadersTable->setItem(i, 0, headerName);

        auto headerValue = new QTableWidgetItem(QString::fromUtf8(headerPairs[i].second));
        ui->responseHeadersTable->setItem(i, 1, headerValue);
    }

    // Resize table to remove scroll bar
    ui->responseHeadersTable->resizeRowsToContents();
    int height = 0;
    for (int i = 0; i < ui->responseHeadersTable->rowCount(); i++) {
        height += ui->responseHeadersTable->rowHeight(i);
    }
    ui->responseHeadersTable->setMinimumHeight(height);
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
    this->responseCodeLabel->setText(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
    this->responseTimeLabel->setText(QString::number(this->timer->elapsed()) + " ms");
    this->responseSizeLabel->setText(QString::number(bodySize + headersSize) + " B");
    updateResponseHeadersTable(reply->rawHeaderPairs());
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

void EndpointDetailWidget::documentationButtonPressed()
{
    if (ui->documentationSheet->isHidden()) {
        ui->documentationSheet->show();
        ui->documentationButton->setText("Hide docs");
    } else {
        ui->documentationSheet->hide();
        ui->documentationButton->setText("Docs");
    }
}
