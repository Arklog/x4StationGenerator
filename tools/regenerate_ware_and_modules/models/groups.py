from typing import List

from loguru import logger
from pydantic import model_validator
from pydantic_xml import BaseXmlModel, attr, element


class SelectXmlModel(BaseXmlModel, tag="select"):
    macro: str = attr()


class GroupXmlModel(BaseXmlModel, tag="group"):
    name: str = attr()
    select: List[SelectXmlModel] = element(tag="select", default_factory=list)

    @model_validator(mode='after')
    def check_self(cls, v):
        logger.info(f"parsed group: {v.name}")
        return v


class GroupsFileXmlModel(BaseXmlModel, tag="groups"):
    group: List[GroupXmlModel] = element()
