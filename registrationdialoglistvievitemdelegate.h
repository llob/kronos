#ifndef REGISTRATIONDIALOGLISTVIEVITEMDELEGATE_H
#define REGISTRATIONDIALOGLISTVIEVITEMDELEGATE_H

#include <QAbstractItemDelegate>
#include <QSize>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QPainter>
#include <QStyleOptionViewItem>

class RegistrationDialogListVievItemDelegate : public QAbstractItemDelegate
{
public:
    RegistrationDialogListVievItemDelegate();
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // REGISTRATIONDIALOGLISTVIEVITEMDELEGATE_H
