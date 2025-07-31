from typing import List
from pydantic_xml import BaseXmlModel, attr, element


class HabitatMacroComponentXmlModel(BaseXmlModel, tag="component"):
    ref: str = attr()


class HabitatMacroIdentificationXmlModel(BaseXmlModel, tag="identification"):
    name: str = attr()
    shortname: str = attr()
    makerrace: str = attr()
    size: str = attr()
    description: str = attr()


class HabitatMacroSetXmlModel(BaseXmlModel, tag="set"):
    ref: str = attr()


class HabitatMacroSetsXmlModel(BaseXmlModel, tag="sets"):
    set: List[HabitatMacroSetXmlModel] = element()


class HabitatMacroBuildXmlModel(BaseXmlModel, tag="build"):
    sets: HabitatMacroSetsXmlModel = element()


class HabitatMacroExplosionDamageXmlModel(BaseXmlModel, tag="explosiondamage"):
    value: int = attr()


class HabitatMacroHullXmlModel(BaseXmlModel, tag="hull"):
    max: int = attr()


class HabitatMacroSecrecyXmlModel(BaseXmlModel, tag="secrecy"):
    level: int = attr()


class HabitatMacroWorkforceXmlModel(BaseXmlModel, tag="workforce"):
    race: str = attr()
    capacity: int = attr()


class HabitatMacroPropertiesXmlModel(BaseXmlModel, tag="properties"):
    identification: HabitatMacroIdentificationXmlModel = element()
    build: HabitatMacroBuildXmlModel = element()
    explosiondamage: HabitatMacroExplosionDamageXmlModel = element()
    hull: HabitatMacroHullXmlModel = element()
    secrecy: HabitatMacroSecrecyXmlModel = element()
    workforce: HabitatMacroWorkforceXmlModel = element()


class HabitatMacroXmlModel(BaseXmlModel, tag="macro"):
    name: str = attr()
    class_: str = attr(name="class")
    component: HabitatMacroComponentXmlModel = element()
    properties: HabitatMacroPropertiesXmlModel = element()


class HabitatMacrosXmlModel(BaseXmlModel, tag="macros"):
    macro: List[HabitatMacroXmlModel] = element()
