import os
from typing import List

from loguru import logger

from models.macros.habitat import HabitatMacrosXmlModel, HabitatMacroXmlModel
from utils.data_types import HabitatsDict, ModuleList

HABITATS_ROOT = "assets/structures/habitat/macros"
habitats_map: HabitatsDict = {}


def load_habitats(root: str, habitats_map: HabitatsDict = habitats_map) -> None:
    """
    Load habitations macro from the given root directory.

    :param root: The root directory where the habitats macros are located.
    :param habitats_map: A dictionary to store the loaded habitats.
    :return:
    """
    lookup_folder = os.path.join(root, HABITATS_ROOT)

    logger.info("loading habitat macros")
    if not os.path.exists(lookup_folder):
        raise FileNotFoundError(f"Habitat macros directory not found: {lookup_folder}")

    for filename in os.listdir(lookup_folder):
        filename = os.path.join(lookup_folder, filename)

        if not os.path.isfile(filename):
            raise FileNotFoundError(f"Not a file: {filename}")
        if not filename.endswith(".xml"):
            raise ValueError(f"Invalid file type: {filename}, expected .xml")

        with open(filename, "r", encoding="utf-8") as f:
            content = f.read()
            habitat_macro: HabitatMacrosXmlModel = HabitatMacrosXmlModel.from_xml(content)
            macros: List[HabitatMacroXmlModel] = habitat_macro.macro

            habitats_map.update({
                macro.name: macro for macro in macros
            })


def set_habitats(modules: ModuleList, habitats_map: HabitatsDict = habitats_map) -> None:
    """
    Set habitats in the modules based on the habitats map.

    :param modules: List of modules to set habitats for.
    :param habitats_map: Dictionary containing habitats.
    :return:
    """
    for module in modules:
        habitat = habitats_map.get(module.macro)

        if not habitat:
            continue

        module.workforce_capacity = habitat.properties.workforce.capacity
        module.race = habitat.properties.identification.makerrace
