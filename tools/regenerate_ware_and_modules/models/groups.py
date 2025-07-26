from typing import List
from pydantic_xml import BaseXmlModel, attr, element

class SelectXmlModel(BaseXmlModel, tag="select"):
    macro: str = attr()

class GroupXmlModel(BaseXmlModel, tag="group"):
    name: str = attr()
    select: List[SelectXmlModel] = element(tag="select", default_factory=list)

class GroupsFileXmlModel(BaseXmlModel, tag="groups"):
    group: List[GroupXmlModel] = element()
