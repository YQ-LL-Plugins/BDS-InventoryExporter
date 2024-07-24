#include "mod/Entry.h"

#include <memory>
#include "IncludeAll.h"
#include "InventoryExporter.h"

ll::Logger logger("InventoryExporter");

namespace my_plugin {

static std::unique_ptr<MyMod> instance;

MyMod& MyMod::getInstance() { return *instance; }

bool MyMod::load() {
    auto& eventBus = ll::event::EventBus::getInstance();

    logger.info("Player Inventory Exporter loaded.");

    // auto listener = eventBus.emplaceListener<ll::event::ServerStartedEvent>(
    //     [](ll::event::ServerStartedEvent& event){
    //         exportInventories();
    //     }
    // );

    return true;
}

bool MyMod::enable() {
    exportInventories();
    return true;
}

bool MyMod::disable() {
    return true;
}

} // namespace my_plugin

LL_REGISTER_MOD(my_plugin::MyMod, my_plugin::instance);
