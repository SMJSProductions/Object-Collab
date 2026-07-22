#pragma once

#include <Geode/Geode.hpp>
#include <arc/prelude.hpp>

class DownloadManager {
public:
    struct ModMetadata {
        std::string id;
        std::string name;
        std::string version;
    };

    using ProgressCallback = geode::CopyableFunction<void(const ModMetadata& metadata, const float progress)>;
private:
    static DownloadManager* INSTANCE;
    static arc::Mutex<std::vector<asp::Instant>> REQUESTS;
public:
    static DownloadManager* get();

    arc::Future<geode::Result<>> downloadAll(std::vector<std::string> modIDs, ProgressCallback onProgress);
    arc::Future<geode::Result<>> download(std::string modID, ProgressCallback onProgress);
private:
    arc::Future<geode::Result<>> downloadGeodeFile(std::string url, ModMetadata metadata, ProgressCallback onProgress);
    arc::Future<geode::Result<std::string>> getViableVersion(const std::string_view modID);
    arc::Future<geode::Result<>> joinedFutureResult(std::vector<arc::Future<geode::Result<>>> futureResults);
    arc::Future<geode::Result<matjson::Value>> resultGeodeRequest(const std::string_view modID, const std::string_view path = "");
    arc::Future<geode::Result<geode::utils::web::WebResponse>> resultRequest(std::string url, std::optional<geode::Function<void(const float)>> onProgress = std::nullopt);
};