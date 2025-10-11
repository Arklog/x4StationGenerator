//
// Created by pierre on 10/11/25.
//

#ifndef X4STATIONGENERATOR_ID_H
#define X4STATIONGENERATOR_ID_H
#include <set>
#include <stdexcept>
#include <string>

template <unsigned IDCLASS, typename IDType = std::string> class ID {
private:
  static std::set<IDType> allowed_ids_;
  IDType id_;

public:
  ID() : id_() {}
  explicit ID(IDType const &i) : id_(i) {
    if (!allowed_ids_.contains(i))
      throw std::logic_error("ID not registered: " + i);
  }

  ID(const ID &other) : id_(other.id_) {}
  ID(ID &&other) noexcept : id_(std::move(other.id_)) {}

  static void registerId(const IDType &new_id) {
    if (allowed_ids_.contains(new_id))
      throw std::logic_error("ID already registered: " + new_id);
    allowed_ids_.insert(new_id);
  }

  ID &operator=(const ID &other) = default;
  ID &operator=(ID &&other) noexcept = default;

  operator IDType() const { return id_; }
  operator const IDType() const { return id_; }
  // operator IDType &() { return id_; }
  operator const IDType &() const { return id_; }

  bool operator==(ID const &other) const { return id_ == other.id_; }
  bool operator!=(ID const &other) const { return id_ != other.id_; }
  bool operator<(ID const &other) const { return id_ < other.id_; }
  bool operator<=(ID const &other) const { return id_ <= other.id_; }
  bool operator>(ID const &other) const { return id_ > other.id_; }
  bool operator>=(ID const &other) const { return id_ >= other.id_; }
};

typedef ID<0, std::string> t_ware_id;

#endif // X4STATIONGENERATOR_ID_H
