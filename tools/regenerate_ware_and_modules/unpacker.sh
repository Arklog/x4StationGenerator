#!/usr/bin/env bash

if [[ ! -f .env ]]; then
  # Check for correct number of arguments
  if [[ $# -ne 4 ]]; then
      echo "Usage: $0 <x4_tool> <x4_root_dir> <destination_dir> <xml_patch_executable>"
      exit 1
  fi

  # Assign input arguments to variables
  X4_TOOL="$1"
  X4_ROOT_DIR="$2"
  DEST_DIR="$3"
  XML_PATCH_EXEC="$4"
else
  source .env
fi

# Validate input

if [[ ! -f "$X4_TOOL" ]]; then
    echo "Error: '$X4_TOOL' is not a file."
    exit 1
fi

#if [[ ! -d "$X4_ROOT_DIR" ]]; then
#    echo "Error: '$X4_ROOT_DIR' is not a valid directory."
#    exit 1
#fi

if [[ ! -f "$XML_PATCH_EXEC" ]]; then
    echo "Error: '$XML_PATCH_EXEC' is not a file."
    exit 1
fi

# Check wine installed
if ! which wine &> /dev/null; then
    echo "Error: 'wine' is not installed. Please install it and try again."
    exit 1
fi

X4_DLC_DIRS=(/extensions/ego_dlc_{boron,mini_01,mini_02,pirate,split,terran,timelines})

# Extracting Data from X4: Foundations
if [[ ! -d "$DEST_DIR" ]]; then
    mkdir -p "$DEST_DIR"
    if [[ $? -ne 0 ]]; then
        echo "Error: Could not create destination directory '$DEST_DIR'."
        exit 1
    fi

    X4_DIRS=("/" ${X4_DLC_DIRS[@]})

    echo "Extracting data from X4: Foundations directories to $DEST_DIR"

    for DIR in "${X4_DIRS[@]}"; do
        FULL_DIR="${X4_ROOT_DIR}${DIR}"
        FULL_DEST_DIR="${DEST_DIR}${DIR}"

        mkdir -p "$FULL_DEST_DIR"
        echo "Processing directory: $FULL_DIR"
        echo "Destination directory: $FULL_DEST_DIR"

        OIFS="${IFS}"
        IFS=$'\n'
        for DAT_FILE in $(find "$FULL_DIR" -maxdepth 1 -type f -regextype 'posix-extended' -regex ".*[0-9]{2}\.cat" -exec winepath -w {} \;); do
            echo "Extracting: $DAT_FILE"
            WIN_DEST_DIR="$(winepath -w "$FULL_DEST_DIR")"

            wine "$X4_TOOL" -in "$DAT_FILE" -out "$WIN_DEST_DIR" &
        done
        IFS="${OIFS}"

        wait
    done
fi

# Patching XML Diffs
FILES_TO_PATCH=(libraries/{wares.xml,macro.xml,modules.xml,modulegroups.xml,waregroups.xml,factions.xml})
DIR_CONTENT_TO_COPY=(assets/structures/habitat/macros)

for DLC_DIR in ${X4_DLC_DIRS[@]}; do
  DIR_TO_PATCH="${DEST_DIR}${DLC_DIR}"

  # Patching xml files
  for FILE in ${FILES_TO_PATCH[@]}; do
    FULL_PATH_TO_ADD="${DIR_TO_PATCH}/${FILE}"
    FILE_TO_PATCH="${DEST_DIR}/${FILE}"

    [[ ! -f "${FULL_PATH_TO_ADD}" ]] && (echo "File to add not found: ${FULL_PATH_TO_ADD}"; exit 1)
    [[ ! -f "${FILE_TO_PATCH}" ]] && (echo "File to patch not found: ${FILE_TO_PATCH}" && exit 1)

    echo "Patching ${FILE_TO_PATCH} with ${FULL_PATH_TO_ADD}"
    FULL_PATH_TO_ADD_WIN="$(winepath -w "${FULL_PATH_TO_ADD}")"
    FILE_TO_PATCH_WIN="$(winepath -w "${FILE_TO_PATCH}")"

    wine "$XML_PATCH_EXEC" -o "${FILE_TO_PATCH_WIN}" -d "${FULL_PATH_TO_ADD_WIN}" -u "${FILE_TO_PATCH_WIN}" 1>/dev/null 2>&1 &
  done

  # Copying other files
  for DIR_TO_COPY in ${DIR_CONTENT_TO_COPY[@]}; do
    find "${DIR_TO_PATCH}/${DIR_TO_COPY}" -type f -name "*.xml" -exec cp {} "${DEST_DIR}/${DIR_TO_COPY}" \; &
  done

  wait

  # modulegroups.xml special case
  # need to append the content
  MODULEGROUPS_BASE="${DEST_DIR}/libraries/modulegroups.xml"
  MODULEGROUPS_DLC="${DIR_TO_PATCH}/libraries/modulegroups.xml"

  if [[ -f "${MODULEGROUPS_DLC}" ]]; then
    MODULEGROUPS_DCL_SIZE="$(wc -l <${MODULEGROUPS_DLC})"
    MODULEGROUPS_DCL_SIZE=$((MODULEGROUPS_DCL_SIZE - 2))

    head "${MODULEGROUPS_BASE}" -n -1 >/tmp/modulegroups.xml
    tail -n "-${MODULEGROUPS_DCL_SIZE}" "${MODULEGROUPS_DLC}" >>/tmp/modulegroups.xml
    mv /tmp/modulegroups.xml "${MODULEGROUPS_BASE}"
  fi

  echo "Patched files in ${DIR_TO_PATCH}"
done