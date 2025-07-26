from typing import List, Optional
from pydantic_xml import BaseXmlModel, attr, element

class WareGroupXmlModel(BaseXmlModel, tag="group"):
    id: str = attr()
    name: str = attr()
    factoryname: Optional[str] = attr(default=None)
    icon: Optional[str] = attr(default=None)
    factorymapicon: Optional[str] = attr(default=None)
    factoryhudicon: Optional[str] = attr(default=None)
    tags: Optional[str] = attr(default=None)
    tier: Optional[int] = attr(default=None)
    priority: Optional[int] = attr(default=None)

class WareGroupFileXmlModel(BaseXmlModel, tag="groups"):
    group: List[WareGroupXmlModel] = element()
