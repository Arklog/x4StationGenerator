import argparse
import os
import pathlib

from models.modules import ModuleFileXmlModel
from models.wares import parse_lang_file, WareFileXmlModel, WareXmlModel
from models.models_out import Module

parser = argparse.ArgumentParser()
parser.add_argument("--x4-root", help="X4 root directory", type=str, required=True)
parser.add_argument("--xrcattool", help="XRCatTool binary", type=str, required=True)
parser.add_argument("--output", help="Output directory", type=str, required=True)
args = parser.parse_args()

print(args.x4_root)
print(args.xrcattool)
print(args.output)
print()

os.makedirs(args.output, exist_ok=True)
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

lang = parse_lang_file(lang_file)

with open(wares_file, "r", encoding="utf-8") as f:
    input_ware = WareFileXmlModel.from_xml(f.read())
with open(modules_file, "r", encoding="utf-8") as f:
    input_modules = ModuleFileXmlModel.from_xml(f.read())

module_production_map = {
    module.id: module for module in input_modules.modules
}
modules = [m for m in input_ware.wares if m.is_module()]
for i in modules:
    if i.macro is None:
        print(i.id)

for i in modules:
    i.translate_fields(lang)

for i in modules:
    module_xml = module_production_map.get(i.id)
    Module.from_xml_model(i, module_xml)