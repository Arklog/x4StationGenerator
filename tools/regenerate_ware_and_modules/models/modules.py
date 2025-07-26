from pydantic import ConfigDict
from pydantic_xml import BaseXmlModel, attr, element
from typing import List, Optional

class ProductionXmlModel(BaseXmlModel, tag='production'):
    ware: Optional[str] = attr(default=None)
    ref: Optional[str] = attr(default=None)
    chance: Optional[int] = attr(default=None)


class CompatibilitiesXmlModel(BaseXmlModel, tag='compatibilities'):
    limits_production: Optional[int] = element(name='limits', default=None)
    maxlimits_production: Optional[int] = element(name='maxlimits', default=None)
    productions: List[ProductionXmlModel] = element(tag='production', default_factory=list)


class CategoryXmlModel(BaseXmlModel, tag='category'):
    ware: Optional[str] = attr(default=None)
    tags: Optional[str] = attr(default=None)
    race: Optional[str] = attr(default=None)
    faction: Optional[str] = attr(default=None)

    @property
    def type(self) -> str:
        tmp = self.tags.replace("[", "").replace("]", "").split(",")
        return tmp[0]


class ModuleXmlModel(BaseXmlModel, tag='module'):
    id: str = attr()
    group: str = attr()
    category: CategoryXmlModel = element(tag='category')
    compatibilities: Optional[CompatibilitiesXmlModel] = element(default=None)

class ModuleFileXmlModel(BaseXmlModel, tag='modules'):
    model_config = ConfigDict(extra="ignore")

    modules: List[ModuleXmlModel] = element(tag='module')