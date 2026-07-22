#include "DownloadManager.hpp"

using namespace geode::prelude;
using namespace web;
using namespace arc;
using namespace matjson;

DownloadManager* DownloadManager::INSTANCE = nullptr;

Mutex<std::vector<asp::Instant>> DownloadManager::REQUESTS;

DownloadManager* DownloadManager::get() {
    if (!DownloadManager::INSTANCE) DownloadManager::INSTANCE = new DownloadManager();

    return DownloadManager::INSTANCE;
}

Future<Result<>> DownloadManager::downloadAll(std::vector<std::string> modIDs, ProgressCallback onProgress) {
    std::vector<Future<Result<>>> downloads;

    for (std::string& modID : modIDs) {
        downloads.emplace_back(this->download(std::move(modID), std::move(onProgress)));
    }

    return this->joinedFutureResult(std::move(downloads));
}

Future<Result<>> DownloadManager::download(std::string modID, ProgressCallback onProgress) {
    GEODE_CO_UNWRAP_INTO(std::string version, co_await this->getViableVersion(modID));
    GEODE_CO_UNWRAP_INTO(Value versionJson, co_await this->resultGeodeRequest(modID, fmt::format("/versions/{}", version)));
    Loader* loader = Loader::get();
    std::vector<Future<Result<>>> downloads;

    for (Value& dependency : GEODE_CO_UNWRAP(versionJson["payload"]["dependencies"].asArray())) {
        if (!GEODE_CO_UNWRAP(dependency["required"].asBool())) continue;

        GEODE_CO_UNWRAP_INTO(std::string depeendencyID, dependency["mod_id"].asString());

        if (Mod* mod = loader->getInstalledMod(depeendencyID)) {
            GEODE_CO_UNWRAP(mod->enable());
        } else {
            downloads.emplace_back(this->download(std::move(depeendencyID), onProgress));
        }
    }

    downloads.emplace_back(this->downloadGeodeFile(GEODE_CO_UNWRAP(versionJson["payload"]["download_link"].asString()), {
        .id = std::move(modID),
        .name = GEODE_CO_UNWRAP(versionJson["payload"]["name"].asString()),
        .version = std::move(version),
    }, std::move(onProgress)));

    co_return co_await this->joinedFutureResult(std::move(downloads));
}

arc::Future<geode::Result<>> DownloadManager::downloadGeodeFile(std::string url, ModMetadata metadata, ProgressCallback onProgress) {
    std::string modID = metadata.id;

    co_return GEODE_CO_UNWRAP(
        co_await this->resultRequest(std::move(url), [
            metadata = std::move(metadata),
            onProgress = std::move(onProgress)
        ](const float value){ onProgress(metadata, value); });
    ).into(dirs::getModsDir() / fmt::format("{}.geode", modID));
}

Future<Result<std::string>> DownloadManager::getViableVersion(const std::string_view modID) {
    GEODE_CO_UNWRAP_INTO(Value metadataJson, co_await this->resultGeodeRequest(modID));

    for (Value& version : GEODE_CO_UNWRAP(metadataJson["payload"]["versions"].asArray())) {
        Value& gdVersionValue = version["gd"][GEODE_PLATFORM_SHORT_IDENTIFIER];

        if (gdVersionValue.isNull()) continue;

        GEODE_CO_UNWRAP_INTO(const std::string gdVersion, gdVersionValue.asString());

        if (gdVersion == "*" || gdVersion == GEODE_GD_VERSION_STRING) {
            co_return Ok(GEODE_CO_UNWRAP(version["version"].asString()));
        }
    }

    co_return Err("No version found for the current GD version");
}

Future<Result<>> DownloadManager::joinedFutureResult(std::vector<Future<Result<>>> futureResults) {
    for (Result<>& result : co_await arc::joinAll(std::move(futureResults))) {
        GEODE_CO_UNWRAP(result);
    }

    co_return Ok();
}

Future<Result<Value>> DownloadManager::resultGeodeRequest(const std::string_view modID, const std::string_view path) {
    co_return GEODE_CO_UNWRAP(co_await this->resultRequest(fmt::format("https://api.geode-sdk.org/v1/mods/{}{}", modID, path))).json();
}

Future<Result<WebResponse>> DownloadManager::resultRequest(std::string url, std::optional<geode::Function<void(const float)>> onProgress) {
    static const size_t RATE_LIMIT_AMOUNT = 25;
    static const size_t RATE_LIMIT_TIME = 1;
    bool shouldWait = false;

    {
        auto lockedRequests = co_await DownloadManager::REQUESTS.lock();

        std::erase_if(*lockedRequests, [](const asp::Instant& requestTime) {
            return requestTime.elapsed().seconds() >= RATE_LIMIT_TIME;
        });

        shouldWait = lockedRequests->size() >= RATE_LIMIT_AMOUNT;

        if (!shouldWait) lockedRequests->emplace_back(asp::Instant::now());
    }

    if (shouldWait) {
        co_await arc::sleepFor(asp::time::Duration::fromSecs(RATE_LIMIT_TIME));

        co_return co_await this->resultRequest(std::move(url), std::move(onProgress));
    }

    WebRequest request;

    if (onProgress) request.onProgress([
        onProgress = std::move(onProgress).value()
    ](const WebProgress& progress) mutable { onProgress(progress.downloadProgress().value_or(0) / 100); });

    if (WebResponse response = co_await request.get(std::move(url)); response.ok()) {
        co_return Ok(std::move(response));
    } else {
        co_return Err(response.string().unwrapOr("Unknown error"));
    }
}