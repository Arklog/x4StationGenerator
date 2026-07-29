//
// Created by pierre on 7/27/26.
//

#ifndef X4STATIONGENERATOR_SRC_UI_UTILS_SHAREDSTATE_HPP
#define X4STATIONGENERATOR_SRC_UI_UTILS_SHAREDSTATE_HPP
#include <QObject>

#include "Managed.hpp"
#include "common/types/StationSaveFile.hpp"
#include "stationbuilder/Complex.hpp"
#include "stationbuilder/defines.hpp"

namespace ui::utils {
    class SharedState : public QObject {
        Q_OBJECT

    public:
        SharedState() = default;

        auto settings() {
            return Managed{
                settings_, [this](common::stationbuilder::Settings &) {
                    emit this->settingsChanged(settings_);
                }
            };
        }

        auto complex() {
            return Managed{
                complex_, [this](common::stationbuilder::Complex &) {
                    emit this->complexChanged(complex_);
                }
            };
        }

        auto toStationSaveFile() const {
            common::types::StationSaveFile save_file;

            if (settings_.name.empty())
                throw std::runtime_error{"Station is missing a name"};
            if (settings_.workforce_module.empty())
                throw std::runtime_error{"Invalid habitat id"};

            save_file.name       = settings_.name;
            save_file.sun        = settings_.sunlight;
            save_file.workforce  = settings_.workforce_enables;
            save_file.habitat_id = settings_.workforce_module;

            save_file.docks             = {};
            save_file.storages          = {};
            save_file.primary_targets   = {};
            save_file.secondary_targets = {};

            std::ranges::for_each(settings_.docks, [&](auto &dock) {
                save_file.docks[dock.module_id] = dock.amount;
            });
            std::ranges::for_each(settings_.storages, [&](auto &st) {
                save_file.storages[st.module_id] = st.amount;
            });
            std::ranges::for_each(settings_.ware_targets.getPrimaryTargets(), [&](auto target) {
                save_file.primary_targets.push_back(*target);
            });
            std::ranges::for_each(complex_.wares.getSecondaryTargets(), [&](auto target) {
                save_file.secondary_targets.push_back(*target);
            });

            return save_file;
        };

    signals:
        void settingsChanged(const common::stationbuilder::Settings &);

        void complexChanged(const common::stationbuilder::Complex &);

        void saveFileLoaded();

    private:
        common::stationbuilder::Settings settings_;
        common::stationbuilder::Complex  complex_;
    };
}

#endif //X4STATIONGENERATOR_SRC_UI_UTILS_SHAREDSTATE_HPP
