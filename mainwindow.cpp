#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include <QtNetwork/QNetworkRequest>
#include <QUrl>
#include <QJsonDocument>
#include <QDomDocument>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <Widgets/paramstable.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
//    setWindowFlag(Qt::FramelessWindowHint);
    ui->setupUi(this);

//    this->networkAccessManager = new QNetworkAccessManager(this);
//    connect(this->networkAccessManager, &QNetworkAccessManager::finished, this, &MainWindow::requestFinished);

    ui->titleBar->hide();

//    connect(ui->makeRequestButton, &QPushButton::clicked, this, &MainWindow::makeRequestButtonPressed);
//    connect(ui->urlInput, &QLineEdit::returnPressed, this, &MainWindow::makeRequestButtonPressed);
//    connect(ui->paramsTable, &ParamsTable::urlencodedChanged, this, &MainWindow::paramsChanged);
//    connect(ui->urlInput, &QLineEdit::textEdited, this, &MainWindow::urlEdited);

    // TODO: delete later
//    QList<Param> endpointData;
//    endpointData << Param("key1", "value1") << Param("key2", "value2", false) << Param("key3", "value3");
//    ui->paramsTable->setData(endpointData);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::makeRequest()
//{
//    ui->makeRequestButton->setDisabled(true);
//    ui->responseText->clear();
//    this->networkAccessManager->get(QNetworkRequest(QUrl(ui->urlInput->text())));
//}

//void MainWindow::makeRequestButtonPressed()
//{
//    this->makeRequest();
//}

//void MainWindow::requestFinished(QNetworkReply *reply)
//{
//    ui->makeRequestButton->setDisabled(false);
//    QString text = reply->readAll();

//    QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString().split(';')[0];
//    if (contentType == "application/json") {
//        QJsonDocument doc = QJsonDocument::fromJson(text.toUtf8());
//        text = doc.toJson(QJsonDocument::Indented);
//    }

//    ui->responseText->setPlainText(text);
//}

//void MainWindow::paramsChanged(QString &newUrlencoded)
//{
//    auto currentUrl = ui->urlInput->text().split('?')[0];
//    if (newUrlencoded.isEmpty()) {
//        ui->urlInput->setText(currentUrl);
//    } else{
//        ui->urlInput->setText(currentUrl + "?" + newUrlencoded);
//    }
//}

//void MainWindow::urlEdited(const QString &newUrl)
//{
//    auto urlParts = newUrl.split('?');
//    if (urlParts.length() == 1)
//        urlParts.append("");

//    ui->paramsTable->setUrlencoded(urlParts[1]);
//}
