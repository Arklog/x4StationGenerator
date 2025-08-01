import os
from typing import List

from data_types import ProductionDict, ModuleList, StructureDict
from macros.structure import (
    StructureMacroMacrosXmlModel,
    StructureMacroMacroXmlModel,
)

STRUCTURE_ROOT = "assets/structures/landmarks/macros"
structure_map: StructureDict = {}


def load_structures(root: str, habitats_map: ProductionDict = structure_map) -> None:
    """
    Load habitations macro from the given root directory.

    :param root: The root directory where the habitats macros are located.
    :param habitats_map: A dictionary to store the loaded habitats.
    :return:
    """
    lookup_folder = os.path.join(root, STRUCTURE_ROOT)

    print("Loading structure macros")

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
                structure_macro: StructureMacroMacrosXmlModel = (
                    StructureMacroMacrosXmlModel.from_xml(content)
                )
            except Exception as e:
                print(f"Error parsing {filename}: {e}")
                continue
            macro: StructureMacroMacroXmlModel = structure_macro.macro

            structure_map.update({macro.name: macro})


def set_structure(
    modules: ModuleList, structure_map: StructureDict = structure_map
) -> None:
    """
    Set habitats in the modules based on the habitats map.

    :param modules: List of modules to set habitats for.
    :param structure_map: Dictionary containing habitats.
    :return:
    """
    for module in modules:
        structure = structure_map.get(module.macro)

        if not structure:
            continue

        # module.workforce_max = structure.properties.workforce.max

        module.race = structure.properties.identification.makerrace
        module.workforce_capacity = structure.properties.workforce.capacity
        module.type = structure.class_