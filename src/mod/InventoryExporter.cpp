#include "IncludeAll.h"
#include "DBStorage.h"
#include "UuidUtils.h"
#include <filesystem>
#include <map>
#include <fstream>

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

std::map<std::string, std::string> loadUuidNameMap() {
    std::ifstream fin("./plugins/InventoryExporter/uuid-name_map.txt");
    if (!fin.is_open()) {
        return {};
    }

    std::map<std::string, std::string> result;
    std::string line;
    while (std::getline(fin, line)) {
        // strip
        line.erase(line.find_last_not_of(" \n\r\t")+1);
        line.erase(0, line.find_first_not_of(" \n\r\t"));

        if(line.empty() || line[0] == '#')
            continue;
        size_t splitPos = line.find_first_of(" \t");

        std::string uuid = line.substr(0, splitPos);
        std::string name = line.substr(splitPos + 1);
        result[uuid] = name;
    }
    fin.close();
    return result;
}

void exportInventories() {
    // clean up
    std::error_code ec;
    std::filesystem::remove_all(std::filesystem::path("./plugins/InventoryExporter/saved"), ec);

    std::map<std::string, std::string> uuidNameMap = loadUuidNameMap();
 
    auto uuids = getAllUuids(true);
    for(auto &uuid : uuids)
    {
        std::string uuidStr = uuid.asString();
        if(uuidNameMap.contains(uuidStr))
            savePlayerData(uuid, uuidNameMap[uuidStr]);
        else
            savePlayerData(uuid);
    }
    logger.info("All players inventory exported.");
}