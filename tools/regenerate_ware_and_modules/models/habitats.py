from typing import List, Optional
from pydantic import BaseModel
from pydantic_xml import BaseXmlModel, attr, element

class IdentificationXmlModel(BaseXmlModel, tag='identification'):
    name: str = attr()
    shortname: str = attr()
    makerrace: str = attr()
    size: str = attr()
    description: str = attr()

class SetXmlModel(BaseXmlModel, tag='set'):
    ref: str = attr()

class SetsXmlModel(BaseXmlModel, tag='sets'):
    set: List[SetXmlModel] = element(tag='set')

class BuildXmlModel(BaseXmlModel, tag='build'):
    sets: SetsXmlModel = element()

class ExplosionDamageXmlModel(BaseXmlModel, tag='explosiondamage'):
    value: int = attr()
    shield: int = attr()

class HullXmlModel(BaseXmlModel, tag='hull'):
    max: int = attr()

class SecrecyXmlModel(BaseXmlModel, tag='secrecy'):
    level: int = attr()

class WorkforceXmlModel(BaseXmlModel, tag='workforce'):
    race: str = attr()
    capacity: int = attr()

class PropertiesXmlModel(BaseXmlModel, tag='properties'):
    identification: IdentificationXmlModel = element()
    build: BuildXmlModel = element()
    explosiondamage: ExplosionDamageXmlModel = element()
    hull: HullXmlModel = element()
    secrecy: SecrecyXmlModel = element()
    workforce: WorkforceXmlModel = element()

class ComponentXmlModel(BaseXmlModel, tag='component'):
    ref: str = attr()

class MacroXmlModel(BaseXmlModel, tag='macro'):
    name: str = attr()
    class_: str = attr(name='class')
    component: ComponentXmlModel = element()
    properties: PropertiesXmlModel = element()

class MacrosXmlModel(BaseXmlModel, tag='macros'):
    macro: MacroXmlModel = element()