#ifndef PARAMMODEL_H
#define PARAMMODEL_H

#include <QString>

struct ParamModel
{
    ParamModel(QString key, QString value, bool enabled = true);

    bool enabled;
    QString key;
    QString value;
};

#endif // PARAMMODEL_H
