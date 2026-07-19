//
// Created by pierre on 6/25/26.
//

#include <filesystem>

#include "Extractor.hpp"

#include "ThreadPool.hpp"
#include "spdlog/spdlog.h"
#include "utils.hpp"
#include "databuilder/AggregateStore.hpp"
#include "databuilder/Databuilder.hpp"
#include "extraction_logic/Archive.hpp"
#include "extraction_logic/Extension.hpp"

#include "extraction_logic/FileDetector.hpp"
#include "models/model_store.hpp"
#include "patching_logic/X4Patchable.hpp"

#include <rfl/xml.hpp>

namespace extractor {
    Extractor::Extractor(const ExtractorSettings &settings) :
    _settings(settings) {
    }

    void Extractor::extract() {
        try {
            if (!std::filesystem::exists(this->_settings.ExtractionDirPath))
                std::filesystem::create_directories(
                    this->_settings.ExtractionDirPath);

            CacheFile<std::string, bool> cache(_settings.ExtractionDirPath / "cache.json");
            common::ThreadPool pool(_settings.nthreads);
            FileDetector detector(_settings.X4RootDirPath, _settings.ExtractionDirPath, _settings.XRCatToolPath);

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
            LangFile lang;
            auto base_t = rfl::xml::load<models::T>((_settings.ExtractionDirPath / "t/0001-l044.xml").string());
            lang.add_t(std::move(base_t.value()));
            for (auto extension: detector.extensions) {
                X4Patchable::insert_patchable(extension, patchables);

                for (auto patchable: patchables) {
                    auto task = new common::Task([this, &lang, patchable, &cache]() {
                        patchable->patch(_settings.XMLPatchPath, lang, cache);
                    });
                    pool.enqueue(task);
                }
                pool.wait();
                patchables.clear();
            }
            lang_file = std::move(lang);
        } catch (const std::exception &e) {
            spdlog::error("{}", e.what());
        }
    }

    void Extractor::parse() {
        try {
            models::ModelStore          models(std::move(this->lang_file), _settings.ExtractionDirPath);
            databuilder::AggregateStore datas(std::move(models));
            databuilder::Databuilder    builder(std::move(datas));
            common::ThreadPool          pool(_settings.nthreads);

            pool.start();

            for (auto &i: builder.wares) {
                auto fn = [this, i]() {
                    auto path = _settings.OutputDirPath / "wares";
                    std::filesystem::create_directories(path);
                    path /= fmt::format("{}.json", i.id);
                    rfl::json::save(path.string(), i);
                };
                pool.enqueue(new common::Task(fn));
            }
            spdlog::info("Saved {} wares", builder.wares.size());
            save_datas("productions", builder.modules.productions, pool);
            save_datas("habitats", builder.modules.habitats, pool);
            save_datas("storage", builder.modules.storage, pool);
            save_datas("docks", builder.modules.docks, pool);
            save_datas("piers", builder.modules.piers, pool);

            pool.wait();
            spdlog::info("Parsing done");
        } catch (const std::exception &e) {
            spdlog::error("{}", e.what());
        }
    }
} // namespace extractor
