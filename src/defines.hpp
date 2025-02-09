//
// Created by pierre on 2/9/25.
//

#ifndef X4STATIONGENERATOR_DEFINES_HPP
#define X4STATIONGENERATOR_DEFINES_HPP

#include <string>
#include <map>

enum class RESSOURCE {
    METHANE,
    ORE,
    ENERGY_CELL,
    GRAPHENE,
    REFINED_METAL,
    HULL_PART,
};

typedef enum MODULE_TYPE_ENUM {
    DOCK              = 1,
    PIER              = DOCK << 1,
    STORAGE           = PIER << 1,
    HABITAT           = STORAGE << 1,
    PRODUCTION        = HABITAT << 1,
    DOCK_OR_PIER      = DOCK | PIER,
    DOCK_PIER_STORAGE = DOCK | PIER | STORAGE,
} MODULE_TYPE;

struct StationSize {
    int x_plus;
    int x_minus;
    int y_plus;
    int y_minus;
    int z_plus;
    int z_minus;
};

struct Module {
    std::string              name;
    std::string              macro;
    MODULE_TYPE              type;
    std::map<RESSOURCE, int> ressources_produced;
    std::map<RESSOURCE, int> ressources_produced_max;
    std::map<RESSOURCE, int> ressources_consumed;
    int                      workforce_max;

    [[nodiscard]] std::map<RESSOURCE, int> getTotal(int nmodules = 1, bool workforce_is_max = false) const;
};

#endif //X4STATIONGENERATOR_DEFINES_HPP
