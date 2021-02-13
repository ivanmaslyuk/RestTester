#ifndef REQUESTMODEL_H
#define REQUESTMODEL_H

#include <QObject>
#include <QList>
#include "parammodel.h"

struct RequestModel {
    RequestModel();

    QString url;
    QList<ParamModel> queryParams;
    QList<ParamModel> dataParams;
    QList<ParamModel> headers;
    QString rawData;
    QString method;
    QString displayName;
    QString contentType;
    QString documentation;
};

Q_DECLARE_METATYPE(RequestModel);

#endif // REQUESTMODEL_H
