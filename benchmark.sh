#!/bin/bash

## SETTINGS
# Program pathname (absolute or relative path)
PROGNAME="./ha"
# Program extra arguments (all the arguments except the input and output files)
PROGARG="--block-size=10MB --memlimit=1GB --thread=4"
# Text file (the test is skip if it is empty)
TEXTFILE="Makefile"
# Audio file (the test is skip if it is empty)
AUDIOFILE=""
# Video file (the test is skip if it is empty)
VIDEOFILE=""
# Binary file (the test is skip if it is empty)
BINARYFILE="/usr/bin/ls"
# Define the number of test per file (default: 10)
NBTEST=10
# Define the temporary output file name (default: .tmp.bench)
TMPFILE=".tmp.bench"
## END SETTINGS

# Color
BLACK="\033[0;30m"
RED="\033[1;31m"
GREEN="\033[0;32m"
YELLOW="\033[0;33m"
BLUE="\033[1;34m"
PINK="\033[0;35m"
LIGTH_BLUE="\033[0;36m"
WHITE="\033[1;37m"
DEFAULT="\033[0m"

# Function to measuring time between 2 points in the program execution
start_measuring_time() {
  read s1 s2 < <(date +'%s %N')
}

stop_measuring_time() {
  read e1 e2 < <(date +'%s %N')
}

bytes_to_hr_form() {
    if [ $1 -ge 1073741824 ]; then # 1GB
        byte="$(bc <<< "scale = 1; $1 / 1073741824") GB"
    elif [ $1 -ge 1048576 ]; then # 1MB
        byte="$(bc <<< "scale = 1; $1 / 1048576") MB"
    elif [ $1 -ge 1024 ]; then # 1kB
        byte="$(bc <<< "scale = 1; $1 / 1024") kB"
    else
        byte="$1 B"
    fi
}

test_file() {
    echo
    if [ -z "$1" ]; then
        echo -e "${YELLOW}The $2 file is not set: skip test${DEFAULT}"
        return
    elif [ ! -r "$1" ]; then
        echo -e "${RED}The $2 file is not readable: skip test${DEFAULT}"
        return
    fi

    echo "Testing $2 file"

    unlink "${TMPFILE}" 2> /dev/null
    total=0 # in ms
    outfile="${TMPFILE}"
    for i in $(seq ${NBTEST})
    do
        echo -en "\rTest number $i / ${NBTEST}"
        start_measuring_time
        ${PROGNAME} ${PROGARG} < "$1" > "${outfile}"
        ret=$?
        stop_measuring_time

        if [ "$ret" -ne 0 ]; then
            echo -e "\n${RED}${PROGNAME} has return the exit code ${ret}: skip the rest of the test${DEFAULT}"
            return
        fi

        t=$(expr \( ${e1} - ${s1} \) \* 1000 + \( ${e2} - ${s2} \) / 1000000)
        total=$(expr ${total} + ${t})
        outfile="/dev/null" # Just to avoid the slowness of io operation
    done

    compressedsize=$(stat -L --printf="%s" "${TMPFILE}")
    unlink "${TMPFILE}" 2> /dev/null
    bytes_to_hr_form ${compressedsize}
    compressedbyte="${byte}"

    filesize=$(stat -L --printf="%s" "$1")
    bytes_to_hr_form ${filesize}
    filesizebyte="${byte}"

    ratio=$(bc <<< "scale = 3; (${compressedsize} * 100) / ${filesize}")
    bytepersec=$(bc <<< "scale = 1; ${filesize} / (${total} / 1000)" | cut -d . -f 1)
    bytes_to_hr_form ${bytepersec}
    bytepersecbyte="${byte}"

    echo
    echo
    echo -e "${WHITE}Result for $2 file: '$1'${DEFAULT}"
    echo -e "${WHITE}Total time: ${total} ms${DEFAULT}"
    echo -e "${WHITE}Average time: $(expr ${total} / ${NBTEST}) ms${DEFAULT}"
    echo -e "${WHITE}File size: ${filesize} B (${filesizebyte})${DEFAULT}"
    echo -e "${WHITE}Compressed file size: ${compressedsize} (${compressedbyte})${DEFAULT}"
    echo -e "${WHITE}Compression ratio: ${ratio} %${DEFAULT}"
    echo -e "${WHITE}Compression speed: ${bytepersec} B / sec (${bytepersecbyte} / sec)${DEFAULT}"
}

echo -en "${RED}"
tmp=$(which "${PROGNAME}")
ret=$?
echo -en "${DEFAULT}"
if [ ${ret} -ne 0 ]; then
    exit 1
fi
PROGNAME=${tmp}

echo -n "Testing program '${PROGNAME}'"
if [ ! -z "${PROGARG}" ]; then
    echo -n " with arguments '${PROGARG}'"
fi
echo

test_file "${TEXTFILE}" "text"
test_file "${AUDIOFILE}" "audio"
test_file "${VIDEOFILE}" "video"
test_file "${BINARYFILE}" "binary"
