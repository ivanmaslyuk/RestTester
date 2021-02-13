#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QFontMetrics>
#include "requestlistdelegate.h"
#include "TreeModel/treeitem.h"
#include "Models/requestmodel.h"

RequestListDelegate::RequestListDelegate(QWidget *parent) : QItemDelegate(parent)
{
    this->height = 38;
}

void RequestListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->setRenderHint(QPainter::HighQualityAntialiasing);

    QBrush hoverBrush(QColor("#444"));
    QBrush selectedBrush(QColor("#333"));
    QPen textPen(QColor("#fff"));
    QPen iconPen(QColor("#777"));
    QPen linePen(QColor("#444"));
    QFont font("Roboto");
    font.setStyleStrategy(QFont::StyleStrategy::PreferQuality);
    font.setPixelSize(12);

    TreeItem *treeItem = static_cast<TreeItem *>(index.internalPointer());
    RequestModel request = treeItem->data().value<RequestModel>();

    QRect rect = QRect(option.rect.topLeft() - QPoint(20, 0), option.rect.bottomRight());

    // Set background
    if (option.state & QStyle::State_MouseOver) {
        painter->fillRect(rect, hoverBrush);
    } else if (option.state & QStyle::State_Selected) {
        painter->fillRect(rect, selectedBrush);
    }

    // Expand/Collapse icon
    if (option.state & QStyle::State_Children) {
        painter->setPen(iconPen);
        if (option.state & QStyle::State_Open) {
            QPoint iconStart = rect.topLeft() + QPoint(16, (this->height - 5) / 2);
            QPainterPath iconPath(iconStart);
            iconPath.lineTo(iconStart + QPoint(5, 5));
            iconPath.moveTo(iconStart + QPoint(5, 5));
            iconPath.lineTo(iconStart + QPoint(10, 0));
            painter->drawPath(iconPath);
        } else {
            QPoint iconStart = rect.topLeft() + QPoint(16 + 2.5, (this->height - 10) / 2);
            QPainterPath iconPath(iconStart);
            iconPath.lineTo(iconStart + QPoint(5, 5));
            iconPath.moveTo(iconStart + QPoint(5, 5));
            iconPath.lineTo(iconStart + QPoint(0, 10));
            painter->drawPath(iconPath);
        }
    }

    // Request method
    QFontMetrics fontMetrics(font);
    int methodTextWidth = fontMetrics.horizontalAdvance(request.method);
    if (!(option.state & QStyle::State_Children)) {
        QFont boldFont(font);
        boldFont.setBold(true);
        painter->setFont(boldFont);
        painter->setPen(textPen);
        QPoint methodTopLeft = rect.topLeft() + QPoint(16, 0);
        QPoint methodBottomRight = rect.bottomLeft() + QPoint(16 + methodTextWidth, 0);
        painter->drawText(QRect(methodTopLeft, methodBottomRight), Qt::AlignVCenter, request.method);
    }

    // Request/Folder name
    painter->setPen(textPen);
    painter->setFont(font);
    int textIndent = option.state & QStyle::State_Children ? 16 + 10 + 12 : 16 + methodTextWidth + 12;
    QPoint textTopLeft = rect.topLeft() + QPoint(textIndent, 0);
    QPoint textBottomRight = rect.bottomRight() - QPoint(16, 0);
    painter->drawText(QRect(textTopLeft, textBottomRight), Qt::AlignVCenter, request.displayName);

    // Line at the bottom
    painter->setPen(linePen);
    painter->drawLine(rect.bottomLeft(), rect.bottomRight());
}

QSize RequestListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(-1, this->height);
}
