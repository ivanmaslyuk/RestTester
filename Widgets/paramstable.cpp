#include "paramstable.h"
#include "ui_paramstable.h"
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QWidget>
#include <QDebug>
#include <QUrl>

ParamsTable::ParamsTable(QWidget *parent) :
    QTableWidget(parent),
    ui(new Ui::ParamsTable)
{
    ui->setupUi(this);

    this->setRowCount(0);
    this->setColumnCount(3);

    this->horizontalHeader()->setMinimumSectionSize(28);
    this->setColumnWidth(0, 28);

    this->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    this->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    this->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    this->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

    this->appendRow();
}

ParamsTable::~ParamsTable()
{
    delete ui;
}

void ParamsTable::setUrlencoded(QString &urlencoded)
{
    QList<ParamModel> newData;
    QStringList params = urlencoded.split('&');

    for (auto param: params)
    {
        if (param.isEmpty()) continue;

        auto keyAndValue = param.split('=');
        keyAndValue.append("");

        QString key = QUrl::fromPercentEncoding(keyAndValue[0].toUtf8());
        QString value = QUrl::fromPercentEncoding(keyAndValue[1].toUtf8());
        newData.append(ParamModel(key, value));
    }

    newData.append(getDisabledParams());
    setData(newData);
}

void ParamsTable::setData(QList<ParamModel> newData)
{
    this->data.clear();
    setRowCount(0);
    for (auto param: newData) {
        appendRow(param.key, param.value, param.enabled);
    }

    appendRow();
}

QString ParamsTable::getUrlencoded()
{
    QString urlencoded;
    for (auto param: this->data)
    {
        if (param.key.isEmpty() && param.value.isEmpty()) continue;
        if (!param.enabled) continue;

        if (!urlencoded.isEmpty()) {
            urlencoded += "&";
        }
        urlencoded += QUrl::toPercentEncoding(param.key) + "=" + QUrl::toPercentEncoding(param.value);
    }

    return urlencoded;
}

QMap<QString, QString> ParamsTable::getHeaders()
{
    QMap<QString, QString> headers;

    for (auto param: this->data)
    {
        if (!param.enabled) continue;

        headers[param.key] = param.value;
    }

    return headers;
}

void ParamsTable::appendRow(QString key, QString value, bool enabled)
{
    auto insertAt = this->rowCount();
    this->insertRow(insertAt);

    auto checkBox = new QCheckBox(this);
    checkBox->setCheckState(enabled ? Qt::Checked : Qt::Unchecked);
    checkBox->setVisible(false);
    checkBox->setStyleSheet("QCheckBox {padding: 9px 8px}");
    this->setCellWidget(insertAt, 0, checkBox);

    auto keyInput = new QLineEdit(this);
    keyInput->setPlaceholderText("Enter key");
    keyInput->setProperty("cssClass", "keyInput");
    this->setCellWidget(insertAt, 1, keyInput);
    keyInput->setText(key);

    auto valueInput = new QLineEdit(this);
    valueInput->setPlaceholderText("Enter value");
    this->setCellWidget(insertAt, 2, valueInput);
    valueInput->setText(value);

    if (enabled) {
        keyInput->setStyleSheet("color: white; padding-left: 12px; font-size: 12px");
        valueInput->setStyleSheet("color: white; padding-left: 12px; font-size: 12px");
    } else {
        keyInput->setStyleSheet("color: rgba(255, 255, 255, 0.5); padding-left: 12px; font-size: 12px");
        valueInput->setStyleSheet("color: rgba(255, 255, 255, 0.5); padding-left: 12px; font-size: 12px");
    }

    this->setRowHeight(insertAt, 30);

    this->data.append(ParamModel(key, value, enabled));

    connect(checkBox, &QCheckBox::stateChanged, [=](int newState){paramCheckStateChanged(insertAt, newState);});
    connect(keyInput, &QLineEdit::textEdited, [=](QString newValue){keyEdited(insertAt, newValue);});
    connect(valueInput, &QLineEdit::textEdited, [=](QString newValue){valueEdited(insertAt, newValue);});
}

void ParamsTable::keyEdited(int row, QString &newKey)
{
    this->data[row].key = newKey;
    if (row == this->rowCount() - 1) {
        appendRow();
    }
    sendNewData();
    scrollToBottom();
}

void ParamsTable::valueEdited(int row, QString &newValue)
{
    this->data[row].value = newValue;
    if (row == this->rowCount() - 1) {
        appendRow();
    }
    sendNewData();
    scrollToBottom();
}

void ParamsTable::paramCheckStateChanged(int row, int newCheckState)
{
    auto keyInput = this->cellWidget(row, 1);
    auto valueInput = this->cellWidget(row, 2);

    if (newCheckState == Qt::Checked) {
        keyInput->setStyleSheet("color: white; padding-left: 12px; font-size: 12px");
        valueInput->setStyleSheet("color: white; padding-left: 12px; font-size: 12px");
        this->data[row].enabled = true;
    } else {
        keyInput->setStyleSheet("color: rgba(255, 255, 255, 0.5); padding-left: 12px; font-size: 12px");
        valueInput->setStyleSheet("color: rgba(255, 255, 255, 0.5); padding-left: 12px; font-size: 12px");
        this->data[row].enabled = false;
    }

    sendNewData();
}

void ParamsTable::sendNewData()
{
    emit this->urlencodedChanged(getUrlencoded());
    emit this->headersChanged(getHeaders());
}

QList<ParamModel> ParamsTable::getDisabledParams()
{
    QList<ParamModel> result;

    for (auto param: this->data) {
        if (!param.enabled) {
            result.append(param);
        }
    }

    return result;
}
