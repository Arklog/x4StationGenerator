//
// Created by pierre on 7/20/25.
//

#include "StationBuilder/Generator/ComplexGeneratorBase.hpp"
#include "Data/WareModuleAndWorkforce.hpp"
#include "utils/FmtVectorFormatter.hpp"

#include "spdlog/spdlog.h"

bool ComplexGeneratorBase::_done (const t_target_container &targets,
				  t_target_container &current_state,
				  t_x4_complex &modules) const
{
    // check all primary targets ok
    for (auto primary_target : targets.getPrimaryTargets ())
    {
	auto current_target
	    = current_state.getPrimaryTarget (primary_target->ware_id);

	if (current_target->prodution < primary_target->prodution)
	{
	    return false;
	}
    }

    // check secondary targets ok
    for (auto secondary_target : current_state.getSecondaryTargets ())
    {
	if (secondary_target->prodution < 0
	    && isWareProduced (secondary_target->ware_id))
	{
	    return false;
	}
    }

    return true;
}

void ComplexGeneratorBase::_step (const t_target_container &targets,
				  t_target_container &current_state,
				  t_x4_complex &modules)
{
    spdlog::info ("Step: {}", current_step_);
    const auto &wares = store_.wares.all;
    const auto &ware = this->_nextTarget (targets, current_state, modules);

    const auto &module_to_add = store_.modules.by_id.at (ware->source_module);
    const auto &module_production = module_to_add->getProduction ();

    auto amount_produced = module_production.amount;
    if (settings_.workforce_enables)
    {
	amount_produced
	    = static_cast<long> (static_cast<double> (amount_produced)
				 * module_production.getWorkforceFactor ());
    }
    if (ware->ware_id == t_ware_id{"energycells"})
    {
	amount_produced = static_cast<long> (
	    static_cast<double> (amount_produced) * settings_.sunlight);
    }

    // Update production value of all ware produced and consumed by
    // module_to_add
    spdlog::info ("Module production: {}", module_production.wares);
    this->_updateCurrentProduction (ware->ware_id, amount_produced,
				    module_production.time);
    for (const auto &i : module_production.wares)
    {
	this->_updateCurrentProduction (i.id, -(i.amount),
					module_production.time);
    }
    modules.push_back (module_to_add->id);

    // deal with workforce
    if (!settings_.workforce_enables)
	return;

    if (!module_to_add->workforce_max)
	return;

    auto habitat = store_.modules.by_id.at (settings_.workforce_module);
    auto consumption
	= getWorkforceUsage (habitat->race.value (),
			     module_to_add->workforce_max.value ());
    spdlog::info ("Workforce consumption: {}", consumption);
    for (const auto &i : consumption)
    {
	this->_updateCurrentProduction (i.id, -(i.amount), 3600);
    }
    workforce_ -= module_to_add->workforce_max.value ();

    while (workforce_ < 0)
    {
	workforce_ += habitat->workforce_capacity.value ();
	modules.push_back (habitat->id);
    }
}

WareTarget *
ComplexGeneratorBase::_nextTarget (const t_target_container &targets,
				   t_target_container &current_state,
				   t_x4_complex &modules)
{
    spdlog::debug ("determining next target");
    std::vector<t_ware_id> ids{};

    auto all_wares = current_state.getPrimaryAndSecondaryTargets ();
    // Get all potential next targets
    for (const auto &ware : all_wares)
    {
	const auto &key = ware->ware_id;

	if (!isWareProduced (key))
	    continue;

	auto is_base_target = targets.isPrimaryTarget (key);
	auto is_done
	    = is_base_target
		  ? targets.getPrimaryTarget (key)->prodution <= ware->prodution
		  : ware->prodution >= 0;

	if (!is_done)
	    ids.push_back (key);
    }

    std::ranges::sort (ids, [current_state, this] (const t_ware_id &a,
						   const t_ware_id &b) {
	const auto &a_ware = current_state.getTarget (a);
	const auto &b_ware = current_state.getTarget (b);

	auto a_neg = b_ware->prodution < 0;
	auto b_neg = a_ware->prodution < 0;

	if (a_neg || b_neg)
	    return a_ware->prodution < b_ware->prodution;

	const auto &wares = this->store_.wares.by_id;
	const auto a_ware_data = wares.at (a_ware->ware_id);
	const auto b_ware_data = wares.at (a_ware->ware_id);

	return a_ware_data->group.tier > b_ware_data->group.tier;
    });

    spdlog::debug ("next target {}", ids.at (0).raw ());
    return current_state.getTarget (ids[0]);
}

void ComplexGeneratorBase::_updateCurrentProduction (const t_ware_id &ware_id,
						     long int value,
						     long int cycle_time)
{
    if (!isWareProduced (ware_id))
    {
	spdlog::info ("ware {} is not produced, skipping", ware_id.raw ());
	return;
    }
    spdlog::debug ("update current production {}", ware_id.raw ());

    auto is_produced = this->current_production_.isPrimaryTarget (ware_id)
		       || this->current_production_.isSecondaryTarget (ware_id);
    double mult = 3600.0f / (double) (cycle_time);
    value *= mult;

    // If ware is not in the list of produced ware
    // Happens if required to produce another ware
    if (is_produced)
    {
	this->current_production_.getTarget (ware_id)->prodution += value;
	return;
    }

    spdlog::debug ("adding ware in list {}", ware_id.raw ());
    this->current_production_.setSecondaryTarget (ware_id);
    auto target = this->current_production_.getSecondaryTarget (ware_id);
    target->prodution = value;
}

ComplexGeneratorBase::ComplexGeneratorBase (const Settings &settings,
					    const Store &store,
					    WareTargetContainer &targets)
    : targets_ (targets), current_production_{store}, settings_ (settings),
      store_ (store)
{
    // Copy all targets production methods
    for (const auto &target : targets.getTargets ())
    {
	auto current_target = current_production_.getTarget (target.ware_id);
	current_target->source_module = target.source_module;
    }

    // Set primary targets
    auto primary_targets = targets_.getPrimaryTargets ();
    for (const auto &target : primary_targets)
    {
	current_production_.setPrimaryTarget (target->ware_id);
	current_production_.getPrimaryTarget (target->ware_id)->prodution = 0;
    }
}

t_x4_complex ComplexGeneratorBase::build ()
{
    t_x4_complex result{};
    workforce_ = 0;
    spdlog::info ("Starting complex generation");

    current_step_ = 0;
    sunlight_ = this->settings_.sunlight;
    while (!_done (this->targets_, this->current_production_, result))
    {
	_step (this->targets_, this->current_production_, result);
	++current_step_;
    }

    spdlog::info ("Complex generated");
    return result;
}

const ComplexGeneratorBase::t_target_container &
ComplexGeneratorBase::getCurrentProduction () const
{
    return current_production_;
}
