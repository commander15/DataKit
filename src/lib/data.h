#ifndef DATAKIT_DATA_H
#define DATAKIT_DATA_H

#include <DataKit/global.h>

#include <QtCore/qshareddata.h>
#include <QtCore/qmetatype.h>

namespace DataKit {

class DataPrivate;
class DATAKIT_EXPORT Data
{
public:
    Data();
    Data(const Data &);
    Data(Data &&);
    ~Data();

    Data &operator=(const Data &);
    Data &operator=(Data &&);

    bool has(const QString &key) const;
    QStringList keys() const;
    bool isEmpty() const;

    bool boolean(const QString &key) const;
    bool boolean(const QString &key, bool defaultValue) const;

    int integer(const QString &key) const;
    int integer(const QString &key, int defaultValue) const;

    double number(const QString &key) const;
    double number(const QString &key, double defaultValue) const;

    QString string(const QString &key) const;
    QString string(const QString &key, const QString &defaultValue) const;

    Data data(const QString &key) const;
    Data data(const QString &key, const Data &defaultValue) const;

    QVariantList array(const QString &key) const;
    QVariantList array(const QString &key, const QVariantList &defaultValue) const;

    QVariant variant(const QString &key) const;
    QVariant variant(const QString &key, const QVariant &defaultValue) const;

    static Data fromJsonObject(const QJsonObject &object);
    QJsonObject toJsonObject() const;

    void put(const QString &key, const QVariant &value);

    void remove(const QString &key);
    void clear();

    bool operator==(const Data &other) const;
    inline bool operator!=(const Data &other) const
    { return !operator==(other); }

private:
    static QVariant variantValue(const QJsonValue &value);
    static QJsonValue jsonValue(const QVariant &variant);

    QSharedDataPointer<DataPrivate> d_ptr;
};

} // namespace DataKit

Q_DECLARE_METATYPE(DataKit::Data)

#endif // DATAKIT_DATA_H
