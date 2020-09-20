#ifndef PARAMSTABLE_H
#define PARAMSTABLE_H

#include <QTableWidget>
#include <QList>

namespace Ui {
class ParamsTable;
}

struct Param
{
    Param(QString key, QString value, bool enabled = true)
    {
        this->key = key;
        this->value = value;
        this->enabled = enabled;
    }

    bool enabled;
    QString key;
    QString value;
};

class ParamsTable : public QTableWidget
{
    Q_OBJECT

public:
    explicit ParamsTable(QWidget *parent = nullptr);
    ~ParamsTable();

    void setUrlencoded(QString &urlencoded);
    void setData(QList<Param> newData);

private:
    Ui::ParamsTable *ui;
    QList<Param> data;

    void appendRow(QString key = "", QString value = "", bool enabled = true);
    void keyEdited(int row, QString &newKey);
    void valueEdited(int row, QString &newValue);
    void paramCheckStateChanged(int row, int newCheckState);
    void sendNewUrlencodedData();
    void sendNewHeaders();
    QList<Param> getDisabledParams();

signals:
    void urlencodedChanged(QString &newValue);
    void headersChanged(QMap<QString, QString> &newHeaders);
};

#endif // PARAMSTABLE_H
