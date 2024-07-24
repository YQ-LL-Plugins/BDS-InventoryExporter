#pragma once

#include <fmt/format.h>
#include <functional>
#include <ll/api/Config.h>
#include <ll/api/command/Command.h>
#include <ll/api/command/CommandHandle.h>
#include <ll/api/command/CommandRegistrar.h>
#include <ll/api/data/KeyValueDB.h>
#include <ll/api/event/EventBus.h>
#include <ll/api/event/ListenerBase.h>
#include <ll/api/event/player/PlayerJoinEvent.h>
#include <ll/api/event/player/PlayerUseItemEvent.h>
#include <ll/api/event/server/ServerStartedEvent.h>
#include <ll/api/form/ModalForm.h>
#include <ll/api/io/FileUtils.h>
#include <ll/api/mod/NativeMod.h>
#include <ll/api/mod/ModManagerRegistry.h>
#include <ll/api/mod/RegisterHelper.h>
#include <ll/api/service/Bedrock.h>
#include <ll/api/memory/Hook.h>
#include <ll/api/Logger.h>

#include <mc/entity/utilities/ActorType.h>
#include <mc/server/commands/CommandOrigin.h>
#include <mc/server/commands/CommandOutput.h>
#include <mc/server/commands/CommandPermissionLevel.h>
#include <mc/world/actor/player/Player.h>
#include <mc/world/item/registry/ItemStack.h>
#include <mc/world/level/Level.h>
#include <mc/world/level/storage/DBStorage.h>

#include <memory>
#include <stdexcept>

extern ll::Logger logger;