//
// Created by pierre on 7/5/26.
//

#include "X4Patchable.hpp"

#include <spdlog/spdlog.h>

#include <rfl/xml.hpp>
#include "extractor/utils.hpp"
#include "extractor/extraction_logic/Extension.hpp"

extractor::X4Patchable::X4Patchable(const std::filesystem::path &in,
                                    const std::filesystem::path &out) :
in{in},
out{out} {
    output_exists = std::filesystem::exists(out);
}

void extractor::X4Patchable::patch(const std::filesystem::path & xml_diff_exec, LangFile &lang_file,
                                   CacheFile<std::string, bool> &cache) {
    if (cache.contains(in.string()) && cache.get(in.string())) {
        spdlog::info("File {} already patched", in.string());
        return;
    }

    auto is_this_lang_file = [](const std::filesystem::path &path) -> bool {
        std::vector<std::regex> lang_file_reg
                {std::regex{"t/0001-[lL]044.xml"}, std::regex{"t/0001.xml"}};
        return std::ranges::any_of(lang_file_reg, [&path](const std::regex &reg) {
            return std::regex_search(path.string(), reg);
        });
    };

    if (is_this_lang_file(in)) {
        patch_lang(lang_file);
        return;
    }

    spdlog::info("Patching file {}", in.string());
    cache.register_entry(in.string(), false);
    cache.save();

    if (output_exists)
        patch_diff(xml_diff_exec);
    else
        patch_move();

    cache.register_entry(in.string(), true);
    cache.save();
    spdlog::info("File {} patched", in.string());
}

void extractor::X4Patchable::insert_patchable(const Extension *extension, std::vector<X4Patchable *> &patchables) {
    if (!exists(extension->output_tmp)) {
        spdlog::warn("Extension {} have not been extracted", extension->output_tmp.string());
        return;
    }

    spdlog::info("Listing patchable files from {}", extension->output_tmp.string());

    std::filesystem::recursive_directory_iterator it(extension->output_tmp);
    auto do_we_want_to_patch_this = [](const std::filesystem::path &path) -> bool {
        std::vector<std::regex> lang_file_reg{std::regex{"t/0001-[Ll]044.xml"}, std::regex{"t/0001.xml"}};
        lang_file_reg.insert_range(lang_file_reg.begin(), X4Extractable::extraction_targets);

        return std::ranges::any_of(lang_file_reg, [&path](const std::regex &reg) {
            return std::regex_search(path.string(), reg);
        });
    };

    for (auto &item: it) {
        const auto &path = item.path();
        if (!std::filesystem::is_regular_file(path) || path.extension() != ".xml" || !do_we_want_to_patch_this(path))
            continue;

        auto relative_path = std::filesystem::relative(path, extension->output_tmp);
        auto output_path   = extension->output / relative_path;
        patchables.push_back(new X4Patchable(path, output_path));
    }
}

void extractor::X4Patchable::patch_move() {
    std::filesystem::create_directories(out.parent_path());
    std::filesystem::rename(in, out);
}

void extractor::X4Patchable::patch_diff(const std::filesystem::path &xml_diff_exec) {
    auto command = generate_command(xml_diff_exec, "-o", out, "-d", in, "-u", out);

    system(command.c_str());
}

void extractor::X4Patchable::patch_lang(LangFile &lang_file) {
    auto      t     = rfl::xml::load<models::T>(in.string());
    auto      tdiff = rfl::xml::load<models::TDiff>(in.string());
    models::T v{};

    if (tdiff.has_value())
        v = tdiff.value().add;
    else
        v = t.value();

    lang_file.add_t(std::move(v));
    spdlog::info("Translation file {} patched", in.string());
}
