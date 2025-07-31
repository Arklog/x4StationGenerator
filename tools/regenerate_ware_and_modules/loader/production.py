import os
from typing import List

from data_types import ProductionDict, ModuleList
from macros.production import (
    ProductionMacroMacrosXmlModel,
    ProductionMacroMacroXmlModel,
)

PRODUCTION_ROOT = "assets/structures/production/macros"
production_map: ProductionDict = {}


def load_productions(root: str, habitats_map: ProductionDict = production_map) -> None:
    """
    Load habitations macro from the given root directory.

    :param root: The root directory where the habitats macros are located.
    :param habitats_map: A dictionary to store the loaded habitats.
    :return:
    """
    lookup_folder = os.path.join(root, PRODUCTION_ROOT)

    print("Loading production macros")

    if not os.path.exists(lookup_folder):
        raise FileNotFoundError(
            f"Productions macros directory not found: {lookup_folder}"
        )

    for filename in os.listdir(lookup_folder):
        filename = os.path.join(lookup_folder, filename)
        print(filename)

        if not os.path.isfile(filename):
            raise FileNotFoundError(f"Not a file: {filename}")
        if not filename.endswith(".xml"):
            raise ValueError(f"Invalid file type: {filename}, expected .xml")

        with open(filename, "r", encoding="utf-8") as f:
            content = f.read()
            try:
                production_macro: ProductionMacroMacrosXmlModel = (
                    ProductionMacroMacrosXmlModel.from_xml(content)
                )
            except Exception as e:
                print(f"Error parsing {filename}: {e}")
                continue
            macros: List[ProductionMacroMacroXmlModel] = production_macro.macro

            habitats_map.update({macro.name: macro for macro in macros})


def set_production(
    modules: ModuleList, production_map: ProductionDict = production_map
) -> None:
    """
    Set habitats in the modules based on the habitats map.

    :param modules: List of modules to set habitats for.
    :param production_map: Dictionary containing habitats.
    :return:
    """
    for module in modules:
        production = production_map.get(module.macro)

        if not production:
            continue

        module.workforce_max = production.properties.workforce.max
        # module.race = production.properties.identification.makerrace
