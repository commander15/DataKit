#include "etl.h"
#include "etl_p.h"

#include <DataKit/data.h>
#include <DataKit/value.h>

#include <QtCore/qvariant.h>

namespace DataKit {

ETL::ETL(QObject *parent)
    : QThread{parent}
    , d_ptr(new ETLPrivate(this))
{
    setTerminationEnabled(false);
}

ETL::~ETL()
{
    if (isRunning()) {
        requestInterruption();
        wait();
    }

    while (!d_ptr->steps.isEmpty())
        delete d_ptr->steps.takeLast();
}

QVariant ETL::parameter(const QString &name) const
{
    Q_D(const ETL);
    QMutexLocker locker(&d->mutex);

    const int index = d->parameterNames.indexOf(name);
    return (index >= 0 ? d->parameterValues.at(index) : QVariant());
}

void ETL::setParameter(const QString &name, const QVariant &value)
{
    Q_D(ETL);
    QMutexLocker locker(&d->mutex);

    const int index = d->parameterNames.indexOf(name);
    if (index >= 0)
        d_ptr->parameterValues.replace(index, value);
    else {
        d->parameterNames.append(name);
        d->parameterValues.append(value);
    }
}

void ETL::addStep(ETLStep *step)
{
    if (!isRunning())
        d_ptr->steps.append(step);
}

void ETL::addStep(const QString &name, const ProcessingCallback &processing, int type)
{
    addStep(ETLStep::create(name, processing, static_cast<ETLStep::StepType>(type)));
}

void *ETL::utilityObject(const QString &name) const
{
    Q_D(const ETL);
    QMutexLocker locker(&d->mutex);
    return d_ptr->utilityObjects.value(name);
}

void ETL::stop()
{
    requestInterruption();
}

void ETL::run()
{
    QList<ETLStep *> initialSteps;
    QList<ETLStep *> processingSteps;
    for (ETLStep *step : d_ptr->steps) {
        if (step->isInitStep())
            initialSteps.append(step);
        else
            processingSteps.append(step);
    }

    Data initialData;
    for (ETLStep *step : std::as_const(initialSteps))
        initialData = step->process(initialData, this);

    int currentCount = 0;
    const int totalCount = initialData.integer("total_count", -1);

    Data currentData;
    for (int i(0); (totalCount > 0 ? i < totalCount : i == 0 || currentData.isEmpty()); ++i) {
        currentData.put("_index", currentCount);
        for (ETLStep *step : processingSteps)
            currentData = step->process(currentData, this);
        currentData.clear();

        ++currentCount;
        emit processed(currentCount, totalCount);

        if (isInterruptionRequested())
            break;
    }

    for (ETLStep *step : std::as_const(initialSteps))
        initialData = step->process(initialData, this);
}

ETLStep *ETLStep::create(const ProcessingCallback &processing, StepType type)
{
    return new DefaultStep(QString(), processing, type);
}

ETLStep *ETLStep::create(const QString &name, const ProcessingCallback &processing, StepType type)
{
    return new DefaultStep(name, processing, type);
}

void *ETLStep::object(const QString &name, ETL *etl) const
{
    ETLData *data = etl->d_ptr.get();
    return data->utilityObjects.value(name);
}

QVariant ETLStep::parameter(const QString &name, ETL *etl) const
{
    return etl->parameter(name);
}

Data InitStep::process(const Data &data, ETL *etl)
{
    m_etl = etl;

    if (!m_objectCreated) {
        createObjects();
        m_objectCreated = true;

        Data processedData = data;
        processedData.put("total_count", dataCount());
        return processedData;
    } else {
        destroyObjects();
        m_objectCreated = false;
        return data;
    }
}

void *InitStep::object(const QString &name) const
{
    return ETLStep::object(name, m_etl);
}

void InitStep::registerObject(const QString &name, void *object)
{
    ETLData *data = m_etl->d_ptr.get();
    data->utilityObjects.insert(name, object);
}

void InitStep::unregisterObject(const QString &name)
{
    ETLData *data = m_etl->d_ptr.get();
    data->utilityObjects.remove(name);
}

QVariant InitStep::parameter(const QString &name) const
{
    return ETLStep::parameter(name, m_etl);
}

void InitStep::setParameter(const QString &name, const QVariant &value)
{
    m_etl->setParameter(name, value);
}

ETL *InitStep::etl() const
{
    return m_etl;
}

} // namespace DataKit
