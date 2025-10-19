//
// Created by pierre on 10/11/25.
//

#ifndef X4STATIONGENERATOR_ID_H
#define X4STATIONGENERATOR_ID_H
#include <set>
#include <stdexcept>
#include <string>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <nlohmann/detail/meta/std_fs.hpp>

#include "spdlog/spdlog.h"

enum class E_IDCLASS {
    IDCLASS_WARE,
    IDCLASS_WARE_GROUP,
    IDCLASS_PRODUCTION_METHOD,
    IDCLASS_MODULE,
    IDCLASS_RACE
};

template<E_IDCLASS IDCLASS, typename IDType = std::string>
class ID {
private:
    static std::set<IDType> allowed_ids_; // Set of registered IDs
    IDType                  id_;          // The actual ID value

public:
    ID() : id_() {
    }

    explicit ID(IDType const &i) : id_(i) {
        if (!allowed_ids_.contains(i))
            throw std::logic_error("ID not registered: " + i);
    }

    ID(const ID &other) : id_(other.id_) {
    }

    ID(ID &&other) noexcept : id_(std::move(other.id_)) {
    }

    static void registerId(const IDType &new_id) {
        if (allowed_ids_.contains(new_id))
            return;
        allowed_ids_.insert(new_id);
    }

    const IDType &raw() const {
        return id_;
    }

    ID &operator=(const ID &other) = default;

    ID &operator=(ID &&other) noexcept = default;

    operator IDType() const { return id_; }
    operator const IDType &() const { return id_; }

    bool operator==(ID const &other) const { return id_ == other.id_; }
    bool operator!=(ID const &other) const { return id_ != other.id_; }
    bool operator<(ID const &other) const { return id_ < other.id_; }
    bool operator<=(ID const &other) const { return id_ <= other.id_; }
    bool operator>(ID const &other) const { return id_ > other.id_; }
    bool operator>=(ID const &other) const { return id_ >= other.id_; }
};

template<E_IDCLASS IDCLASS, typename IDType>
std::set<IDType> ID<IDCLASS, IDType>::allowed_ids_{};

/**
 * Template function to extract ID from nlohmann json
 *
 * @tparam IDCLASS
 * @tparam IDType
 * @param j
 * @param id_value
 */
template<E_IDCLASS IDCLASS, typename IDType = std::string>
void from_json(const nlohmann::json &j, ID<IDCLASS, IDType> &id_value) {
    try {
        auto value = j.template get<IDType>();
        ID<IDCLASS, IDType>::registerId(value);
        id_value = ID<IDCLASS, IDType>{value};
    } catch (std::exception &e) {
        spdlog::error(e.what());
    }
}

typedef ID<E_IDCLASS::IDCLASS_WARE, std::string>              t_ware_id;
typedef ID<E_IDCLASS::IDCLASS_WARE_GROUP, std::string>        t_ware_group_id;
typedef ID<E_IDCLASS::IDCLASS_PRODUCTION_METHOD, std::string> t_production_method_id;
typedef ID<E_IDCLASS::IDCLASS_MODULE, std::string>            t_module_id;
typedef ID<E_IDCLASS::IDCLASS_RACE, std::string>              t_race_id;

/**
 * Hash function for ID
 */
namespace std {
    template<E_IDCLASS IDCLASS, typename IDType>
    struct hash<ID<IDCLASS, IDType> > {
        size_t operator()(const ID<IDCLASS, IDType> &id) const {
            return hash<IDType>{}(id.raw());
        }
    };
}

/**
 * Formatter for ID using fmt library
 */
template<E_IDCLASS IDCLASS, typename IDType>
struct fmt::formatter<ID<IDCLASS, IDType> > : formatter<string_view> {
    auto format(ID<IDCLASS, IDType> id, format_context &ctx) const -> format_context::iterator {
        return formatter<string_view>::format(id.raw(), ctx);
    }
};

#endif // X4STATIONGENERATOR_ID_H
