#include "treemodel.h"
#include <QVariant>
#include "Models/requestmodel.h"

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    rootItem = new TreeItem("");
    setupModelData(rootItem);
}

TreeModel::~TreeModel()
{
    delete rootItem;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

void TreeModel::setupModelData(TreeItem *parent)
{
    RequestModel topLevelFolder;
    topLevelFolder.displayName = "Top level folder";
    TreeItem *folder1Item = new TreeItem(QVariant::fromValue(topLevelFolder), parent);
    parent->appendChild(folder1Item);

    RequestModel folder2;
    folder2.displayName = "Folder";
    TreeItem *folder2Item = new TreeItem(QVariant::fromValue(folder2), folder1Item);
    folder1Item->appendChild(folder2Item);

    QList<ParamModel> queryParams;
    queryParams << ParamModel("qenabled", "qvalue1") << ParamModel("qdisabled", "q", false);
    QList<ParamModel> dataParams;
    dataParams << ParamModel("denabled", "dvalue1") << ParamModel("ddisabled", "d", false);
    QList<ParamModel> headers;
    headers << ParamModel("henabled", "hvalue1") << ParamModel("hdisabled", "h", false);
    RequestModel request = RequestModel();
    request.url = "http://postman-echo.com/post?qenabled=qvalue1";
    request.method = "POST";
    request.displayName = "Lmao";
    request.queryParams = queryParams;
    request.dataParams = dataParams;
    request.headers = headers;
    request.rawData = "{\"key\": 482938473}";
    request.contentType = "application/json";
    request.documentation = "# Echo endpoint by Postman\n[Docs on website](https://docs.postman-echo.com/)";
    TreeItem *requestItem = new TreeItem(QVariant::fromValue(request), folder2Item);
    folder2Item->appendChild(requestItem);
}
