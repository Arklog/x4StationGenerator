from typing import Dict, List

from groups import GroupXmlModel
from macros.habitat import HabitatMacroXmlModel
from macros.production import ProductionMacroMacroXmlModel
from macros.structure import StructureMacroMacroXmlModel
from models.models_out import Module
from modules import ModuleXmlModel
from waregroups import WareGroupXmlModel
from wares import WareXmlModel

ModuleList = List[Module]
HabitatsDict = Dict[str, HabitatMacroXmlModel]
ProductionDict = Dict[str, ProductionMacroMacroXmlModel]
StructureDict = Dict[str, StructureMacroMacroXmlModel]

ModuleProductionDict = Dict[str, ModuleXmlModel]
ModuleMacroGroupDict = Dict[str, GroupXmlModel]
WareDict = Dict[str, WareXmlModel]
WareGroupIdWareGroupDict = Dict[str, WareGroupXmlModel]