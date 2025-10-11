//
// Created by pierre on 7/20/25.
//

#include "StationBuilder/Generator/ComplexGeneratorBase.hpp"

#include "Data/WareModuleAndWorkforce.hpp"

#include "spdlog/spdlog.h"

bool ComplexGeneratorBase::_done(const t_target_container &targets,
                                 t_target_container &current_state,
                                 t_x4_complex &modules) const {
  for (const auto &iter : current_state) {
    const auto &key = iter.first;
    const auto &ware = iter.second;

    bool is_neg = ware.prodution < 0;
    if (is_neg && isWareProduced(ware.ware_id))
      return false;

    bool is_target = targets.contains(key);
    if (is_target && ware.prodution < targets.at(ware.ware_id).prodution)
      return false;
  }

  return true;
}

void ComplexGeneratorBase::_step(const t_target_container &targets,
                                 t_target_container &current_state,
                                 t_x4_complex &modules) {
  spdlog::info("Step ComplexGeneratorBase");
  const auto &wares = getWares();
  const auto &ware = this->_nextTarget(targets, current_state, modules);

  const auto &module_to_add = getModules().at(ware.source_module);
  const auto &module_production = module_to_add->getProduction();

  auto amount_produced = module_production.amount;
  if (settings_.workforce_enables) {
    amount_produced = static_cast<long>(static_cast<double>(amount_produced) *
                                        module_production.getWorkforceFactor());
  }

  // Update production value of all ware produced and consumed by module_to_add
  this->_updateCurrentProduction(ware.ware_id, amount_produced,
                                 module_production.time);
  for (const auto &i : module_production.wares) {
    this->_updateCurrentProduction(i.id, -(i.amount), module_production.time);
  }
  modules.push_back(module_to_add->id);

  // deal with workforce
  if (!settings_.workforce_enables)
    return;

  auto habitat = getModules().at(settings_.workforce_module);
  auto consumption = getWorkforceUsage(habitat->race.value(),
                                       module_to_add->workforce_max.value());
  for (const auto &i : consumption) {
    this->_updateCurrentProduction(i.id, -(i.amount), 3600);
  }
  workforce_ -= module_to_add->workforce_max.value();

  while (workforce_ < 0) {
    workforce_ += habitat->workforce_capacity.value();
    modules.push_back(habitat->id);
  }
}

WareTarget &ComplexGeneratorBase::_nextTarget(const t_target_container &targets,
                                              t_target_container &current_state,
                                              t_x4_complex &modules) {
  spdlog::debug("determining next target");
  std::vector<t_ware_id> ids{};

  ids.reserve(current_state.size());

  // Get all potential next targets
  for (const auto &ware_checked : current_state) {
    if (!isWareProduced(ware_checked.first))
      continue;

    auto const &key = ware_checked.first;
    auto const &ware = ware_checked.second;
    auto is_base_target = targets.contains(key);
    auto is_done = is_base_target ? targets.at(key).prodution <= ware.prodution
                                  : ware.prodution >= 0;

    if (!is_done)
      ids.push_back(key);
  }

  std::ranges::sort(ids,
                    [current_state](const t_ware_id &a, const t_ware_id &b) {
                      const auto &a_ware = current_state.at(a);
                      const auto &b_ware = current_state.at(b);

                      auto a_neg = b_ware.prodution < 0;
                      auto b_neg = a_ware.prodution < 0;

                      if (a_neg || b_neg)
                        return a_ware.prodution < b_ware.prodution;

                      const auto &wares = getWares();
                      const auto a_ware_data = wares.at(a_ware.ware_id);
                      const auto b_ware_data = wares.at(a_ware.ware_id);

                      return a_ware_data->group.tier > b_ware_data->group.tier;
                    });

  spdlog::debug("next target {}", ids[0]);
  return current_state[ids[0]];
}

void ComplexGeneratorBase::_updateCurrentProduction(const t_ware_id &ware_id,
                                                    long int value,
                                                    long int cycle_time) {
  spdlog::debug("update current production {}", ware_id);
  auto is_produced = this->current_production_.contains(ware_id);
  double mult = 3600.0f / (double)(cycle_time);
  value *= mult;

  if (is_produced) {
    this->current_production_.at(ware_id).prodution += value;
    return;
  }

  spdlog::debug("adding ware in list {}", ware_id);
  this->current_production_.emplace(std::pair<t_ware_id, WareTarget>{
      ware_id, WareTarget{.ware_id = ware_id,
                          .source_module = "default",
                          .prodution = value}});
}

ComplexGeneratorBase::ComplexGeneratorBase(const Settings &settings,
                                           WareTargetContainer &targets)
    : targets_(targets), current_production_{}, settings_(settings) {
  auto primary_targets = targets_.getPrimaryTargets();
  for (const auto &target : primary_targets) {
    current_production_.setPrimaryTarget(target->ware_id);
  }
}

t_x4_complex ComplexGeneratorBase::build() {
  t_x4_complex result{};
  workforce_ = 0;
  spdlog::info("staring complex generation");

  while (!_done(this->targets_, this->current_production_, result)) {
    spdlog::info("entering new step in complex generation");
    _step(this->targets_, this->current_production_, result);
  }

  spdlog::info("finished complex generation");
  return result;
}
