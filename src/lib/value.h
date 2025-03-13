#ifndef DATAKIT_VALUE_H
#define DATAKIT_VALUE_H

#include <DataKit/global.h>

#include <QtCore/qvariant.h>
#include <QSharedDataPointer>

class QJsonValue;

namespace DataKit {

class Data;

class ValueData;
class DATAKIT_EXPORT Value : public QVariant
{
public:
    enum Type {
        BooleanValue,
        IntegerValue,
        NumberValue,
        StringValue,
        DataValue,
        ArrayValue,
        NullValue = -1
    };

    Value();
    Value(Type type);
    Value(bool value);
    Value(int value);
    Value(double value);
    Value(const QString &value);
    Value(const Data &value);
    Value(const QList<Value> &values);
    Value(const Value &);
    Value(Value &&);
    ~Value();

    using QVariant::QVariant;

    Value &operator=(const Value &);
    Value &operator=(Value &&);

    // Boolean type methods
    bool isBoolean() const;

    // Integer type methods
    bool isInteger() const;

    // Number type methods
    bool isNumber() const;

    // String type methods
    bool isString() const;

    // Array type methods
    bool isArray() const;
    QList<Value> toArray() const;

    // Data type methods
    bool isData() const;
    Data toData() const;

    Type type() const;

    static Value fromVariant(const QVariant &variant);
    QVariant toVariant() const;

    static Value fromJson(const QJsonValue &value);
    QJsonValue toJson() const;

    bool operator==(const Value &other) const;
    inline bool operator!=(const Value &other) const
    { return !operator==(other); }

private:
    QSharedDataPointer<ValueData> d_ptr;
};

} // namespace DataKit

#endif // DATAKIT_VALUE_H
