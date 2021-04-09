#ifndef ABSTRACTISSUE_H
#define ABSTRACTISSUE_H

#include <QByteArray>
#include <QString>
#include "modelobject.h"

/**
 * @brief The AbstractIssue class encapsulates an issue
 *  with which worklogs can be associated. An issue may be
 *  any object that time may be spent on.
 */
class AbstractIssue : public ModelObject
{
protected:
    QString mKey;
    QString mSummary;
    QString mId;
public:
    AbstractIssue();
    /**
     * @brief setKey Set the unique key identifying this issue
     *  An issue may be identified either by their id or their key
     * @param key The key
     */
    void setKey(const QString key);
    /**
     * @brief key Get the unique key identifying this issue
     * @return The key
     */
    QString key() const;
    /**
     * @brief setId Set the unique id identifying this issue
     *  An issue may be identified by either its key or its id
     * @param id The id
     */
    void setId(const QString id);
    /**
     * @brief id Get the unique id identifying this issue
     * @return
     */
    QString id() const;
    /**
     * @brief setSummary Set the one-line summary of this issue
     * @param summary
     */
    void setSummary(const QString summary);
    /**
     * @brief summary Get the oneline summary of this issue
     * @return
     */
    QString summary() const;
    /**
     * @brief toJson Encode this issue as a UTF-8 JSON string
     *  stored as a byte array
     * @return
     */
    virtual QByteArray toJson() const  = 0;
    /**
     * @brief toString String representation of this issue
     * @return
     */
    virtual QString toString() const = 0;
};

typedef QSharedPointer<AbstractIssue> PAbstractIssue;

#endif // ABSTRACTISSUE_H
