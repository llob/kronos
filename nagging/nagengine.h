#ifndef NAGENGINE_H
#define NAGENGINE_H

#include <QObject>
#include <QTimer>
#include "settings/settings.h"
#include "nag.h"
#include "nagstate.h"

/**
 * @brief The NagEngine class is responsible for generating
 *  nag messages at appropriate times and tracking which
 *  nags have already been generated.
 */
class NagEngine : public QObject
{
    Q_OBJECT
private:
    QList<PNag> mNags;
    QTimer mNagTimer;
    NagState mNagState;
    Settings mSettings;
    void setupConnections();
    void setupNags();
private slots:
    void nagTimerTimeout();
public:
    explicit NagEngine();

signals:
    /**
     * @brief nag Emitted when user should be nagged
     * @param message The nagging message
     */
    void nag(QString message);

};

#endif // NAGENGINE_H
