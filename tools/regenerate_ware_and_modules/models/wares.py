from typing import List, Dict, Optional

from pydantic import ConfigDict
from pydantic_xml import BaseXmlModel, attr, element, wrapped

from utils.lang import get_loc

LangData = Dict[str, Dict[str, str]]


class ProductionMethodXmlModel(BaseXmlModel, tag="method"):
    id: str = attr("id")
    name: str = attr("name")

    # tags: List[str] = attr("tags")


class ProductionsXmlModel(BaseXmlModel, tag="production"):
    methods: List[ProductionMethodXmlModel] = element(tag="method")


class DefaultsXmlModel(BaseXmlModel, tag="defaults"):
    id: str = attr("id")


class PriceXmlModel(BaseXmlModel, tag="price"):
    min: int = attr("min")
    max: int = attr("max")
    avg: int = attr("average")

    def to_json(self):
        return self.model_dump_json()


class WareAmountXmlModel(BaseXmlModel, tag="ware"):
    ware: str = attr()
    amount: int = attr()

    def to_json(self):
        return self.model_dump_json()


class WareProductionEffectXmlModel(BaseXmlModel, tag="effect"):
    type: str = attr()
    product: float = attr()

    def to_json(self):
        return self.model_dump_json()


class WareProductionXmlModel(BaseXmlModel):
    time: float = attr("time")
    amount: int = attr("amount")
    name: str = attr("name")


    wares: List[WareAmountXmlModel] = wrapped(
        "primary", element(tag="ware", default_factory=list)
    )
    effects: List[WareProductionEffectXmlModel] = wrapped(
        "effects", element(tag="effect", default_factory=list)
    )

    def translate_fields(self, lang: LangData) -> None:
        self.name = get_loc(self.name, lang)

    def to_json(self):
        return self.model_dump_json()


class ComponentXmlModel(BaseXmlModel, tag='component'):
    ref: str = attr()

class WareXmlModel(BaseXmlModel, tag="ware"):
    model_config = ConfigDict(extra="ignore")

    id: str = attr("id")
    name: str = attr(name="name")
    group: Optional[str] = attr(name="group", default=None)
    transport: str = attr(name="transport")
    volume: str = attr(name="volume")
    tags: Optional[str] = attr(name="tags", default=None)

    price: PriceXmlModel = element(tag="price")
    production: Optional[List[WareProductionXmlModel]] = element(tag="production", default_factory=list)
    # research: Optional[Research] = element(tag="research", default=None)
    components: List[ComponentXmlModel] = element(tag="component", default_factory=list)

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

    # def is_ware(self) -> bool:
    #     return self.production is not None

    def translate_fields(self, lang: LangData) -> None:
        self.name = get_loc(self.name, lang)

        if self.production:
            for p in self.production:
                p.translate_fields(lang)


class WareFileXmlModel(BaseXmlModel, tag="wares"):
    model_config = ConfigDict(extra="ignore")

    productions: ProductionsXmlModel = element(tag="production")
    defaults: DefaultsXmlModel = element(tag="defaults")
    wares: List[WareXmlModel] = element(tag="ware", default=[])
