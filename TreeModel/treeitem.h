#ifndef TREEITEM_H
#define TREEITEM_H

#include <QVariant>

class TreeItem
{
public:
    explicit TreeItem(const QVariant &data, TreeItem *parentItem = nullptr);
    ~TreeItem();

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data() const;
    bool insertChildren(int position, int count);
    TreeItem *parentItem();
    bool removeChildren(int position, int count);
    int childNumber() const;
    bool setData(const QVariant &value);


private:
    QVector<TreeItem*> m_childItems;
    QVariant m_itemData;
    TreeItem *m_parentItem;
};

#endif // TREEITEM_H
