#ifndef SERVERAPI_H
#define SERVERAPI_H

#include <QObject>
#include <QUrl>
#include <QDebug>

struct Api {
    Api() {}
    Api(int id, QString name) {
        this->id = id;
        this->name = name;
    }

    int id;
    QString name;
};

inline QDebug operator<<(QDebug dbg, const Api &api) {
    dbg.nospace() << "Api(" + QString::number(api.id) + ", " + api.name + ")";

    return dbg.space();
}

inline QDebug operator<<(QDebug dbg, const Api *api) {
    dbg.nospace() << "Api(" + QString::number(api->id) + ", " + api->name + ")";

    return dbg.space();
}

Q_DECLARE_METATYPE(Api);

class ServerAPI : public QObject
{
    Q_OBJECT
public:
    explicit ServerAPI(QObject *parent = nullptr);
    QList<Api> getApisListForCurrentUser();
    QList<QString> getApiHierarchy(int apiId);
private:
    QString serverBaseUrl;
    QString password;
    QString username;

    void checkCredentials();
signals:

};

#endif // SERVERAPI_H
