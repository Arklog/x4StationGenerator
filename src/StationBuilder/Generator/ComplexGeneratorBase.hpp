//
// Created by pierre on 7/20/25.
//

#ifndef COMPLEXGENERATOR_HPP
#define COMPLEXGENERATOR_HPP
#include "../defines.hpp"
#include "utils/WareTargetContainer.hpp"

/**
 * A complex generator generate a new complex based on a provided list of
 * ressources
 */
class ComplexGeneratorBase {
public:
  typedef std::unordered_map<t_ware_id, WareTarget, std::hash<std::string>>
      t_target_map;

  using t_target_container = WareTargetContainer;

protected:
  /**
   * Check if we are done generating the complex.
   *
   * @param targets The ware target list
   * @param current_state The current ware produced
   * @param modules The current list of modules to be build
   *
   * @return True if the generation is done, else false
   */
  virtual bool _done(const t_target_container &targets,
                     t_target_container &current_state,
                     t_x4_complex &modules) const;

  /**
   * Next step in the generation
   *
   * @param targets The ware targets list
   * @param current_state The current wares produced
   * @param modules The current list of modules to be built
   */
  virtual void _step(const t_target_container &targets,
                     t_target_container &current_state, t_x4_complex &modules);

  /**
   * Choose the next target to build
   *
   * @param targets The list of end targets
   * @param current_state The list of currently produced wares
   * @param modules The end list of modules to be built
   * @return
   */
  virtual WareTarget &_nextTarget(const t_target_container &targets,
                                  t_target_container &current_state,
                                  t_x4_complex &modules);

  /**
   * Update the production value of the ware identified by ware_id by values.
   * If ware_id does not identify a ware in the production chain, it will be
   * added.
   *
   * @param ware_id The identifier of the ware to update
   * @param value The value by which to update the ware, must be negative when
   * the ware is consumed.
   */
  void _updateCurrentProduction(const t_ware_id &ware_id, long int value,
                                long int cycle_time);

  WareTargetContainer &targets_;
  WareTargetContainer current_production_;
  const Settings &settings_;
  long int workforce_;

public:
  ComplexGeneratorBase(const Settings &settings, WareTargetContainer &targets);

  ComplexGeneratorBase(const ComplexGeneratorBase &complex_generator) = delete;

  virtual ~ComplexGeneratorBase() = default;

  t_x4_complex build();
};

#endif // COMPLEXGENERATOR_HPP
