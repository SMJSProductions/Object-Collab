#include "VanillaWarning.hpp"

using namespace object_collab;

// Text object with the string: "This level contains\nmodded objects" on { x: 200, y: 45 } which is high detail
static const std::string VANILLA_PLACEHOLDER = "1,914,2,200,3,45,103,1,31,VGhpcyBsZXZlbCBjb250YWlucwptb2RkZWQgb2JqZWN0cw==;";

// This mostly exists for Android support
bool endsWithPlaceholder(const std::string_view levelString) {
    return levelString.ends_with(VANILLA_PLACEHOLDER);
}

gd::string& cleanLevel(gd::string& levelString) {
    #define levelStringName GEODE_CONCAT(levelString, GEODE_ANDROID(Android))

    if (endsWithPlaceholder(levelString)) {
        #ifdef GEODE_IS_ANDROID
            std::string levelStringName = levelString;
        #endif

        levelStringName.erase(levelString.size() - VANILLA_PLACEHOLDER.size());
        GEODE_ANDROID(levelString = levelStringName);
    }

    return levelString;
}

void VanillaModPlayLayer::prepareCreateObjectsFromSetup(gd::string& levelString) {
    return PlayLayer::prepareCreateObjectsFromSetup(cleanLevel(levelString));
}

void VanillaModLevelEditorLayer::createObjectsFromSetup(gd::string& levelString) {
    return LevelEditorLayer::createObjectsFromSetup(cleanLevel(levelString));
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