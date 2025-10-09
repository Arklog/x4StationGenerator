import argparse
import os
import shutil
from typing import List, Dict

from loguru import logger

from loader.habitat import load_habitats, set_habitats
from loader.production import set_production, load_productions
from models.groups import GroupsFileXmlModel, GroupXmlModel
from models.models_out import Module
from models.modules import ModuleFileXmlModel, ModuleXmlModel
from models.waregroups import WareGroupFileXmlModel, WareGroupXmlModel
from models.wares import WareFileXmlModel, WareXmlModel
from utils.lang import parse_lang_file

parser = argparse.ArgumentParser()
parser.add_argument("--x4-root", help="X4 root directory", type=str, required=True)
parser.add_argument("--output", help="Output directory", type=str, required=True)
args = parser.parse_args()

print(args.x4_root)
print(args.output)
print()

if not args.x4_root.endswith("/"):
    args.x4_root = args.x4_root + "/"

outdir = "output"
modules_outdir = f"{outdir}/modules"

if os.path.exists(outdir):
    shutil.rmtree(outdir)

os.makedirs(args.output, exist_ok=True)
os.makedirs(modules_outdir, exist_ok=True)

lang_file = f"t/0001-l044.xml"
wares_file = f"libraries/wares.xml"
modules_file = f"libraries/modules.xml"
modulegroups_file = f"libraries/modulegroups.xml"
waregroups_file = f"libraries/waregroups.xml"

#     print(get_loc(i.name))

wares: List[WareXmlModel] = []
modules: List[WareXmlModel] = []
parse_lang_file(f"{args.x4_root}/{lang_file}")
module_production_map: Dict[str, ModuleXmlModel] = {}
module_macro_to_module_group: Dict[str, GroupXmlModel] = {}
ware_id_to_ware: Dict[str, WareXmlModel] = {}
waregroup_id_to_waregroup: Dict[str, WareGroupXmlModel] = {}


def parse_root(root: str):
    logger.info(f"parsing root: {root}")

    root_wares_file = f"{root}{wares_file}"
    root_modules_file = f"{root}{modules_file}"
    root_modulegroups_file = f"{root}{modulegroups_file}"
    root_waregroups_file = f"{root}{waregroups_file}"

    # Loading data from files
    load_habitats(root)
    load_productions(root)
    with open(root_wares_file, "r", encoding="utf-8") as f:
        input_ware = WareFileXmlModel.from_xml(f.read())
    with open(root_modules_file, "r", encoding="utf-8") as f:
        input_modules = ModuleFileXmlModel.from_xml(f.read())
    with open(root_modulegroups_file, "r", encoding="utf-8") as f:
        input_modulegroups = GroupsFileXmlModel.from_xml(f.read())
    with open(root_waregroups_file, "r", encoding="utf-8") as f:
        input_waregroup = WareGroupFileXmlModel.from_xml(f.read())

    # Merging data into global maps
    global wares
    global module_production_map
    global module_macro_to_module_group
    global ware_id_to_ware
    global waregroup_id_to_waregroup

    module_production_map.update({
        module.id: module for module in input_modules.modules
    })
    module_macro_to_module_group.update({
        select.macro: group for group in input_modulegroups.group for select in group.select
    })
    ware_id_to_ware.update({
        ware.id: ware for ware in input_ware.wares
    })
    wares.extend(input_ware.wares)
    waregroup_id_to_waregroup.update({
        group.id: group for group in input_waregroup.group
    })


parse_root(args.x4_root)

# Selecting all modules from wares
for ware_item in wares:
    if not ware_item.is_module():
        logger.warning(f"not a module: {ware_item.id}")
        continue
    if "xenon" in ware_item.macro.lower():
        logger.warning(f"xenon module: {ware_item.id}")
        continue
    modules.append(ware_item)

output_modules: List[Module] = []
for i in modules:
    module_group = module_macro_to_module_group.get(i.macro)
    if module_group:
        module_xml = module_production_map.get(module_group.name)
    else:
        module_xml = None
        logger.warning(f"module not found: {i.id}")
    item = Module.from_xml_model(i, module_xml, ware_id_to_ware, waregroup_id_to_waregroup)
    if len(item.production) == 0:
        logger.warning(f"production missing: {item.id}")
        continue
    output_modules.append(item)

set_habitats(output_modules)
set_production(output_modules)

for item in output_modules:
    outfilename = f"{modules_outdir}/{item.id}.json"
    with open(outfilename, "w", encoding="utf-8") as f:
        f.write(item.model_dump_json(exclude_none=True))

# with open(f"{outdir}/production_method.json", "w", encoding="utf-8") as f:
#     f.write(input_ware.productions.model_dump_json(exclude_none=True))
