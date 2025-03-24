#ifndef DATAKIT_DEBUG_P_H
#define DATAKIT_DEBUG_P_H

#include <QtCore/qloggingcategory.h>

#define datakitDebug()    qCDebug(datakit).noquote().nospace()
#define datakitInfo()     qCInfo(datakit).noquote().nospace()
#define datakitWarning()  qCWarning(datakit).noquote().nospace()
#define datakitCritical() qCCritical(datakit).noquote().nospace()

Q_DECLARE_LOGGING_CATEGORY(datakit)

#endif // DATAKIT_DEBUG_P_H
