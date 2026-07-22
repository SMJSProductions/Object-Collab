#pragma once

#include <Geode/modify/EditButtonBar.hpp>
#include "EditorUI.hpp"

class $modify(ModEditButtonBar, EditButtonBar) {
    $override static EditButtonBar* create(cocos2d::CCArray* objects, cocos2d::CCPoint position, const int tab, const bool hasCreateItems, const int columns, const int rows);
};