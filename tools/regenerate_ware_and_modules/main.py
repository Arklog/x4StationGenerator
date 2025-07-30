import argparse
import os
import pathlib
import re
import shutil
from multiprocessing import Pool, cpu_count
from typing import List, Dict, Tuple

from groups import GroupsFileDiffXmlModel
from loader.habitat import load_habitats, set_habitats
from models.groups import GroupsFileXmlModel, GroupXmlModel
from models.modules import ModuleFileXmlModel, ModuleXmlModel
from models.waregroups import WareGroupFileXmlModel, WareGroupXmlModel
from models.wares import WareFileXmlModel, WareXmlModel
from models.models_out import Module
from modules import ModuleFileDiffXmlModel
from utils.lang import parse_lang_file
from waregroups import WareGroupFileDiffXmlModel
from wares import WareFileDiffXmlModel

parser = argparse.ArgumentParser()
parser.add_argument("--x4-root", help="X4 root directory", type=str, required=True)
parser.add_argument("--xrcattool", help="XRCatTool binary", type=str, required=True)
parser.add_argument("--output", help="Output directory", type=str, required=True)
parser.add_argument("--no-extract", action="store_true", help="Do not extract .cat files")
args = parser.parse_args()

print(args.x4_root)
print(args.xrcattool)
print(args.output)
print()

outdir = "output"
modules_outdir = f"{outdir}/modules"

if os.path.exists(outdir):
    shutil.rmtree(outdir)

os.makedirs(args.output, exist_ok=True)
os.makedirs(modules_outdir, exist_ok=True)

def to_win_path(p: str, no_prefix: bool = False) -> str:
    tmp = fr"{p.replace('/', '\\\\').replace(" ", "\\ ")}"
    if no_prefix:
        return tmp
    return fr"z:{tmp}"

root_paths: List[str] = [
    "/",
    "/extensions/ego_dlc_boron/",
    "/extensions/ego_dlc_mini_01/",
    "/extensions/ego_dlc_pirate/",
    "/extensions/ego_dlc_split/",
    "/extensions/ego_dlc_terran/",
    "/extensions/ego_dlc_terran/"
]
win_paths: List[str] = []

# for root_path in root_paths:
#     out_path = os.path.join(args.output, root_path.lstrip("/"))
#     if not os.path.exists(out_path):
#         print(f"Creating output directory: {out_path}")
#         os.makedirs(out_path, exist_ok=True)
#
#     full_root_path = os.path.join(args.x4_root, root_path.lstrip("/"))
#     print(f"Checking path: {full_root_path}")
#     if not os.path.exists(full_root_path):
#         print(f"Path does not exist: {full_root_path}")
#         continue
#
#     if os.path.isdir(full_root_path):
#         for fname in os.listdir(full_root_path):
#             if re.match(r"(?:\d{2}|ext_\d{2})\.cat$", fname):
#                 fpath = os.path.join(full_root_path, fname)
#                 print(f"Found .cat file: {fpath}")
#                 win_paths.append(to_win_path(fpath))

def run_that_shit(xrcattool: str, fpath: str, out_path: str) -> int:
    print(f"Running {xrcattool} -in {fpath} -out {out_path}")
    return os.system(rf"wine {xrcattool} -in {fpath} -out {out_path}")

# run_that_shit(args.xrcattool, " ".join(win_paths), to_win_path(args.output))

# for i in range(1, 10):
#     win_paths.append(fr"z:{args.x4_root.replace('/', '\\\\')}\\\\0{i}.cat")


with Pool(10) as pool:
    args_list: List[Tuple[str, str, str]] = []

    for root_path in root_paths:
        out_path = os.path.join(args.output, root_path.lstrip("/"))
        # out_path = args.output
        if not os.path.exists(out_path):
            print(f"Creating output directory: {out_path}")
            os.makedirs(out_path, exist_ok=True)

        full_root_path = os.path.join(args.x4_root, root_path.lstrip("/"))
        print(f"Checking path: {full_root_path}")
        if not os.path.exists(full_root_path):
            print(f"Path does not exist: {full_root_path}")
            continue

        if os.path.isdir(full_root_path):
            for fname in os.listdir(full_root_path):
                if re.match(r"(?:\d{2}|ext_\d{2})\.cat$", fname):
                    fpath = os.path.join(full_root_path, fname)
                    print(f"Found .cat file: {fpath}")
                    args_list.append((args.xrcattool, to_win_path(fpath), to_win_path(out_path.lstrip('/').lstrip('\\'), no_prefix=True)))

    if not args.no_extract:
        pool.starmap(run_that_shit, args_list, 3)


lang_file = f"t/0001-l044.xml"
wares_file = f"libraries/wares.xml"
modules_file = f"libraries/modules.xml"
modulegroups_file = f"libraries/modulegroups.xml"
waregroups_file = f"libraries/waregroups.xml"

#     print(get_loc(i.name))

wares: List[WareXmlModel] = []
modules: List[WareXmlModel] = []
parse_lang_file(f"{args.output}/{lang_file}")
module_production_map: Dict[str, ModuleXmlModel] = {}
module_macro_to_module_group: Dict[str, GroupXmlModel] = {}
ware_id_to_ware: Dict[str, WareXmlModel] = {}
waregroup_id_to_waregroup: Dict[str, WareGroupXmlModel] = {}

def parse_root(root: str):
    print(f"Parsing root: {root}")
    root_wares_file = f"{root}{wares_file}"
    root_modules_file = f"{root}{modules_file}"
    root_modulegroups_file = f"{root}{modulegroups_file}"
    root_waregroups_file = f"{root}{waregroups_file}"

    load_habitats(root)
    with open(root_wares_file, "r", encoding="utf-8") as f:
        input_ware = WareFileXmlModel.from_xml(f.read())
    with open(root_modules_file, "r", encoding="utf-8") as f:
        input_modules = ModuleFileXmlModel.from_xml(f.read())
    with open(root_modulegroups_file, "r", encoding="utf-8") as f:
        input_modulegroups = GroupsFileXmlModel.from_xml(f.read())
    with open(root_waregroups_file, "r", encoding="utf-8") as f:
        input_waregroup = WareGroupFileXmlModel.from_xml(f.read())

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


for root_path in root_paths[:1]:
    root_path = os.path.join(args.output, root_path.lstrip("/"))
    parse_root(root_path.lstrip("/"))

modules = [m for m in wares if m.is_module() and not "xenon" in m.macro.lower()]
output_modules: List[Module] = []
for i in modules:
    module_group = module_macro_to_module_group.get(i.macro)
    if module_group:
        module_xml = module_production_map.get(module_group.name)
    else:
        module_xml = None
    item = Module.from_xml_model(i, module_xml, ware_id_to_ware, waregroup_id_to_waregroup)
    if len(item.production) == 0:
        continue
    output_modules.append(item)

set_habitats(output_modules)
for item in output_modules:
    outfilename = f"{modules_outdir}/{item.id}.json"
    with open(outfilename, "w", encoding="utf-8") as f:
        f.write(item.model_dump_json(exclude_none=True))

# with open(f"{outdir}/production_method.json", "w", encoding="utf-8") as f:
#     f.write(input_ware.productions.model_dump_json(exclude_none=True))
