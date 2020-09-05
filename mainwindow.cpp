#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include <QtNetwork/QNetworkRequest>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setWindowFlag(Qt::FramelessWindowHint);
    ui->setupUi(this);

    connect(ui->makeRequestButton, &QPushButton::pressed, this, &MainWindow::makeRequestButtonPressed);
    connect(ui->urlInput, &QLineEdit::returnPressed, this, &MainWindow::makeRequestButtonPressed);
    this->networkAccessManager = new QNetworkAccessManager(this);
    connect(this->networkAccessManager, &QNetworkAccessManager::finished, this, &MainWindow::requestFinished);
    connect(ui->paramsTable, &ParamsTable::urlencodedChanged, this, &MainWindow::paramsChanged);
    connect(ui->urlInput, &QLineEdit::textEdited, this, &MainWindow::urlEdited);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makeRequest()
{
    ui->responseText->clear();
    this->networkAccessManager->get(QNetworkRequest(QUrl(ui->urlInput->text())));
}

void MainWindow::makeRequestButtonPressed()
{
    this->makeRequest();
}

void MainWindow::requestFinished(QNetworkReply *reply)
{
    ui->responseText->setPlainText(reply->readAll());
}

void MainWindow::paramsChanged(QString &newUrlencoded)
{
    auto currentUrl = ui->urlInput->text().split('?')[0];
    if (newUrlencoded.isEmpty()) {
        ui->urlInput->setText(currentUrl);
    } else{
        ui->urlInput->setText(currentUrl + "?" + newUrlencoded);
    }
}

void MainWindow::urlEdited(const QString &newUrl)
{
    auto urlParts = newUrl.split('?');
    if (urlParts.length() == 1)
        urlParts.append("");

    ui->paramsTable->setUrlencoded(urlParts[1]);
}
