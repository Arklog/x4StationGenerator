from pydantic_xml import BaseXmlModel, attr, element


class StructureMacroComponentXmlModel(BaseXmlModel, tag="component"):
    ref: str = attr()


class StructureMacroIdentificationXmlModel(BaseXmlModel, tag="identification"):
    name: str = attr()
    shortname: str = attr()
    makerrace: str = attr()
    size: str = attr()
    description: str = attr()


class StructureMacroExplosionDamageXmlModel(BaseXmlModel, tag="explosiondamage"):
    value: int = attr()
    shield: int = attr()


class StructureMacroHullXmlModel(BaseXmlModel, tag="hull"):
    max: int = attr()


class StructureMacroWorkforceXmlModel(BaseXmlModel, tag="workforce"):
    race: str = attr()
    capacity: int = attr()


class StructureMacroPropertiesXmlModel(BaseXmlModel, tag="properties"):
    identification: StructureMacroIdentificationXmlModel = element()
    explosiondamage: StructureMacroExplosionDamageXmlModel = element()
    hull: StructureMacroHullXmlModel = element()
    workforce: StructureMacroWorkforceXmlModel = element()


class StructureMacroMacroXmlModel(BaseXmlModel, tag="macro"):
    name: str = attr()
    class_: str = attr(name="class")
    component: StructureMacroComponentXmlModel = element()
    properties: StructureMacroPropertiesXmlModel = element()


class StructureMacroMacrosXmlModel(BaseXmlModel, tag="macros"):
    macro: StructureMacroMacroXmlModel = element()
