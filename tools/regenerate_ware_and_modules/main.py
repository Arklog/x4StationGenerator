import argparse
import os

parser = argparse.ArgumentParser()
parser.add_argument("--ware-an-modules-file", help="XML file to parse", type=str, required=True)
parser.add_argument("--lang-file", help="XML language file to parse", type=str, required=True)
parser.add_argument("--output", help="Output directory", type=str, required=True)

args = parser.parse_args()

# check output is a folder
out_dir: str = args.output
if not os.path.exists(out_dir):
    os.makedirs(args.output, exist_ok=True)
elif not os.path.isdir(out_dir):
    raise TypeError("Output directory must be a directory")

# check input file exists
in_file: str = args.xml_file
if not os.path.isfile(in_file):
    raise TypeError("Input file must be a file")

