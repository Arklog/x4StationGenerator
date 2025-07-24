import re
from typing import List, Dict, TypeVar, Optional, Tuple
from xml.dom.minidom import parse

from pydantic import model_validator, ConfigDict, field_validator
from pydantic_xml import BaseXmlModel, attr, element, wrapped

LangData = Dict[str, Dict[str, str]]


def parse_lang_file(filename: str) -> LangData:
    filecontent = parse(filename)
    retv: LangData = {}

    pages = filecontent.getElementsByTagName("page")
    for page in pages:
        page_id = page.getAttribute("id")
        page_data: Dict[str, str] = {}
        retv[page_id] = page_data

        for item in page.getElementsByTagName("t"):
            item_id = item.getAttribute("id")
            item_value = item.childNodes[0].data

            page_data[item_id] = item_value

    return retv


def get_loc(field: str, lang: LangData) -> str:
    reg = re.compile(r"({\d+,\d+})", flags=re.NOFLAG)
    match = re.search(reg, field)

    while match:
        cmatch = match.group(1)
        cloc = cmatch[1:-1].split(",")
        translation = lang.get(cloc[0], {}).get(cloc[1], "")
        field = field.replace(cmatch, translation)
        match = re.search(reg, field)

    match = re.match(r"\((.+?)\)", field)
    if match:
        return match.group(1)

    return field


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
