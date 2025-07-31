from typing import List
from pydantic_xml import BaseXmlModel, attr, element


class ProductionMacroComponentXmlModel(BaseXmlModel, tag="component"):
    ref: str = attr()


class ProductionMacroIdentificationXmlModel(BaseXmlModel, tag="identification"):
    name: str = attr()
    # shortname: str = attr()
    # makerrace: str = attr()
    description: str = attr()


class ProductionMacroSetXmlModel(BaseXmlModel, tag="set"):
    ref: str = attr()


class ProductionMacroSetsXmlModel(BaseXmlModel, tag="sets"):
    set: List[ProductionMacroSetXmlModel] = element()


class ProductionMacroBuildXmlModel(BaseXmlModel, tag="build"):
    sets: ProductionMacroSetsXmlModel = element()


class ProductionMacroExplosionDamageXmlModel(BaseXmlModel, tag="explosiondamage"):
    value: int = attr()
    shield: int = attr(default=0)


class ProductionMacroHullXmlModel(BaseXmlModel, tag="hull"):
    max: int = attr()


class ProductionMacroSecrecyXmlModel(BaseXmlModel, tag="secrecy"):
    level: int = attr()


class ProductionMacroQueueXmlModel(BaseXmlModel, tag="queue"):
    ware: str = attr()
    # method: str = attr()


class ProductionMacroProductionXmlModel(BaseXmlModel, tag="production"):
    wares: str = attr()
    queue: ProductionMacroQueueXmlModel = element(default=ProductionMacroQueueXmlModel(ware="null"))


class ProductionMacroWorkforceXmlModel(BaseXmlModel, tag="workforce"):
    max: int = attr()


class ProductionMacroPropertiesXmlModel(BaseXmlModel, tag="properties"):
    identification: ProductionMacroIdentificationXmlModel = element()
    build: ProductionMacroBuildXmlModel = element()
    explosiondamage: ProductionMacroExplosionDamageXmlModel = element()
    hull: ProductionMacroHullXmlModel = element()
    secrecy: ProductionMacroSecrecyXmlModel = element()
    production: ProductionMacroProductionXmlModel = element()
    workforce: ProductionMacroWorkforceXmlModel = element()


class ProductionMacroMacroXmlModel(BaseXmlModel, tag="macro"):
    name: str = attr()
    class_: str = attr(name="class")
    component: ProductionMacroComponentXmlModel = element()
    properties: ProductionMacroPropertiesXmlModel = element()


class ProductionMacroMacrosXmlModel(BaseXmlModel, tag="macros"):
    macro: List[ProductionMacroMacroXmlModel] = element()
