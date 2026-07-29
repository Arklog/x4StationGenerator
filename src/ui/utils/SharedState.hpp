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
