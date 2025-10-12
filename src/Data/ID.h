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

template<unsigned IDCLASS, typename IDType = std::string>
class ID {
private:
    static std::set<IDType> allowed_ids_;
    IDType id_;

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
    // operator const IDType() const { return id_; }
    // operator IDType &() { return id_; }
    operator const IDType &() const { return id_; }

    bool operator==(ID const &other) const { return id_ == other.id_; }
    bool operator!=(ID const &other) const { return id_ != other.id_; }
    bool operator<(ID const &other) const { return id_ < other.id_; }
    bool operator<=(ID const &other) const { return id_ <= other.id_; }
    bool operator>(ID const &other) const { return id_ > other.id_; }
    bool operator>=(ID const &other) const { return id_ >= other.id_; }
};

template<unsigned IDCLASS, typename IDType>
std::set<IDType> ID<IDCLASS, IDType>::allowed_ids_{};

typedef ID<0, std::string> t_ware_id;

template<unsigned IDCLASS, typename IDType = std::string>
void from_json(const nlohmann::json &j, ID<IDCLASS, IDType> &id_value) {
    try {
        auto value = j.template get<IDType>();
        ID<IDCLASS, IDType>::registerId(value);
        id_value = ID<IDCLASS, IDType>{value};
    } catch (std::exception &e) {
        spdlog::error(e.what());
    }
}

#endif // X4STATIONGENERATOR_ID_H
