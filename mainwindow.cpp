#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include <QtNetwork/QNetworkRequest>
#include <QUrl>
#include <QJsonDocument>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "Widgets/paramstable.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->titleBar->hide();

    // Example request
    QString url = "http://postman-echo.com/post?qenabled=qvalue1";
    QString method = "POST";
    QString displayName = "Postman Echo";
    QList<Param> queryParams;
    queryParams << Param("qenabled", "qvalue1") << Param("qdisabled", "q", false);
    QList<Param> dataParams;
    dataParams << Param("denabled", "dvalue1") << Param("ddisabled", "d", false);
    QList<Param> headers;
    headers << Param("henabled", "hvalue1") << Param("hdisabled", "h", false);
    QString rawData = "{\"key\": 482938473}";
    QString contentType = "application/json";
    QString docs = "# Echo endpoint by Postman\n[Docs on website](https://docs.postman-echo.com/)";
    ui->endpoint->setRequestData(RequestData(
                                     url,
                                     method,
                                     displayName,
                                     queryParams,
                                     dataParams,
                                     headers,
                                     rawData,
                                     contentType,
                                     docs));
}

MainWindow::~MainWindow()
{
    delete ui;
}
