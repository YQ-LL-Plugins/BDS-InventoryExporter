#pragma once
#include <string>
#include <mc/deps/core/mce/UUID.h>

std::string getServerIdFromUuid(mce::UUID const& uuid);
std::vector<mce::UUID> getAllUuids(bool includeOfflineSignedId);