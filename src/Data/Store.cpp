//
// Created by pierre on 3/31/26.
//

#include "Store.hpp"

#include "StationBuilder/defines.hpp"
// Store::Store ()
// {
// this->modules.all.reserve (1000);
// }

void Store::registerModule (Module &&module)
{
    if (this->modules.by_id.contains (module.id))
	throw std::runtime_error ("module already registered");

    this->modules.all.push_back (std::move (module));
    auto new_module = &this->modules.all.back ();

    this->modules.by_id.emplace (new_module->id, new_module);
    this->modules.by_name.emplace (new_module->name, new_module);

    // deal with module type

    if (new_module->type == ModuleType::dock
	|| new_module->type == ModuleType::pier)
	this->modules.docks_and_piers.push_back (new_module);
    else if (new_module->type == ModuleType::storage)
	this->modules.storages.push_back (new_module);
    else if (new_module->type == ModuleType::habitat)
	this->modules.habitats.push_back (new_module);
    else if (!new_module->production.empty ())
	this->modules.production.push_back (new_module);

    // deal with module production

    for (auto &ware : new_module->production)
    {
	// register module production
	if (new_module->production_method)
	{
	    std::pair<t_ware_id, t_production_method_id> key
		= {ware.id, new_module->production_method.value ()};
	    this->modules.producing.emplace (key, new_module);
	}
	this->modules.all_producing[ware.id].push_back (new_module);

	if (this->wares.by_id.contains (ware.id))
	    continue;

	this->wares.all.push_back (ware);
	auto new_ware = &this->wares.all.back ();

	this->wares.by_id.emplace (ware.id, new_ware);
	this->wares.by_name.emplace (ware.name, new_ware);

	// Register new ware groups
	if (this->ware_groups.by_id.contains (new_ware->group.id))
	    continue;
	this->ware_groups.all.push_back (new_ware->group);
	auto new_group = &this->ware_groups.all.back ();

	this->ware_groups.by_id.emplace (new_group->id, new_group);
	this->ware_groups.by_name.emplace (new_group->name, new_group);
    }
}