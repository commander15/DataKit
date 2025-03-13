#ifndef DATAKIT_ETL_H
#define DATAKIT_ETL_H

#include <DataKit/global.h>
#include <DataKit/data.h>

#include <QtCore/qthread.h>
#include <QtCore/qhash.h>

namespace DataKit {

class ETLStep;

class ETLData
{
public:
    QStringList parameterNames;
    QVariantList parameterValues;
    QList<ETLStep *> steps;
    QHash<QString, void *> utilityObjects;
};

class ETLPrivate;
class DATAKIT_EXPORT ETL : public QThread
{
    Q_OBJECT

public:
    typedef std::function<Data(const Data &, ETL *)> ProcessingCallback;

    explicit ETL(QObject *parent = nullptr);
    virtual ~ETL();

    QVariant parameter(const QString &name) const;
    void setParameter(const QString &name, const QVariant &value);

    void addStep(ETLStep *step);
    void addStep(const QString &name, const ProcessingCallback &processing, int type);

    void *utilityObject(const QString &name) const;

public slots:
    void stop();

signals:
    void processed(int count, int total = -1);

protected:
    void run() override;

private:
    Q_DECLARE_PRIVATE(ETL)

    QScopedPointer<ETLData> d_ptr;

    friend class ETLStep;
    friend class InitStep;
};

class DATAKIT_EXPORT ETLStep
{
public:
    enum StepType {
        InitialStep,
        ExtractStep,
        TransformStep,
        LoadStep
    };

    typedef ETL::ProcessingCallback ProcessingCallback;

    virtual ~ETLStep() = default;

    virtual QString name() const { return QString(); }

    virtual Data process(const Data &data, ETL *etl) = 0;

    inline bool isInitStep() const { return type() == InitialStep; }
    virtual StepType type() const = 0;

    static ETLStep *create(const ProcessingCallback &processing, StepType type);
    static ETLStep *create(const QString &name, const ProcessingCallback &processing, StepType type);

protected:
    void *object(const QString &name, ETL *etl) const;
    QVariant parameter(const QString &name, ETL *etl) const;
};

class DATAKIT_EXPORT InitStep : public ETLStep
{
public:
    virtual ~InitStep() = default;

    virtual void createObjects() = 0;
    virtual void destroyObjects() = 0;
    virtual int dataCount() = 0;

    Data process(const Data &data, ETL *etl) override;

    StepType type() const override final
    { return InitialStep; }

protected:
    void *object(const QString &name) const;
    void registerObject(const QString &name, void *object);
    void unregisterObject(const QString &name);

    QVariant parameter(const QString &name) const;
    void setParameter(const QString &name, const QVariant &value);

    ETL *etl() const;

private:
    ETL *m_etl = nullptr;
    bool m_objectCreated = false;
};

} // namespace DataKit

#endif // DATAKIT_ETL_H
