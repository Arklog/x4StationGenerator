from typing import List

from pydantic import BaseModel

from models.modules import ModuleXmlModel
from models.wares import WareXmlModel, WareProductionXmlModel, WareAmountXmlModel


class Price(BaseModel):
    min: int
    max: int
    avg: int

class Group(BaseModel):
    id: str
    name: str
    tier: int

class WareProduction(BaseModel):
    ware: str
    amount: int

    @staticmethod
    def from_xml_model(model: WareAmountXmlModel) -> 'WareProduction':
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
    wares: List[WareProduction]

    @staticmethod
    def from_xml_model(model: WareProductionXmlModel) -> 'Production':
        return Production(
            time=int(model.time),
            amount=model.amount,
            name=model.name,

            wares=[WareProduction.from_xml_model(model) for model in model.wares],
        )

class Product(BaseModel):
    id: str
    name: str
    transport: str

    production: List[Production]

    @staticmethod
    def from_xml_model(xml_product_model: WareProductionXmlModel, xml_module_model: ModuleXmlModel) -> 'Product':
        return Product(
            id=xml_module_model.category.ware,
            name=xml_module_model.category.name,
            transport=xml_module_model.category.transport,
            production=[Production.from_xml_model(xml_product_model)]
        )

class Module(BaseModel):
    id: str
    name: str
    macro: str

    product: List[Product]

    @staticmethod
    def from_xml_model(xml_ware_model: WareXmlModel, xml_module_model: ModuleXmlModel | None) -> 'Module':

        return Module(
            id=xml_ware_model.id,
            name=xml_ware_model.name,
            macro=xml_ware_model.macro,

            product=[Product.from_xml_model(xml_model, xml_module_model) for xml_model in xml_ware_model.production if xml_module_model is not None],
        )