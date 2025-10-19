//
// Created by pierre on 7/20/25.
//

#ifndef DEFINES_HPP
#define DEFINES_HPP

#include "Data/RawData.hpp"

namespace ModuleType {
extern const std::string pier;
extern const std::string dock;
extern const std::string storage;
extern const std::string habitat;
} // namespace ModuleType

struct WareTarget {
  t_ware_id ware_id;
  t_module_id source_module;
  long int prodution;
  bool is_secondary;
};

struct ModuleTarget {
  t_module_id module_id;
  size_t amount;

  bool operator==(const ModuleTarget &other) const;
  bool operator==(const t_module_id &module_id) const;
};

struct Settings {
  std::string name;
  bool workforce_enables;
  t_module_id workforce_module;
};

extern const Settings default_settings;

typedef std::vector<const WareTarget *> t_target_list;
typedef std::vector<t_module_id> t_x4_complex;
typedef std::vector<ModuleTarget> t_module_target_list;

#endif // DEFINES_HPP
