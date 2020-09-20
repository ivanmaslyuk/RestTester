#include "requestdatainputwidget.h"
#include "ui_requestdatainputwidget.h"
#include <QComboBox>
#include <QDebug>

RequestDataInputWidget::RequestDataInputWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RequestDataInputWidget)
{
    ui->setupUi(this);

    ui->tableWidget = ui->tableWidget;
    ui->textEdit = ui->textEdit;

    connect(ui->contentTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &RequestDataInputWidget::contentTypeChanged);

    initContentTypeChoices();
}

RequestDataInputWidget::~RequestDataInputWidget()
{
    delete ui;
}

QString RequestDataInputWidget::contentType()
{
    return ui->contentTypeComboBox->currentData().toString();
}

QByteArray RequestDataInputWidget::data()
{
    auto type = ui->contentTypeComboBox->currentData().toString();

    if (type == "application/x-www-form-urlencoded") {
        return ui->tableWidget->getUrlencoded().toUtf8();
    }

    return ui->textEdit->toPlainText().toUtf8();
}

void RequestDataInputWidget::setParams(QList<Param> params)
{
    ui->tableWidget->setData(params);
}

void RequestDataInputWidget::setRawData(QString data)
{
    ui->textEdit->setPlainText(data);
}

void RequestDataInputWidget::setContentType(QString type)
{
    int index = ui->contentTypeComboBox->findData(type);
    if (index == -1) {
        return;
    }

    ui->contentTypeComboBox->setCurrentIndex(index);
}

void RequestDataInputWidget::initContentTypeChoices()
{
    ui->contentTypeComboBox->addItem("Urlencoded", "application/x-www-form-urlencoded");
    ui->contentTypeComboBox->addItem("JSON", "application/json");
    ui->contentTypeComboBox->addItem("Text", "text/plain");
    ui->contentTypeComboBox->addItem("HTML", "text/html");

    ui->contentTypeComboBox->setCurrentIndex(0);
}

void RequestDataInputWidget::contentTypeChanged(int index)
{
    auto type = ui->contentTypeComboBox->itemData(index);

    if (type == "application/x-www-form-urlencoded") {
        ui->textEdit->hide();
        ui->tableWidget->show();
    } else {
        ui->textEdit->show();
        ui->tableWidget->hide();
    }
}
