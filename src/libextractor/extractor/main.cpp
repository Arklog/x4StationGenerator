#include "extractor/Extractor.hpp"
#include "extractor/ExtractorSettings.hpp"
//
// Created by pierre on 6/25/26.
//
int main(int argc, char **argv) {
    extractor::ExtractorSettings settings{};

    settings.XRCatToolPath
            = "/home/pierre/Documents/x4StationGenerator/tools/XRCatTool.exe";
    settings.ExtractionDirPath = "/home/pierre/Documents/x4StationGenerator/data";
    settings.X4RootDirPath
            = "/home/pierre/.local/share/Steam/steamapps/common/X4 Foundations";
    settings.XMLPatchPath = "/home/pierre/Documents/x4StationGenerator/tools/"
            "XMLDiffAndPatch/XMLPatch.exe";
    settings.nthreads      = 10;
    settings.OutputDirPath = "/home/pierre/Documents/x4StationGenerator/assets";
    extractor::Extractor extractor{settings};
    extractor.extract();
    extractor.parse();
}
