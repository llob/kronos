#ifndef WORKLOGDIALOGLISTVIEVITEMDELEGATE_H
#define WORKLOGDIALOGLISTVIEVITEMDELEGATE_H

#include <QAbstractItemDelegate>
#include <QSize>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QPainter>
#include <QStyleOptionViewItem>

class WorklogDialogListVievItemDelegate : public QAbstractItemDelegate
{
public:
    WorklogDialogListVievItemDelegate();
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // WORKLOGDIALOGLISTVIEVITEMDELEGATE_H
