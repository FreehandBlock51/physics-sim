#!/bin/bash
# taken and modified from https://github.com/Zaya6/build_shaders

SOURCE="${1:-shaders}" # source directory to find shaders
DESTINATION="${2:-include/shaders}" # Output directory
MAKE_DESTINATION="${3:-true}"
USE_COLOR="${4:-checkterm}"
if [ "$USE_COLOR" = "checkterm" ]; then
    if [ "$COLORTERM" ]; then
        USE_COLOR="true";
    else
        USE_COLOR="false";
    fi
fi

if $USE_COLOR; then
    CYAN="\033[1;36m"
    YELLOW="\033[0;33m"
    PURPLE='\033[0;35m'
    RED='\033[0;31m'
    NC='\033[0m'
fi

# check if source exists
if [ ! -d "$SOURCE" ]; then
  echo -e "${RED}ERROR:$NC Source directory $SOURCE does not exist."
  exit 1
fi

# check if destination exists
if [ ! -d "$DESTINATION" ]; then
    # if MAKE_DESTINATION is true, make the directory if it doesn not exist
    if $MAKE_DESTINATION; then
        echo -e "${YELLOW}WARNING:$NC Output directory does not exist. Creating it..."
        mkdir -p "$DESTINATION"
    # otherwise, fail with error
    else
        echo -e "${RED}ERROR:$NC Output directory does not exist."
        exit 1
    fi
fi

# loop over every file in source directory
for shaderFile in $SOURCE/*; do
    if [ -f "$shaderFile" ]; then

        name=$(echo ${shaderFile#$SOURCE/} | tr ./ __)
        define=$(echo "SHADER_$name" | tr [:lower:] [:upper:])
        newFile=$(echo "$DESTINATION/$name.h")

        # report what's happening
        echo -e "Processing $PURPLE$define$NC in $YELLOW$shaderFile$NC; Outputing to >> $CYAN$newFile$NC"

        # add defines, overwrites old file
        echo -e "#ifndef $define\n#define $define" > "$newFile"

        # add variable name
        echo -e "\nstatic const char* shader_src_$name = \"\"" >> "$newFile"

        # loop through lines of shader text, wraping lines in " and \n"
        while read -r p; do
            echo "\"$p\n\"" >> "$newFile"
        done <$shaderFile

        # add ""; to the end, >> automatically adds newline at end
        echo -e "\"\";\n#endif" >> "$newFile"

    fi
done
