#include "registrationdialoglistvievitemdelegate.h"
#include "jira/jiraissue.h"
#include "utils/colors.h"
#include <QPainterPath>

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
    QPainterPath path;
    path.addRoundedRect(typeBoundingRect, 3, 3);
    painter->save();
    auto currentFont = painter->font();
    currentFont.setPixelSize(typeBoundingRect.height());
    painter->setFont(currentFont);
    QPen pen;

    if (issueType == 0) {
        painter->fillPath(path, Colors::radicalRed());
        painter->drawText(typeBoundingRect, "Recent", QTextOption(Qt::AlignHCenter|Qt::AlignVCenter));
    } else {
        painter->fillPath(path, Colors::turquise());
        painter->drawText(typeBoundingRect, "Search", QTextOption(Qt::AlignHCenter|Qt::AlignVCenter));

    }
    painter->restore();

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
