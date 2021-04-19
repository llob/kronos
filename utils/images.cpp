#include "images.h"
#include <QBuffer>

QImage Images::defaultAvatar()
{
    return QImage(":/resources/default-avatar.jpg", "JPG");
}

QPixmap Images::defaultAvatarPixmap()
{
    QImage image = defaultAvatar();
    QByteArray bArray;
    QBuffer buffer(&bArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    QPixmap result;
    result.loadFromData(bArray, "PNG");
    return result;
}
