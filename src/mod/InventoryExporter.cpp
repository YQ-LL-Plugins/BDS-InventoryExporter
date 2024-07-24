#include "IncludeAll.h"
#include "DBStorage.h"
#include "ServerID.h"

std::unique_ptr<CompoundTag> getOfflineNbt(std::string const& serverId) {
    if (!globalDBStorage->hasKey(serverId, DBHelpers::Category::Player)) {
        return nullptr;
    }
    return globalDBStorage->getCompoundTag(serverId, DBHelpers::Category::Player);
}

std::unique_ptr<CompoundTag> getPlayerNbt(mce::UUID const& uuid) {
    auto serverId = getServerIdFromUuid(uuid);
    if (serverId.empty()) {
        return nullptr;
    }
    if (auto player = ll::service::getLevel()->getPlayerFromServerId(serverId)) {
        // Is online player
        auto nbt_result = std::make_unique<CompoundTag>();
        player->save(*nbt_result);
        return std::move(nbt_result);
    } else {
        // offline player
        return getOfflineNbt(serverId);
    }
}

bool savePlayerData(mce::UUID const& uuid) {
    auto nbt = getPlayerNbt(uuid);
    ll::file_utils::writeFile(
        "./plugins/InventoryExporter/save/" + uuid.asString() + ".dat",
        nbt->toBinaryNbt(),
        true
    );
    return false;
}

void exportInventories() {
    std::string uuid = "9bcaac62-8e99-372f-b416-cb6f47130479";

    logger.info("Exporting inventory for yqs112358...");
    savePlayerData(uuid);
    logger.info("Inventory exported");
}