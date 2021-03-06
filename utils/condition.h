#ifndef CONDITION_H
#define CONDITION_H

#include <QDebug>
#include <QImage>
#include <QSharedPointer>

/// Possible condition statuses: good or bad.
enum ConditionStatus : bool {
    GOOD = true, /// Condition is good
    BAD = false /// Condition is bad
};

///
/// \brief The Bad class represents a bad condition.
///     It holds an error message.
///
class Bad {
public:
    Bad(QString m)
        : message(m)
    {
    }
    Bad(const char* m)
        : message(m)
    {
    }
    QString message;
};

///
/// \brief The Good class represents a good condition.
///     It holds a value of type T.
///
template <typename T = bool>
class Good {
public:
    Good() {}
    Good(T t)
        : value(t)
    {
    }
    T value;
};

using GoodEmptyCondition = Good<>;

///
/// \brief Condition is used to signal the result status of an operation.
///        Call getCondition() (or good() or bad()) to determine if the operation succeeded.
///        If the condition is bad, call getMessage() to get a description of what went wrong.
///        Call getData() to get any result values generated by the operation (both if good or bad).
///        Please note that data included in the condition is wrapped in a QSharedPointer which
///        manages the data life cycle. You are not allowed to delete the included payload data,
///        as it will be freed by the QSharedPointer once it is no longer in use.
template <class T = bool>
class Condition {

protected:
    T mData;
    ConditionStatus mStatus;
    QString mMessage;
    int mErrorCode = ConditionStatus::BAD;

public:
    ///
    /// \brief Condition Bad constructor
    /// \param bad
    ///
    Condition(Bad bad)
        : Condition(ConditionStatus::BAD, bad.message)
    {
    }

    ///
    /// \brief Condition Good constructor
    /// \param good
    ///
    Condition(Good<T> good)
        : Condition(ConditionStatus::GOOD, "", good.value)
    {
    }

    ///
    /// \brief Condition copy constructor
    /// \param other The object to copy.
    Condition(const Condition<T>& other)
    {
        mData = other.mData;
        mStatus = other.mStatus;
        mMessage = other.mMessage;
    }

    Condition& operator=(const Condition<T>& other)
    {
        mData = other.mData;
        mStatus = other.mStatus;
        mMessage = other.mMessage;

        return *this;
    }

    explicit operator bool() const
    {
        return good();
    }

    ///
    /// \brief Create a new condition, setting the condition and message fields
    Condition(ConditionStatus status, QString message)
    {
        mStatus = status;
        mMessage = message;
    }

    Condition(ConditionStatus status, QString message, T data)
    {
        mStatus = status;
        mMessage = message;
        mData = data;
    }

    ///
    /// \brief Create a new condition
    ///
    Condition() = default;

    T data() const
    {
        return mData;
    }

    ///
    /// \brief Get any message text included in the condition
    QString message() const
    {
        return mMessage;
    }
    ///
    /// \brief status Get the ConditionStatus of this condition.
    ///
    ConditionStatus status() const
    {
        return mStatus;
    }

    ///
    /// \brief Shorthand for checking if condition is good
    bool good() const
    {
        return mStatus == ConditionStatus::GOOD;
    }

    ///
    /// \brief Shorthand for checking if condition is bad
    bool bad() const
    {
        return mStatus == ConditionStatus::BAD;
    }
};

///
/// \brief EmptyCondition
///        Shorthand name for a condition with no payload data.
///
typedef Condition<bool> EmptyCondition;

#endif // CONDITION_H
