from typing import List, Dict, Optional

from pydantic import ConfigDict, model_validator
from pydantic_xml import BaseXmlModel, attr, element, wrapped

from utils.lang import get_loc

LangData = Dict[str, Dict[str, str]]

class ProductionDefaultXmlModel(BaseXmlModel, tag="default"):
    model_config = ConfigDict(extra="ignore")
    race: str = attr()

class ProductionMethodXmlModel(BaseXmlModel, tag="method"):
    model_config = ConfigDict(extra="ignore")
    id: str = attr("id")
    name: str = attr("name")
    default: Optional[ProductionDefaultXmlModel] = element(default=None)
    # tags: List[str] = attr("tags")


class ProductionsXmlModel(BaseXmlModel, tag="production"):
    model_config = ConfigDict(extra="ignore")
    methods: List[ProductionMethodXmlModel] = element(tag="method")


class DefaultsXmlModel(BaseXmlModel, tag="defaults"):
    model_config = ConfigDict(extra="ignore")
    id: str = attr("id")


class PriceXmlModel(BaseXmlModel, tag="price"):
    model_config = ConfigDict(extra="ignore")
    min: int = attr("min")
    max: int = attr("max")
    avg: int = attr("average")


class WareAmountXmlModel(BaseXmlModel, tag="ware"):
    model_config = ConfigDict(extra="ignore")
    ware: str = attr()
    amount: int = attr()

    def to_json(self):
        return self.model_dump_json()


class WareProductionEffectXmlModel(BaseXmlModel, tag="effect"):
    model_config = ConfigDict(extra="ignore")
    type: str = attr()
    product: float = attr()


class WareProductionXmlModel(BaseXmlModel):
    model_config = ConfigDict(extra="ignore")
    time: float = attr("time")
    amount: int = attr("amount")
    name: str = attr("name")
    method: str = attr("method")

    wares: List[WareAmountXmlModel] = wrapped(
        "primary", element(tag="ware", default_factory=list)
    )
    effects: List[WareProductionEffectXmlModel] = wrapped(
        "effects", element(tag="effect", default_factory=list)
    )


class ComponentXmlModel(BaseXmlModel, tag="component"):
    model_config = ConfigDict(extra="ignore")
    ref: str = attr()


class ResearchWareXmlModel(BaseXmlModel, tag="ware"):
    model_config = ConfigDict(extra="ignore")
    ware: str = attr(name="ware")


class InnerResearchXmlModel(BaseXmlModel, tag="research"):
    model_config = ConfigDict(extra="ignore")
    ware: ResearchWareXmlModel = element()


class ResearchXmlModel(BaseXmlModel, tag="research"):
    model_config = ConfigDict(extra="ignore")
    time: Optional[int] = attr(default=None)
    research: Optional[InnerResearchXmlModel] = element(default=None)

class RestrictionXmlModel(BaseXmlModel):
    model_config = ConfigDict(extra="ignore")
    licence: str = attr()

class WareOwnerXmlModel(BaseXmlModel, tag="owner"):
    model_config = ConfigDict(extra="ignore")
    faction: str = attr()

class WareXmlModel(BaseXmlModel, tag="ware"):
    model_config = ConfigDict(extra="ignore")

    id: str = attr("id")
    name: str = attr(name="name")
    group: Optional[str] = attr(name="group", default=None)
    transport: str = attr(name="transport")
    volume: str = attr(name="volume")
    tags: Optional[str] = attr(name="tags", default=None)

    price: PriceXmlModel = element(tag="price")
    production: Optional[List[WareProductionXmlModel]] = element(
        tag="production", default_factory=list
    )
    research: Optional[ResearchXmlModel] = element(tag="research", default=None)
    components: List[ComponentXmlModel] = element(tag="component", default_factory=list)
    restriction: Optional[RestrictionXmlModel] = element(tag="restriction", default=None)
    owners: List[WareOwnerXmlModel] = element(tag="owner", default_factory=list)

    @property
    def macro(self) -> str:
        if len(self.components) == 0:
            return None
        return self.components[0].ref

    def is_module(self) -> bool:
        return (
            self.tags is not None
            and self.production is not None
            and "module" in self.tags
        )


class WareFileXmlModel(BaseXmlModel, tag="wares"):
    model_config = ConfigDict(extra="ignore")
    productions: ProductionsXmlModel = element(tag="production")
    defaults: DefaultsXmlModel = element(tag="defaults")
    wares: List[WareXmlModel] = element(tag="ware", default=[])

class AddXmlModel(BaseXmlModel, tag="add"):
    model_config = ConfigDict(extra="ignore")
    sel: str = attr()
    method: Optional[ProductionMethodXmlModel] = element(default=None)
    ware: Optional[List[WareXmlModel]] = element(tag="ware", default=None)

class WareFileDiffXmlModel(BaseXmlModel, tag="diff"):
    model_config = ConfigDict(extra="ignore")
    adds: List[AddXmlModel] = element(tag="add", default_factory=list)

    @model_validator(mode="after")
    def check_self(cls, v):
        print(f"Found {len(v.adds[0].ware)} wares in diff")
        return v