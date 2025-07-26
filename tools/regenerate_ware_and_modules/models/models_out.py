from typing import List, Dict, Optional

from pydantic import BaseModel, field_validator

from models.modules import ModuleXmlModel
from models.waregroups import WareGroupXmlModel
from models.wares import (
    WareXmlModel,
    WareProductionXmlModel,
    WareAmountXmlModel,
    PriceXmlModel,
)
from utils.lang import get_loc


class Price(BaseModel):
    min: int
    max: int
    avg: int

    @staticmethod
    def from_xml_model(model: PriceXmlModel) -> "Price":
        return Price(
            min=model.min,
            max=model.max,
            avg=model.avg,
        )


class Group(BaseModel):
    id: str
    name: str
    tier: int

    @staticmethod
    def from_xml_model(model: WareGroupXmlModel) -> "Group":
        return Group(
            id=model.id,
            name=model.name,
            tier=model.tier,
        )

    @field_validator("name", mode="after")
    def validate_name(cls, value: str) -> str:
        return get_loc(value)


class WareProduction(BaseModel):
    ware: str
    amount: int

    @staticmethod
    def from_xml_model(model: WareAmountXmlModel) -> "WareProduction":
        return WareProduction(
            ware=model.ware,
            amount=model.amount,
        )


class Effect(BaseModel):
    type: str
    product: float


class Production(BaseModel):
    time: int
    amount: int
    name: str
    method: str
    wares: List[WareProduction]

    @staticmethod
    def from_xml_model(model: WareProductionXmlModel) -> "Production":
        return Production(
            time=int(model.time),
            amount=model.amount,
            name=model.name,
            method=model.method,
            wares=[WareProduction.from_xml_model(model) for model in model.wares],
        )

    @field_validator("name", mode="after")
    def validate_name(cls, value: str) -> str:
        return get_loc(value)


class Product(BaseModel):
    id: str
    name: str
    volume: int
    transport: str
    price: Price
    group: Group

    production: List[Production]

    @staticmethod
    def from_xml_model(
        xml_product_model: WareProductionXmlModel,
        xml_module_model: ModuleXmlModel,
        wares: Dict[str, WareXmlModel],
        waregroups: Dict[str, WareGroupXmlModel],
    ) -> "Product":
        ware = wares[xml_module_model.category.ware]
        group = waregroups.get(ware.group)
        return Product(
            id=ware.id,
            name=ware.name,
            volume=ware.volume,
            transport=ware.transport,
            price=Price.from_xml_model(ware.price),
            group=Group.from_xml_model(group),
            production=[
                Production.from_xml_model(production) for production in ware.production
            ],
        )

    @field_validator("name", mode="after")
    def validate_name(cls, value: str) -> str:
        return get_loc(value)

class Module(BaseModel):
    id: str
    name: str
    macro: str
    type: Optional[str]
    production_method: Optional[str]

    price: Price
    product: Optional[List[Product]]
    production: List[Production]

    @staticmethod
    def from_xml_model(
        xml_ware_model: WareXmlModel,
        xml_module_model: ModuleXmlModel | None,
        wares: Dict[str, WareXmlModel],
        waregroups: Dict[str, WareGroupXmlModel],
    ) -> "Module":

        module_type = xml_module_model.category.type if xml_module_model else None
        product = [
            Product.from_xml_model(xml_model, xml_module_model, wares, waregroups)
            for xml_model in xml_ware_model.production
            if (xml_module_model and xml_module_model.category.ware)
        ]
        if len(product) == 0:
            product = None

        production_method = None
        owners = [o.faction for o in xml_ware_model.owners]
        if product:
            production_method = "default"
        if len(owners) == 1 and "teladi" in owners:
            production_method = "teladi"

        return Module(
            id=xml_ware_model.id,
            name=xml_ware_model.name,
            macro=xml_ware_model.macro,
            type=module_type,
            production_method=production_method,
            price=Price.from_xml_model(xml_ware_model.price),
            product=product,
            production=[
                Production.from_xml_model(xml_model)
                for xml_model in xml_ware_model.production
            ],
        )

    @field_validator("name", mode="after")
    def validate_name(cls, value: str) -> str:
        return get_loc(value)
