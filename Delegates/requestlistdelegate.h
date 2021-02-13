#ifndef REQUESTLISTDELEGATE_H
#define REQUESTLISTDELEGATE_H

#include <QItemDelegate>

class RequestListDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    RequestListDelegate(QWidget *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    int height;
};

#endif // REQUESTLISTDELEGATE_H
