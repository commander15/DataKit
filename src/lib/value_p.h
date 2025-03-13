#ifndef DATAKIT_VALUE_P_H
#define DATAKIT_VALUE_P_H

#include "value.h"

#include <QtCore/qvariant.h>

namespace DataKit {

class ValueData : public QSharedData
{
public:
    QVariant value;
    Value::Type type;
};

} // namespace DataKit

#endif // DATAKIT_VALUE_H
