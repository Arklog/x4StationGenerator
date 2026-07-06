//
// Created by pierre on 7/15/25.
//

#ifndef LOADER_H
#define LOADER_H

#include "Data.hpp"
#include "Store.hpp"
#include "nlohmann/json.hpp"

class Loader
{
  private:
    /**
     * Load module files
     */
    void _loadModules ();

    void _loadWorkforce ();

    /**
     * Parse raw json ware
     */
    void _parse_wares ();

  public:
    Loader (Store &store);

    void load ();

    Store &_store;
};

#endif // LOADER_H
