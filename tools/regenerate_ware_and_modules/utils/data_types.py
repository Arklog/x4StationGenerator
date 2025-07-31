from typing import Dict, List

from macros.habitat import HabitatMacroXmlModel
from macros.production import ProductionMacroMacroXmlModel
from models.models_out import Module

ModuleList = List[Module]
HabitatsDict = Dict[str, HabitatMacroXmlModel]
ProductionDict = Dict[str, ProductionMacroMacroXmlModel]
