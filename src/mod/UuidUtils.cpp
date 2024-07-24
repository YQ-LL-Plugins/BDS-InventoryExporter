#include "IncludeAll.h"
#include "DBStorage.h"

std::unique_ptr<CompoundTag> getUuidDBTag(mce::UUID const& uuid) {
    auto playerKey = "player_" + uuid.asString();
    if (globalDBStorage->hasKey(playerKey, DBHelpers::Category::Player)) {
        return globalDBStorage->getCompoundTag(playerKey, DBHelpers::Category::Player);
    }
    return {};
}

std::unordered_map<mce::UUID, std::string> getUuidToServerIdMap(bool includeOfflineSignedId) {
    std::unordered_map<mce::UUID, std::string> result;
    globalDBStorage->forEachKeyWithPrefix(
        "player_",
        DBHelpers::Category::Player,
        [&result, includeOfflineSignedId](std::string_view key_left, std::string_view data) {
            if (key_left.size() == 36) {
                auto  tag      = CompoundTag::fromBinaryNbt(data);
                auto& msaId    = tag->getString("MsaId");
                auto& serverId = tag->getString("ServerId");
                if (!serverId.empty()) {
                    if (!msaId.empty()) {
                        auto uuid    = mce::UUID::fromString(msaId);
                        result[uuid] = serverId;
                    } else if (includeOfflineSignedId) {
                        auto& selfSignedId = tag->getString("SelfSignedId");
                        if (!selfSignedId.empty()) {
                            auto uuid    = mce::UUID::fromString(selfSignedId);
                            result[uuid] = serverId;
                        }
                    }
                }
            }
        }
    );
    return result;
}

std::string getServerIdFromUuid(mce::UUID const& uuid) {
    auto DBTag = getUuidDBTag(uuid);
    if (!DBTag) {
        auto idMap = getUuidToServerIdMap(true);
        if (idMap.contains(uuid)) {
            return idMap[uuid];
        }
        return {};
    }
    return DBTag->getString("ServerId");
}