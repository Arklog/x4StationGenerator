//
// Created by pierre on 6/25/26.
//
#include "ExtractorSettings.hpp"

bool extractor::ExtractorSettings::validate () const
{
    using namespace std::filesystem;

    if (!exists (this->XRCatToolPath))
	return false;
    if (!exists (this->XMLPatchPath))
	return false;
    if (!exists (this->X4RootDirPath))
	return false;
    if (!exists (this->OutputDirPath))
	return false;

    return true;
}