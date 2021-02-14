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

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    TreeItem *item = getItem(index);
    bool result = item->setData(value);

    if (result)
        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

    return result;
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return false;

    beginInsertRows(parent, position, position + rows - 1);
    const bool success = parentItem->insertChildren(position, rows);
    endInsertRows();

    return success;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return false;

    beginRemoveRows(parent, position, position + rows - 1);
    const bool success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
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

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    const TreeItem *parentItem = getItem(parent);

    return parentItem ? parentItem->childCount() : 0;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

void TreeModel::setupModelData(TreeItem *pppp)
{
    RequestModel topLevelFolder;
    topLevelFolder.displayName = "Top level folder";

    this->insertRows(0, 10);
    QModelIndex folder1Index = index(0, 0);
    this->setData(folder1Index, QVariant::fromValue(topLevelFolder));


    RequestModel folder2;
    folder2.displayName = "Folder";

    this->insertRows(0, 3, folder1Index);
    QModelIndex folder2Index = index(0, 0, folder1Index);
    this->setData(folder2Index, QVariant::fromValue(folder2));


    QList<ParamModel> queryParams;
    queryParams << ParamModel("qenabled", "qvalue1") << ParamModel("qdisabled", "q", false);
    QList<ParamModel> dataParams;
    dataParams << ParamModel("denabled", "dvalue1") << ParamModel("ddisabled", "d", false);
    QList<ParamModel> headers;
    headers << ParamModel("henabled", "hvalue1") << ParamModel("hdisabled", "h", false);
    RequestModel request;
    request.url = "http://postman-echo.com/post?qenabled=qvalue1";
    request.method = "POST";
    request.displayName = "Postman Echo";
    request.queryParams = queryParams;
    request.dataParams = dataParams;
    request.headers = headers;
    request.rawData = "{\"key\": 482938473}";
    request.contentType = "application/json";
    request.documentation = "# Echo endpoint by Postman\n[Docs on website](https://docs.postman-echo.com/)";

    this->insertRows(0, 2, folder2Index);
    QModelIndex requestIndex = index(0, 0, folder2Index);
    this->setData(requestIndex, QVariant::fromValue(request));


    RequestModel request2(request);
    request2.displayName = "VK";
    request2.url = "http://vk.com/";
    request2.method = "GET";
    request2.documentation = "[ВКонтакте](https://vk.com)";

    QModelIndex request2Index = index(1, 0, folder2Index);
    this->setData(request2Index, QVariant::fromValue(request2));
}

TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}
