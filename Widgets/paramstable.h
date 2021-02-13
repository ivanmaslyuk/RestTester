#ifndef PARAMSTABLE_H
#define PARAMSTABLE_H

#include <QTableWidget>
#include <QList>
#include "Models/parammodel.h"

namespace Ui {
class ParamsTable;
}

class ParamsTable : public QTableWidget
{
    Q_OBJECT

public:
    explicit ParamsTable(QWidget *parent = nullptr);
    ~ParamsTable();

    void setUrlencoded(QString &urlencoded);
    void setData(QList<ParamModel> newData);
    QString getUrlencoded();
    QMap<QString, QString> getHeaders();

private:
    Ui::ParamsTable *ui;
    QList<ParamModel> data;

    void appendRow(QString key = "", QString value = "", bool enabled = true);
    void keyEdited(int row, QString &newKey);
    void valueEdited(int row, QString &newValue);
    void paramCheckStateChanged(int row, int newCheckState);
    void sendNewData();
    QList<ParamModel> getDisabledParams();

signals:
    void urlencodedChanged(QString newValue);
    void headersChanged(QMap<QString, QString> newHeaders);
};

#endif // PARAMSTABLE_H
