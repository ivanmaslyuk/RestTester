#ifndef NODEMODEL_H
#define NODEMODEL_H

#include <QList>
#include <QUuid>
#include "Models/requestmodel.h"

struct NodeModel
{
    NodeModel();

    enum NodeType {
        Folder,
        Request
    };

    QUuid uuid;
    QList<NodeModel> children;
    NodeType type;
    RequestModel request;
    QString folderName;
};

#endif // NODEMODEL_H
