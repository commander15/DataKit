#include "data.h"
#include "data_p.h"

#include <utility>

#include <QtCore/qjsonobject.h>
#include <QtCore/qjsonarray.h>
#include <QtCore/qjsonvalue.h>

namespace DataKit {

Data::Data()
    : d_ptr(new DataPrivate)
{}

Data::Data(const Data &rhs)
    : d_ptr{rhs.d_ptr}
{}

Data::Data(Data &&rhs)
    : d_ptr{std::move(rhs.d_ptr)}
{}

Data::~Data() {}

Data &Data::operator=(const Data &rhs)
{
    if (this != &rhs)
        d_ptr = rhs.d_ptr;
    return *this;
}

Data &Data::operator=(Data &&rhs)
{
    if (this != &rhs)
        d_ptr = std::move(rhs.d_ptr);
    return *this;
}

bool Data::has(const QString &key) const
{
    return d_ptr->keys.contains(key);
}

QStringList Data::keys() const
{
    return d_ptr->keys;
}

bool Data::isEmpty() const
{
    return d_ptr->keys.isEmpty();
}

int Data::integer(const QString &key) const
{
    return integer(key, 0);  // Default to 0 if key is not found
}

int Data::integer(const QString &key, int defaultValue) const
{
    const int index = d_ptr->keys.indexOf(key);
    return (index >= 0 ? d_ptr->values.at(index).toInt() : defaultValue);
}

double Data::number(const QString &key) const
{
    return number(key, 0.0);  // Default to 0.0 if key is not found
}

double Data::number(const QString &key, double defaultValue) const
{
    const int index = d_ptr->keys.indexOf(key);
    return (index >= 0 ? d_ptr->values.at(index).toDouble() : defaultValue);
}

QString Data::string(const QString &key) const
{
    return string(key, "");  // Default to an empty string if key is not found
}

QString Data::string(const QString &key, const QString &defaultValue) const
{
    const int index = d_ptr->keys.indexOf(key);
    return (index >= 0 ? d_ptr->values.at(index).toString() : defaultValue);
}

Data Data::data(const QString &key) const
{
    return data(key, Data());  // Default to an empty Data object if key is not found
}

Data Data::data(const QString &key, const Data &defaultValue) const
{
    const int index = d_ptr->keys.indexOf(key);
    return (index >= 0 ? d_ptr->values.at(index).value<Data>() : defaultValue);
}

QVariantList Data::array(const QString &key) const
{
    return array(key, QVariantList());
}

QVariantList Data::array(const QString &key, const QVariantList &defaultValue) const
{
    const int index = d_ptr->keys.indexOf(key);
    return (index >= 0 ? d_ptr->values.at(index).toList() : defaultValue);
}

QVariant Data::variant(const QString &key) const
{
    return variant(key, QVariant());  // Default to QVariant() if key is not found
}

QVariant Data::variant(const QString &key, const QVariant &defaultValue) const
{
    const int index = d_ptr->keys.indexOf(key);
    return (index >= 0 ? d_ptr->values.at(index) : defaultValue);
}

Data Data::fromJsonObject(const QJsonObject &object)
{
    Data data;

    for (auto it = object.begin(); it != object.end(); ++it) {
        const QString &key = it.key();
        const QJsonValue &jsonValue = it.value();
        data.put(key, variantValue(jsonValue));
    }

    return data;
}

QJsonObject Data::toJsonObject() const
{
    QJsonObject jsonObj;

    for (int i = 0; i < d_ptr->keys.size(); ++i) {
        const QString &key = d_ptr->keys.at(i);
        const QVariant &value = d_ptr->values.at(i);
        jsonObj.insert(key, jsonValue(value));
    }

    return jsonObj;
}

void Data::put(const QString &key, const QVariant &value)
{
    d_ptr->keys.append(key);
    d_ptr->values.append(value);
}

void Data::remove(const QString &key)
{
    const int index = d_ptr->keys.indexOf(key);

    if (index >= 0) {
        d_ptr->keys.removeAt(index);
        d_ptr->values.removeAt(index);
    }
}

void Data::clear()
{
    d_ptr->keys.clear();
    d_ptr->values.clear();
}

bool Data::operator==(const Data &other) const
{
    if (d_ptr == other.d_ptr)
        return true;

    return d_ptr->keys == other.d_ptr->keys
           && d_ptr->values == other.d_ptr->values;
}

QVariant Data::variantValue(const QJsonValue &value)
{
    if (value.isObject()) {
        return QVariant::fromValue(fromJsonObject(value.toObject()));
    }

    if (value.isArray()) {
        QVariantList valueArray;
        for (const QJsonValue &val : value.toArray()) {
            valueArray.append(variantValue(val));
        }
        return valueArray;
    }

    return value.toVariant();
}

QJsonValue Data::jsonValue(const QVariant &variant)
{
    if (variant.canConvert<Data>()) {
        return variant.value<Data>().toJsonObject();
    } else if (variant.canConvert<QVariantList>()) {
        const QVariantList values = variant.toList();

        QJsonArray jsonArray;
        for (const QVariant &value : values)
            jsonArray.append(jsonValue(value));
        return jsonArray;
    } else {
        return QJsonValue::fromVariant(variant);
    }
}

} // namespace DataKit
