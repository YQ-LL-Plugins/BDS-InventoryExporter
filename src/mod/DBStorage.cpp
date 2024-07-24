#include "IncludeAll.h"

DBStorage* globalDBStorage = nullptr;


LL_AUTO_TYPE_INSTANCE_HOOK(
    DBStorageInitEvent,
    ll::memory::HookPriority::Normal,
    DBStorage,
    "??0DBStorage@@QEAA@UDBStorageConfig@@V?$not_null@V?$NonOwnerPointer@VLevelDbEnv@@@Bedrock@@@gsl@@@Z",
    DBStorage*,
    struct DBStorageConfig&                        a1,
    Bedrock::NotNullNonOwnerPtr<class LevelDbEnv>& a2
) {
    auto res                 = origin(a1, a2);
    globalDBStorage = this;
    return res;
}