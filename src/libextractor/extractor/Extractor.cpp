//
// Created by pierre on 6/25/26.
//

#include <filesystem>

#include "Extractor.hpp"

#include "LangFile.hpp"
#include "ThreadPool.hpp"
#include "spdlog/spdlog.h"
#include "utils.hpp"
#include "extraction_logic/Archive.hpp"
#include "extraction_logic/Extension.hpp"

#include "extraction_logic/FileDetector.hpp"
#include "models/model_store.hpp"
#include "patching_logic/X4Patchable.hpp"

namespace extractor {
    Extractor::Extractor(const ExtractorSettings &settings)
        : _settings(settings) {
    }

    void Extractor::extract() const {
        try {
            if (!std::filesystem::exists(this->_settings.OutputDirPath))
                std::filesystem::create_directories(
                    this->_settings.OutputDirPath);

            CacheFile<std::string, bool> cache(_settings.OutputDirPath / "cache.json");
            common::ThreadPool pool(_settings.nthreads);
            FileDetector detector(_settings.X4RootDirPath, _settings.OutputDirPath, _settings.XRCatToolPath);

            pool.start();
            for (auto &archive: detector.archives) {
                auto task = new common::Task([this, archive, &cache]() {
                    archive->extract(cache);
                });
                pool.enqueue(task);
            }

            for (auto &extension: detector.extensions) {
                auto task = new common::Task([this, extension, &cache]() {
                    extension->extract(cache);
                });
                pool.enqueue(task);
            }

            pool.wait();

            std::vector<X4Patchable *> patchables;
            for (auto extension: detector.extensions) {
                X4Patchable::insert_patchable(extension, patchables);

                for (auto patchable: patchables) {
                    auto task = new common::Task([this, patchable, &cache]() {
                        patchable->patch(_settings.XMLPatchPath, cache);
                    });
                    pool.enqueue(task);
                }
                pool.wait();
            }

            LangFile test(_settings.OutputDirPath / "t/0001-l044.xml");
        } catch (const std::exception &e) {
            spdlog::error("{}", e.what());
        }
    }

    void Extractor::parse() const {
        try {
            auto       wares_path        = _settings.OutputDirPath / "libraries/wares.xml";
            auto       waregroups_path   = _settings.OutputDirPath / "libraries/waregroups.xml";
            auto       modules_path      = _settings.OutputDirPath / "libraries/modules.xml";
            auto       modulegroups_path = _settings.OutputDirPath / "libraries/modulegroups.xml";
            auto       t_path            = _settings.OutputDirPath / "t/0001-l044.xml";
            ModelStore store(wares_path, waregroups_path, modules_path, modulegroups_path, t_path);

            LangFile lang(t_path);
        } catch (const std::exception &e) {
            spdlog::error("{}", e.what());
        }
    }
} // namespace extractor
