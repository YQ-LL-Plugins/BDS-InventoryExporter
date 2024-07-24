#include "IncludeAll.h"
#include "DBStorage.h"
#include "ServerID.h"
#include <filesystem>

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

bool savePlayerData(mce::UUID const& uuid, const std::string &playerName = "") {
    auto playerNameId = playerName.empty() ? uuid.asString() : playerName;
    logger.info("Exporting inventory for " + playerNameId + "...");

    auto playerData = getPlayerNbt(uuid);
    ll::file_utils::writeFile(
        "./plugins/InventoryExporter/saved/" + playerNameId + "-PlayerData.nbt",
        playerData->toBinaryNbt(),
        true
    );

    auto inventoryList = playerData->getList("Inventory");
    auto inventory = CompoundTag();
    inventory.put("Inventory", inventoryList->copy());
    ll::file_utils::writeFile(
        "./plugins/InventoryExporter/saved/" + playerNameId + "-Inventory.nbt",
        inventory.toBinaryNbt(),
        true
    );
    inventory.clear();

    auto armorList = playerData->getList("Armor");
    auto armor = CompoundTag();
    armor.put("Armor", armorList->copy());
    ll::file_utils::writeFile(
        "./plugins/InventoryExporter/saved/" + playerNameId + "-Armor.nbt",
        armor.toBinaryNbt(),
        true
    );
    armor.clear();

    auto offhandList = playerData->getList("Offhand");
    auto offhand = CompoundTag();
    offhand.put("Offhand", offhandList->copy());
    ll::file_utils::writeFile(
        "./plugins/InventoryExporter/saved/" + playerNameId + "-Offhand.nbt",
        offhand.toBinaryNbt(),
        true
    );
    offhand.clear();

    auto endChestList = playerData->getList("EnderChestInventory");
    auto endChest = CompoundTag();
    endChest.put("EnderChestInventory", endChestList->copy());
    ll::file_utils::writeFile(
        "./plugins/InventoryExporter/saved/" + playerNameId + "-EndChest.nbt",
        endChest.toBinaryNbt(),
        true
    );
    endChest.clear();

    logger.info("Exported to ./plugins/InventoryExporter/saved/");
    return true;
}

void exportInventories() {
    // clean up
    std::error_code ec;
    std::filesystem::remove_all(std::filesystem::path("./plugins/InventoryExporter/saved"), ec);

    std::string uuid = "9bcaac62-8e99-372f-b416-cb6f47130479";
    savePlayerData(uuid, "yqs112358");
}