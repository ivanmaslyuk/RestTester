#include "paramstable.h"
#include "ui_paramstable.h"
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QWidget>
#include <QDebug>

ParamsTable::ParamsTable(QWidget *parent) :
    QTableWidget(parent),
    ui(new Ui::ParamsTable)
{
    ui->setupUi(this);

    // TODO: ПРИНИМАТЬ this.data ЗДЕСЬ

    this->horizontalHeader()->setDefaultAlignment(Qt::AlignVCenter);
    this->horizontalHeader()->setStyleSheet("::section{padding-left: 19px; border: none; background-color: transparent; color: white; font-size: 10pt; font-weight: bold; height: 30px}");

    this->setRowCount(0);
    this->setColumnCount(3);

    QStringList columnTitles;
    columnTitles << "" << "Key" << "Value" << "Value";
    this->setHorizontalHeaderLabels(columnTitles);

    this->setRowHeight(0, 30);

    this->horizontalHeader()->setMinimumSectionSize(28);
    this->setColumnWidth(0, 28);

    this->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    this->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    this->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    this->appendRow();

    this->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);
}

ParamsTable::~ParamsTable()
{
    delete ui;
}

void ParamsTable::setUrlencoded(QString &urlencoded)
{
    QStringList params = urlencoded.split('&');
    this->data.clear();
    this->setRowCount(0);
    // TODO: memory leak of previous input widgets???
    for (auto param = params.begin(); param != params.end(); ++param)
    {
        if (param->isEmpty()) continue;

        auto keyAndValue = param->split('=');
        keyAndValue.append("");

        QString key = QUrl::fromPercentEncoding(keyAndValue[0].toUtf8());
        QString value = QUrl::fromPercentEncoding(keyAndValue[1].toUtf8());

        appendRow(key, value);
    }

    appendRow();
}

void ParamsTable::appendRow(QString key, QString value)
{
    auto insertAt = this->rowCount();
    this->insertRow(insertAt);

    auto checkBox = new QCheckBox(this);
    checkBox->setCheckState(Qt::Checked);
    checkBox->setVisible(false);
    checkBox->setStyleSheet("QCheckBox {border-top: 1px solid #777; padding: 9px 8px} QCheckBox::indicator {border-radius: 5px}");
    this->setCellWidget(insertAt, 0, checkBox);

    auto keyInput = new QLineEdit(this);
    keyInput->setPlaceholderText("Enter key");
    keyInput->setStyleSheet("color: white; padding-left: 16px; border-top: 1px solid #777; font-size: 10pt");
    this->setCellWidget(insertAt, 1, keyInput);
    keyInput->setText(key);

    auto valueInput = new QLineEdit(this);
    valueInput->setPlaceholderText("Enter value");
    valueInput->setStyleSheet("color: white; padding-left: 16px; border-top: 1px solid #777; font-size: 10pt");
    this->setCellWidget(insertAt, 2, valueInput);
    valueInput->setText(value);

    this->setRowHeight(insertAt, 30);

    this->data.append(Param(key, value, true));

    connect(keyInput, &QLineEdit::textEdited, this, [=](QString newValue){this->keyEdited(insertAt, newValue);});
    connect(valueInput, &QLineEdit::textEdited, this, [=](QString newValue){this->valueEdited(insertAt, newValue);});
    connect(checkBox, &QCheckBox::stateChanged, this, [=](int newState){this->paramCheckStateChanged(insertAt, newState);});
}

void ParamsTable::keyEdited(int row, QString &newKey)
{
    this->data[row].key = newKey;
    if (row == this->rowCount() - 1) {
        appendRow();
    }
    sendNewUrlencodedData();
    sendNewHeaders();
}

void ParamsTable::valueEdited(int row, QString &newValue)
{
    this->data[row].value = newValue;
    if (row == this->rowCount() - 1) {
        appendRow();
    }
    sendNewUrlencodedData();
    sendNewHeaders();
}

void ParamsTable::paramCheckStateChanged(int row, int newCheckState)
{
    auto keyInput = this->cellWidget(row, 1);
    auto valueInput = this->cellWidget(row, 2);

    if (newCheckState == Qt::Checked) {
        keyInput->setStyleSheet("color: white; padding-left: 16px; border-top: 1px solid #777; font-size: 10pt");
        valueInput->setStyleSheet("color: white; padding-left: 16px; border-top: 1px solid #777; font-size: 10pt");
        this->data[row].enabled = true;
    } else {
        keyInput->setStyleSheet("color: rgba(255, 255, 255, 0.5); padding-left: 16px; border-top: 1px solid #777; font-size: 10pt");
        valueInput->setStyleSheet("color: rgba(255, 255, 255, 0.5); padding-left: 16px; border-top: 1px solid #777; font-size: 10pt");
        this->data[row].enabled = false;
    }

    sendNewUrlencodedData();
    sendNewHeaders();
}

void ParamsTable::sendNewUrlencodedData()
{
    QString urlencoded;
    for (auto i = this->data.begin(); i != this->data.end() - 1; ++i)
    {
        if (i->key.isEmpty() && i->value.isEmpty()) continue;

        if (!urlencoded.isEmpty()) {
            urlencoded += "&";
        }
        urlencoded += QUrl::toPercentEncoding(i->key) + "=" + QUrl::toPercentEncoding(i->value);
    }

    emit this->urlencodedChanged(urlencoded);
}

void ParamsTable::sendNewHeaders()
{
    QMap<QString, QString> headers;

    for (auto i = this->data.begin(); i != this->data.end() - 1; ++i)
    {
        headers[i->key] = i->value;
    }

    emit this->headersChanged(headers);
}
