from typing import List, Optional
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


class ProductionMacroQueueItemXmlModel(BaseXmlModel, tag="item"):
    ware: str = attr()
    method: Optional[str] = attr(default=None)


class ProductionMacroQueueXmlModel(BaseXmlModel, tag="queue"):
    ware: Optional[str] = attr(default=None)
    item: List[ProductionMacroQueueItemXmlModel] = element(default_factory=list)
    # method: str = attr()


class ProductionMacroProductionXmlModel(BaseXmlModel, tag="production"):
    wares: Optional[str] = attr(default=None)
    queue: Optional[ProductionMacroQueueXmlModel] = element(default=None)


class ProductionMacroWorkforceXmlModel(BaseXmlModel, tag="workforce"):
    max: int = attr()


class ProductionMacroPropertiesXmlModel(BaseXmlModel, tag="properties", search_mode="unordered"):
    identification: ProductionMacroIdentificationXmlModel = element()
    build: ProductionMacroBuildXmlModel = element()
    explosiondamage: ProductionMacroExplosionDamageXmlModel = element()
    hull: ProductionMacroHullXmlModel = element()
    secrecy: Optional[ProductionMacroSecrecyXmlModel] = element(default=None)
    production: Optional[ProductionMacroProductionXmlModel] = element(default=None)
    workforce: Optional[ProductionMacroWorkforceXmlModel] = element(default=None)


class ProductionMacroMacroXmlModel(BaseXmlModel, tag="macro"):
    name: str = attr()
    class_: str = attr(name="class")
    component: ProductionMacroComponentXmlModel = element()
    properties: ProductionMacroPropertiesXmlModel = element()


class ProductionMacroMacrosXmlModel(BaseXmlModel, tag="macros"):
    macro: List[ProductionMacroMacroXmlModel] = element()
