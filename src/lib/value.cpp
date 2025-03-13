#include "value.h"
#include "value_p.h"

#include <DataKit/data.h>

#include <QtCore/qjsonvalue.h>
#include <QtCore/qjsonarray.h>
#include <QtCore/qjsonobject.h>

namespace DataKit {

Value::Value()
    : d_ptr(new ValueData)
{
}

Value::Value(Type type)
    : d_ptr(new ValueData)
{
    d_ptr->type = type;
}

Value::Value(bool value)
    : d_ptr(new ValueData)
{
    d_ptr->value = value;
    d_ptr->type = BooleanValue;
}

Value::Value(int value)
    : d_ptr(new ValueData)
{
    d_ptr->value = value;
    d_ptr->type = IntegerValue;
}

Value::Value(double value)
    : d_ptr(new ValueData)
{
    d_ptr->value = value;
    d_ptr->type = NumberValue;
}

Value::Value(const QString &value)
    : d_ptr(new ValueData)
{
    d_ptr->value = value;
    d_ptr->type = StringValue;
}

Value::Value(const Data &value)
    : d_ptr(new ValueData)
{
    d_ptr->value = QVariant::fromValue(value);
    d_ptr->type = DataValue;
}

Value::Value(const QList<Value> &values)
    : d_ptr(new ValueData)
{
    d_ptr->value = QVariant::fromValue(values);
    d_ptr->type = ArrayValue;
}

Value::Value(const Value &rhs)
    : d_ptr{rhs.d_ptr}
{}

Value::Value(Value &&rhs)
    : d_ptr{std::move(rhs.d_ptr)}
{}

Value::~Value() {}

Value &Value::operator=(const Value &rhs)
{
    if (this != &rhs)
        d_ptr = rhs.d_ptr;
    return *this;
}

Value &Value::operator=(Value &&rhs)
{
    if (this != &rhs)
        d_ptr = std::move(rhs.d_ptr);
    return *this;
}

bool Value::isBoolean() const
{
    return d_ptr->type == BooleanValue;
}

bool Value::toBool() const
{
    if (isBoolean())
        return d_ptr->value.toBool();
    return false; // Default value
}

bool Value::isInteger() const
{
    return d_ptr->type == IntegerValue;
}

int Value::toInt() const
{
    if (isInteger())
        return d_ptr->value.toInt();
    return 0; // Default value
}

bool Value::isNumber() const
{
    return d_ptr->type == NumberValue;
}

double Value::toDouble() const
{
    if (isNumber())
        return d_ptr->value.toDouble();
    return 0.0; // Default value
}

bool Value::isString() const
{
    return d_ptr->type == StringValue;
}

QString Value::toString() const
{
    if (isString())
        return d_ptr->value.toString();
    return QString(); // Default value
}

bool Value::isArray() const
{
    return d_ptr->type == ArrayValue;
}

QList<Value> Value::toArray() const
{
    if (isArray())
        return d_ptr->value.value<QList<Value>>();
    return QList<Value>(); // Default empty list
}

bool Value::isData() const
{
    return d_ptr->type == DataValue;
}

Data Value::toData() const
{
    if (isData())
        return d_ptr->value.value<Data>();
    return Data(); // Default value
}

Value Value::fromVariant(const QVariant &variant)
{
    const QMetaType type = variant.metaType();

    if (type == QMetaType::fromType<Value>())
        return variant.value<Value>();

    if (type == QMetaType::fromType<QList<Value>>())
        return variant.value<QList<Value>>();

    switch (type.id()) {
    case QMetaType::Bool:
        return variant.toBool();

    case QMetaType::Int:
        return variant.toInt();

    case QMetaType::Double:
        return variant.toDouble();

    case QMetaType::QString:
        return variant.toString();

    default:
        return Value();
    }
}

QVariant Value::toVariant() const
{
    return d_ptr->value;
}

Value Value::fromJson(const QJsonValue &jsonValue)
{
    if (jsonValue.isBool()) {
        return Value(jsonValue.toBool());
    } else if (jsonValue.isDouble()) {
        return Value(jsonValue.toDouble());
    } else if (jsonValue.isString()) {
        return Value(jsonValue.toString());
    } else if (jsonValue.isArray()) {
        QList<Value> valueArray;
        for (const QJsonValue &val : jsonValue.toArray())
            valueArray.append(Value::fromJson(val));
        return Value(valueArray);
    } else if (jsonValue.isObject()) {
        return Value(Data::fromJsonObject(jsonValue.toObject()));
    }
    return Value(); // Default case (empty value)
}

QJsonValue Value::toJson() const
{
    switch (d_ptr->type) {
    case BooleanValue:
        return d_ptr->value.toBool();

    case IntegerValue:
        return d_ptr->value.toInt();

    case NumberValue:
        return d_ptr->value.toDouble();

    case StringValue:
        return d_ptr->value.toString();

    case ArrayValue:
    {
        const QList<Value> values = d_ptr->value.value<QList<Value>>();

        QJsonArray array;
        for (const Value &value : values)
            array.append(value.toJson());
        return array;
    }

    case DataValue:
        return d_ptr->value.value<Data>().toJsonObject();

    default:
        return QJsonValue();
    }
}

bool Value::operator==(const Value &other) const
{
    if (d_ptr == other.d_ptr)
        return true;

    return d_ptr->type == other.d_ptr->type
        && d_ptr->value == other.d_ptr->value;
}

Value::Type Value::type() const
{
    return d_ptr->type;
}

} // namespace DataKit
