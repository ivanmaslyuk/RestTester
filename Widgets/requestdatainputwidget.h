#ifndef REQUESTDATAINPUTWIDGET_H
#define REQUESTDATAINPUTWIDGET_H

#include <QWidget>
#include "paramstable.h"

namespace Ui {
class RequestDataInputWidget;
}

class RequestDataInputWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RequestDataInputWidget(QWidget *parent = nullptr);
    ~RequestDataInputWidget();

    QString contentType();
    QByteArray data();
    void setParams(QList<Param> params);
    void setRawData(QString data);
    void setContentType(QString type);

private:
    Ui::RequestDataInputWidget *ui;

    void initContentTypeChoices();

private slots:
    void contentTypeChanged(int index);
};

#endif // REQUESTDATAINPUTWIDGET_H
