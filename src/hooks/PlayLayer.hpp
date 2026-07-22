#pragma once

#include <object_collab.hpp>
#include <Geode/modify/PlayLayer.hpp>

class $modify(ModPlayLayer, PlayLayer) {
    $override void processCreateObjectsFromSetup();
};