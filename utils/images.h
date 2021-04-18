#ifndef IMAGES_H
#define IMAGES_H

#include <QImage>
#include <QPixmap>

/**
 * @brief The Images class contains utility classes for working with images
 */
class Images
{
public:
    Images() = delete;

    /**
     * @brief defaultAvatar Get the default avatar image
     * @return
     */
    static QImage defaultAvatar();
    /**
     * @brief defaultAvatarPixmap Get the default avatar as a pixmap
     * @return
     */
    static QPixmap defaultAvatarPixmap();
};

#endif // IMAGES_H
