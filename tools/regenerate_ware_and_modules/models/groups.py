from typing import List
from pydantic_xml import BaseXmlModel, attr, element

class SelectXmlModel(BaseXmlModel, tag="select"):
    macro: str = attr()

class GroupXmlModel(BaseXmlModel, tag="group"):
    name: str = attr()
    select: SelectXmlModel = element()

class GroupsFileXmlModel(BaseXmlModel, tag="groups"):
    group: List[GroupXmlModel] = element()
