//
// Created by pierre on 7/5/26.
//

#include "X4Patchable.hpp"

#include <spdlog/spdlog.h>

#include "extractor/utils.hpp"
#include "extractor/extraction_logic/Extension.hpp"

extractor::X4Patchable::X4Patchable(const std::filesystem::path &in,
                                    const std::filesystem::path &out) :
in{in},
out{out} {
    output_exists = std::filesystem::exists(out);
}

void extractor::X4Patchable::patch(const std::filesystem::path &xml_diff_exec, CacheFile<std::string, bool> &cache) {
    if (cache.contains(in.string()) && cache.get(in.string())) {
        spdlog::info("File {} already patched", in.string());
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
    
    std::filesystem::recursive_directory_iterator it(extension->output_tmp);
    auto do_we_want_to_patch_this = [](const std::filesystem::path &path) -> bool {
        std::regex lang_file_reg{"t/0001-l044.xml"};
        for (auto const &reg: X4Extractable::extraction_targets) {
            if (std::regex_search(path.string(), reg))
                return true;
        }

        return std::regex_search(path.string(), lang_file_reg);
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
