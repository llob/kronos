#include "registrationdialoglistvievitemdelegate.h"
#include "jira/jiraissue.h"
#include "utils/colors.h"

RegistrationDialogListVievItemDelegate::RegistrationDialogListVievItemDelegate()
{

}

void RegistrationDialogListVievItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariantMap v = index.model()->data(index, Qt::UserRole).toMap();
    QVariantMap issueMap = v.value("issue").toMap();
    int issueType = v.value("type").toInt();
    Q_UNUSED(issueType);
    JiraIssue j(issueMap);
    QRect boundingRect = option.rect;

    QRect typeBoundingRect(boundingRect);
    typeBoundingRect.setRight(50);
    typeBoundingRect.adjust(5, 5, -5, -5);
    if (issueType == 0) {
        painter->fillRect(typeBoundingRect, Colors::color2());
        painter->drawText(typeBoundingRect, "Recent");
    } else {
        painter->fillRect(typeBoundingRect, Colors::color4());
        painter->drawText(typeBoundingRect, "Search");

    }

    QRect textBoundingRect(boundingRect);
    textBoundingRect.setLeft(50);
    painter->drawText(textBoundingRect, QString("%1 %2").arg(j.key(), j.summary()));
}

QSize RegistrationDialogListVievItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    Q_UNUSED(option);
    return QSize(300, 20);
}
