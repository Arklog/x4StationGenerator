import argparse
import os
import pathlib
import shutil

from models.groups import GroupsFileXmlModel
from models.modules import ModuleFileXmlModel
from models.waregroups import WareGroupFileXmlModel
from models.wares import WareFileXmlModel
from models.models_out import Module
from utils.lang import parse_lang_file

parser = argparse.ArgumentParser()
parser.add_argument("--x4-root", help="X4 root directory", type=str, required=True)
parser.add_argument("--xrcattool", help="XRCatTool binary", type=str, required=True)
parser.add_argument("--output", help="Output directory", type=str, required=True)
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

win_path = fr"z:{args.x4_root.replace('/', '\\\\')}\\"
win_path_wares = f"{win_path}\\08.cat"
win_path_lang = f"{win_path}\\09.cat"

print(win_path)
print(win_path_wares)
print(win_path_lang)
print()

os.system(fr"wine {args.xrcattool} -in {win_path_wares} -out {args.output}")
os.system(fr"wine {args.xrcattool} -in {win_path_lang} -out {args.output}")

lang_file = f"tmp/t/0001-l044.xml"
wares_file = f"tmp/libraries/wares.xml"
modules_file = f"tmp/libraries/modules.xml"
modulegroups_file = f"tmp/libraries/modulegroups.xml"
waregroups_file = f"tmp/libraries/waregroups.xml"

parse_lang_file(lang_file)
with open(wares_file, "r", encoding="utf-8") as f:
    input_ware = WareFileXmlModel.from_xml(f.read())
with open(modules_file, "r", encoding="utf-8") as f:
    input_modules = ModuleFileXmlModel.from_xml(f.read())
with open(modulegroups_file, "r", encoding="utf-8") as f:
    input_modulegroups = GroupsFileXmlModel.from_xml(f.read())
with open(waregroups_file, "r", encoding="utf-8") as f:
    input_waregroup = WareGroupFileXmlModel.from_xml(f.read())

# for i in input_ware.productions.methods:
#     print(get_loc(i.name))

module_production_map = {
    module.id: module for module in input_modules.modules
}
module_macro_to_module_group = {
    select.macro: group for group in input_modulegroups.group for select in group.select
}
ware_id_to_ware = {
    ware.id: ware for ware in input_ware.wares
}
waregroup_id_to_waregroup = {
    group.id: group for group in input_waregroup.group
}
modules = [m for m in input_ware.wares if m.is_module() and not "xenon" in m.macro.lower()]

for i in modules:
    module_group = module_macro_to_module_group.get(i.macro)
    if module_group:
        module_xml = module_production_map.get(module_group.name)
    else:
        module_xml = None
    item = Module.from_xml_model(i, module_xml, ware_id_to_ware, waregroup_id_to_waregroup)
    if len(item.production) == 0:
        continue

    outfilename = f"{modules_outdir}/{item.id}.json"
    with open(outfilename, "w", encoding="utf-8") as f:
        f.write(item.model_dump_json(exclude_none=True))

with open(f"{outdir}/production_method.json", "w", encoding="utf-8") as f:
    f.write(input_ware.productions.model_dump_json(exclude_none=True))