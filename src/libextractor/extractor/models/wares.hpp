
#pragma once

#include "wares_xsd.hpp"

namespace wares {

    struct waresType;
    struct productionRootType;
    struct methodType;
    struct methodDefaultType;
    struct defaultsType;
    struct wareBodyType;
    struct priceType;
    struct wareProductionType;
    struct primaryType;
    struct wareAmount;
    struct effectsType;
    struct effectType;
    struct researchType;
    struct componentType;
    struct containerType;
    struct iconType;
    struct sourcesType;
    struct sourceType;
    struct restrictionType;
    struct useType;
    struct ownerType;
    struct illegalType;
    struct softwareType;
    struct wareType;

    struct waresType
    {
	xsd::optional<wares::productionRootType> production;
	xsd::vector<wares::defaultsType> defaults;
	xsd::vector<wares::wareType> ware;
    };

    struct productionRootType
    {
	xsd::vector<wares::methodType> method;
    };

    struct wareBodyType
    {
	xsd::vector<wares::priceType> price;
	xsd::vector<wares::wareProductionType> production;
	xsd::vector<wares::researchType> research;
	xsd::vector<wares::componentType> component;
	xsd::vector<wares::containerType> container;
	xsd::vector<wares::iconType> icon;
	xsd::vector<wares::sourcesType> sources;
	xsd::vector<wares::restrictionType> restriction;
	xsd::vector<wares::useType> use;
	xsd::vector<wares::ownerType> owner;
	xsd::vector<wares::illegalType> illegal;
	xsd::vector<wares::softwareType> software;
    };

    struct defaultsType : wares::wareBodyType
    {
	xsd::string id;
	xsd::optional<xsd::string> name;
	xsd::string transport;
	double volume;
	xsd::optional<xsd::string> tags;
    };

    struct wareType : wares::wareBodyType
    {
	xsd::string id;
	xsd::optional<xsd::string> name;
	xsd::optional<xsd::string> description;
	xsd::optional<xsd::string> factoryname;
	xsd::optional<xsd::string> group;
	xsd::string transport;
	double volume;
	xsd::optional<xsd::string> tags;
	xsd::optional<xsd::string> shortname;
	xsd::optional<int64_t> sortorder;
	xsd::optional<xsd::string> licence;
	xsd::optional<xsd::string> illegal;
    };

    struct methodType
    {
	xsd::string id;
	xsd::optional<xsd::string> name;
	xsd::optional<xsd::string> tags;
	xsd::vector<wares::methodDefaultType> default_;
    };

    struct priceType
    {
	double min;
	double average;
	double max;
    };

    struct wareProductionType
    {
	double time;
	double amount;
	xsd::string method;
	xsd::optional<xsd::string> name;
	xsd::optional<xsd::string> tags;
	xsd::optional<double> dismantlefactor;
	xsd::optional<wares::primaryType> primary;
	xsd::optional<wares::effectsType> effects;
	xsd::optional<wares::researchType> research;
    };

    struct researchType
    {
	xsd::optional<double> time;
	xsd::vector<wares::primaryType> primary;
	xsd::vector<wares::researchType> research;
	xsd::vector<wares::wareAmount> ware;
    };

    struct componentType
    {
	xsd::string ref;
    };

    struct containerType
    {
	xsd::string ref;
    };

    struct iconType
    {
	xsd::optional<xsd::string> active;
	xsd::optional<xsd::string> video;
    };

    struct sourcesType
    {
	xsd::vector<wares::sourceType> source;
    };

    struct restrictionType
    {
	xsd::optional<xsd::string> licence;
	xsd::optional<xsd::string> buy;
    };

    struct useType
    {
	xsd::optional<double> threshold;
	xsd::optional<xsd::string> factions;
	xsd::optional<xsd::string> purposes;
    };

    struct ownerType
    {
	xsd::string faction;
    };

    struct illegalType
    {
	xsd::string factions;
    };

    struct softwareType
    {
	xsd::string predecessor;
    };

    struct methodDefaultType
    {
	xsd::string race;
    };

    struct primaryType
    {
	xsd::vector<wares::wareAmount> ware;
    };

    struct effectsType
    {
	xsd::vector<wares::effectType> effect;
    };

    struct wareAmount
    {
	xsd::string ware;
	xsd::optional<double> amount;
    };

    struct sourceType
    {
	xsd::string type;
    };

    struct effectType
    {
	xsd::string type;
	xsd::optional<double> product;
    };

    void load_file (const std::string &file, waresType &wares);
    void load_data (const std::string &data, waresType &wares);

} // namespace wares
