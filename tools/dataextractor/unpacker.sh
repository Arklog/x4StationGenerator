#!/usr/bin/env bash

# Check for correct number of arguments
if [[ $# -ne 3 ]]; then
    echo "Usage: $0 <x4_tool> <x4_root_dir> <destination_dir>"
    exit 1
fi

# Assign input arguments to variables
X4_TOOL="$1"
X4_ROOT_DIR="$2"
DEST_DIR="$3"

# Validate input directories

if [[ ! -f "$X4_TOOL" ]]; then
    echo "Error: '$X4_TOOL' is not a file."
    exit 1
fi

if [[ ! -d "$X4_ROOT_DIR" ]]; then
    echo "Error: '$X4_ROOT_DIR' is not a valid directory."
    exit 1
fi

if [[ ! -d "$DEST_DIR" ]]; then
    mkdir -p "$DEST_DIR"
fi

# Check wine installed
if ! which wine &> /dev/null; then
    echo "Error: 'wine' is not installed. Please install it and try again."
    exit 1
fi

X4_DIRS=("/" /extensions/ego_dlc_{boron,mini_01,pirate,split,terran})
X4_DIRS=("/")

function test() {
  echo $1
}

for DIR in "${X4_DIRS[@]}"; do
    FULL_DIR="${X4_ROOT_DIR}${DIR}"
    FULL_DEST_DIR="${DEST_DIR}${DIR}"

    mkdir -p "$FULL_DEST_DIR"
    echo "Processing directory: $FULL_DIR"
    echo "Destination directory: $FULL_DEST_DIR"

    OIFS="${IFS}"
    IFS=$'\n'
#    for DAT_FILE in $(find "$FULL_DIR" -maxdepth 1 -type f -regextype 'posix-extended' -regex ".*[0-9]{2}\.cat" -exec winepath -w {} \;); do
    for DAT_FILE in $(find "$FULL_DIR" -type f -regextype 'posix-extended' -regex ".*[0-9]{2}\.cat" -exec winepath -w {} \;); do
        echo "Extracting: $DAT_FILE"
        WIN_DEST_DIR="$(winepath -w "$FULL_DEST_DIR")"

        wine "$X4_TOOL" -in "$DAT_FILE" -out "$WIN_DEST_DIR" &
    done
    IFS="${OIFS}"

    wait
done