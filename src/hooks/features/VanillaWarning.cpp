#include "VanillaWarning.hpp"

using namespace object_collab;

// Text object with the string: "This level contains\nmodded objects" on { x: 200, y: 45 } which is high detail
static const std::string VANILLA_PLACEHOLDER = "1,914,2,200,3,45,103,1,31,VGhpcyBsZXZlbCBjb250YWlucwptb2RkZWQgb2JqZWN0cw==;";

bool endsWithPlaceholder(const gd::string& str) {
    #ifdef GEODE_IS_ANDROID
        return std::string_view(str).ends_with(VANILLA_PLACEHOLDER);
    #else
        return str.ends_with(VANILLA_PLACEHOLDER);
    #endif
}

void cleanLevel(gd::string& levelString) {
    if (endsWithPlaceholder(levelString)) {
        #ifdef GEODE_IS_ANDROID
            levelString = levelString.substr(0, levelString.size() - VANILLA_PLACEHOLDER.size());
        #else
            levelString.erase(levelString.size() - VANILLA_PLACEHOLDER.size());
        #endif
    }
}

void VanillaModPlayLayer::prepareCreateObjectsFromSetup(gd::string& levelString) {
    cleanLevel(levelString);

    return PlayLayer::prepareCreateObjectsFromSetup(levelString);
}

void VanillaModLevelEditorLayer::createObjectsFromSetup(gd::string& levelString) {
    cleanLevel(levelString);

    return LevelEditorLayer::createObjectsFromSetup(levelString);
}

gd::string VanillaModLevelEditorLayer::getLevelString() {
    if (CustomLevelData::raw(this).size()) {
        std::string levelString = LevelEditorLayer::getLevelString();

        if (endsWithPlaceholder(levelString)) {
            return levelString;
        } else {
            #ifdef GEODE_IS_ANDROID
                return std::move(levelString) + VANILLA_PLACEHOLDER;
            #else
                return levelString.append(VANILLA_PLACEHOLDER);
            #endif
        }
    } else {
        return LevelEditorLayer::getLevelString();
    }
}