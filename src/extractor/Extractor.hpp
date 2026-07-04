//
// Created by pierre on 6/25/26.
//

#ifndef X4STATIONGENERATOR__EXTRACTOR_HPP
#define X4STATIONGENERATOR__EXTRACTOR_HPP

#include "ExtractorSettings.hpp"

namespace extractor {

    /**
     * @class Extractor
     * @brief Provides functionality to extract resources using the
     * configuration provided in ExtractorSettings.
     *
     * The Extractor class is designed to handle resource extraction tasks. It
     * uses the settings specified in an instance of ExtractorSettings to
     * configure paths and other necessary parameters.
     */
    class Extractor
    {
      public:
	Extractor (const ExtractorSettings &settings);

	void extract () const;

      private:
	ExtractorSettings _settings;

	/**
	 * Extract main x4 files
	 */
	void extract_main () const;

	/**
	 * Extract mods and extension
	 */
	void extract_extension () const;

	/**
	 * Patch main x4 files with extension ones
	 */
	void patch_extension () const;
    };

} // namespace extractor

#endif // X4STATIONGENERATOR__EXTRACTOR_HPP
