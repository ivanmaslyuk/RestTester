#ifndef APIMODEL_H
#define APIMODEL_H

#include <QList>
#include <QUuid>
#include "Models/nodemodel.h"

struct ApiModel
{
    ApiModel();

    QUuid uuid;
    QList<NodeModel> nodes;
    QString name;
};

#endif // APIMODEL_H
