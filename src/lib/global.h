#ifndef DATAKIT_GLOBAL_H
#define DATAKIT_GLOBAL_H

#include <DataKit/config.h>

#ifdef DATAKIT_SHARED
#   ifdef DATAKIT_BUILD
#       define DATAKIT_EXPORT Q_DECL_EXPORT
#   else
#       define DATAKIT_EXPORT Q_DECL_IMPORT
#   endif
#else
#   define DATAKIT_EXPORT
#endif

#define DATAKIT_Q(ClassName) ClassName *q = static_cast<ClassName *>(q_ptr)
#define DATAKIT_D(ClassName) \
    ClassName##Private *d = static_cast<ClassName##Private *>(qGetPtrHelper(d_ptr))

#endif // DATAKIT_GLOBAL_H
