#ifndef DATAKIT_DATA_P_H
#define DATAKIT_DATA_P_H

#include <QtCore/qshareddata.h>
#include <QtCore/qstringlist.h>

namespace DataKit {

class DataPrivate : public QSharedData
{
public:
    QStringList keys;
    QVariantList values;
};

} // namespace DataKit

#endif // DATAKIT_DATA_P_H
