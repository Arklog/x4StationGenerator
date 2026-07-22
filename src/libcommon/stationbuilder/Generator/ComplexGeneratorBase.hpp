//
// Created by pierre on 7/20/25.
//

#ifndef COMPLEXGENERATOR_HPP
#define COMPLEXGENERATOR_HPP
#include "stationbuilder/defines.hpp"
#include "data/Store.hpp"
#include "stationbuilder/Complex.hpp"
#include "utils/WareTargetContainer.hpp"

namespace common::stationbuilder::generator {
    /**
     * A complex generator generate a new complex based on a provided list of
     * ressources
     */
    class ComplexGeneratorBase {
    public:
        using t_ware_id = types::Ware::ware_id;
        typedef std::unordered_map<t_ware_id, WareTarget, std::hash<std::string> >
        t_target_map;

        using t_target_container = utils::WareTargetContainer;

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
                           t_target_container &      current_state,
                           t_x4_complex &            modules) const;

        /**
         * Next step in the generation
         *
         * @param targets The ware targets list
         * @param current_state The current wares produced
         * @param modules The current list of modules to be built
         */
        virtual void _step(const t_target_container &targets, t_target_container &current_state,
                           t_x4_complex &            modules);

        /**
         * Choose the next target to build
         *
         * @param targets The list of end targets
         * @param current_state The list of currently produced wares
         * @param modules The end list of modules to be built
         * @return
         */
        virtual WareTarget *_nextTarget(const t_target_container &targets,
                                        t_target_container &      current_state,
                                        t_x4_complex &            modules);

        /**
         * Update the production value of the ware identified by ware_id by values.
         * If ware_id does not identify a ware in the production chain, it will be
         * added.
         *
         * @param ware_id The identifier of the ware to update
         * @param value The value by which to update the ware, must be negative when
         * the ware is consumed.
         */
        void _updateCurrentProduction(const t_ware_id &ware_id, long int value);

        void _add_workforce(size_t amount, t_x4_complex &modules);

        utils::WareTargetContainer &targets_;
        utils::WareTargetContainer  current_production_;
        const Settings &            settings_;
        const data::Store &         store_;
        size_t                      workforce_max_;
        size_t                      workforce_current_;
        size_t                      current_step_;
        double                      sunlight_;

    public:
        ComplexGeneratorBase(const Settings &settings, const data::Store &store, utils::WareTargetContainer &targets);

        ComplexGeneratorBase(const ComplexGeneratorBase &complex_generator) = delete;

        virtual ~ComplexGeneratorBase() = default;

        Complex build();

        const t_target_container &getCurrentProduction() const;
    };
}
#endif // COMPLEXGENERATOR_HPP
