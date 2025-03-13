#ifndef DATAKIT_ETL_P_H
#define DATAKIT_ETL_P_H

#include "etl.h"

#include <QtCore/qmutex.h>

namespace DataKit {

class ETLPrivate : public ETLData
{
public:
    ETLPrivate(ETL *q) : q_ptr(q) {}

    ETL *q_ptr;

    mutable QMutex mutex;
};

class DefaultStep : public ETLStep
{
public:
    DefaultStep(const QString &name, const ProcessingCallback &callback, StepType type)
        : m_name(name), m_callback(callback), m_type(type) {}

    QString name() const override { return m_name; }
    Data process(const Data &data, ETL *etl) override { return m_callback(data, etl); }
    StepType type() const override { return m_type; }

private:
    QString m_name;
    ProcessingCallback m_callback;
    StepType m_type;
};

} // namespace DataKit

#endif // DATAKIT_ETL_H
