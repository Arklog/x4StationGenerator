import os
from typing import List

import elementpath
from xml.etree import ElementTree

from data_types import (
    ModuleProductionDict,
    ModuleMacroGroupDict,
    WareDict,
    WareGroupIdWareGroupDict,
)
from groups import GroupXmlModel
from loader.habitat import load_habitats
from loader.production import load_productions
from loader.structure import load_structures
from modules import ModuleXmlModel
from waregroups import WareGroupXmlModel
from wares import WareXmlModel


def load_wares(ware_file: str) -> List[WareXmlModel]:
    """
    Load wares from the specified root directory.

    Args:
        root (str): The root directory where wares are located.

    Returns:
        dict: A dictionary containing loaded wares.
    """
    if not os.path.exists(ware_file):
        return []

    with open(ware_file, "r", encoding="utf-8") as file:
        content = file.read()
    etree = ElementTree.XML(content)
    elements = elementpath.select(etree, "add/ware")

    ware_list: List[WareXmlModel] = []
    for element in elements:
        ware = WareXmlModel.from_xml_tree(element)
        ware_list.append(ware)

    return ware_list


def load_modules(module_file: str) -> List[ModuleXmlModel]:
    """
    Load modules from the specified root directory.

    Args:
        module_file (str): The path to the module file.

    Returns:
        List[ModuleXmlModel]: A list of loaded modules.
    """
    if not os.path.exists(module_file):
        return []

    with open(module_file, "r", encoding="utf-8") as file:
        content = file.read()
    etree = ElementTree.XML(content)
    elements = elementpath.select(etree, "add/module")

    module_list: List[ModuleXmlModel] = []
    for element in elements:
        module = ModuleXmlModel.from_xml_tree(element)
        module_list.append(module)

    return module_list


def load_modulegroups(module_groups_file: str) -> List[GroupXmlModel]:
    """
    Load module groups from the specified root directory.

    Args:
        module_groups_file (str): The path to the module groups file.

    Returns:
        List[GroupXmlModel]: A list of loaded module groups.
    """
    if not os.path.exists(module_groups_file):
        return []

    with open(module_groups_file, "r", encoding="utf-8") as file:
        content = file.read()
    etree = ElementTree.XML(content)
    elements = elementpath.select(etree, "group")

    group_list: List[GroupXmlModel] = []
    for element in elements:
        group = GroupXmlModel.from_xml_tree(element)
        group_list.append(group)

    return group_list


def load_waregroups(ware_groups_file: str) -> List[WareGroupXmlModel]:
    """
    Load ware groups from the specified root directory.

    Args:
        ware_groups_file (str): The path to the ware groups file.

    Returns:
        List[GroupXmlModel]: A list of loaded ware groups.
    """
    if not os.path.exists(ware_groups_file):
        return []

    with open(ware_groups_file, "r", encoding="utf-8") as file:
        content = file.read()
    etree = ElementTree.XML(content)
    elements = elementpath.select(etree, "add/group")

    group_list: List[WareGroupXmlModel] = []
    for element in elements:
        group = WareGroupXmlModel.from_xml_tree(element)
        group_list.append(group)

    return group_list


def load_extension(
    root: str,
    main_wares: List[WareXmlModel],
    module_production: ModuleProductionDict,
    module_macro: ModuleMacroGroupDict,
    ware_id_to_ware: WareDict,
    waregroup_id_to_waregroup: WareGroupIdWareGroupDict
):
    ware_file_path = os.path.join(root, "libraries/wares.xml")
    module_file_path = os.path.join(root, "libraries/modules.xml")
    module_groups_file_path = os.path.join(root, "libraries/modulegroups.xml")
    ware_groups_file_path = os.path.join(root, "libraries/waregroups.xml")

    wares = load_wares(ware_file_path)
    modules = load_modules(module_file_path)
    module_groups = load_modulegroups(module_groups_file_path)
    ware_groups = load_waregroups(ware_groups_file_path)

    try:
        load_habitats(root)
    except FileNotFoundError:
        print(f"Habitat file not found in {root}, skipping habitat loading.")
    try:
        load_productions(root)
    except FileNotFoundError:
        print(f"Productions file not found in {root}, skipping production loading.")

    try:
        load_structures(root)
    except FileNotFoundError:
        print(f"Structures file not found in {root}, skipping structure loading.")

    main_wares.extend(wares)
    ware_id_to_ware.update({
        ware.id: ware for ware in wares
    })
    module_production.update({
        module.id: module for module in modules
    })
    module_macro.update({
        select.macro: group for group in module_groups for select in group.select
    })
    waregroup_id_to_waregroup.update({
        group.id: group for group in ware_groups
    })