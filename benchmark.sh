#!/bin/bash

start_measuring_time() {
  read s1 s2 < <(date +'%s %N')
}

stop_measuring_time() {
  read e1 e2 < <(date +'%s %N')
}

if [ $# -lt 1 ]; then
    echo "Usage: $0 file_to_compress"
    exit 1
elif [ ! -f "$1" ]; then
    echo "Unable to read the file '$1'"
    exit 1
fi

prog="./ha"
numberOfTest=10
thread=3
memlimit="1GB"
inputfile="$1"

echo "Testing program '${prog}'"

for block_size in 10kB 100kB 1MB 10MB 20MB 50MB 100MB
do
    echo "Testing with block size of ${block_size}"

    total=0
    for i in $(seq ${numberOfTest})
    do
        echo -en "\rTest number $i / ${numberOfTest}"
        start_measuring_time
        $("${prog}" "${inputfile}" --block-size="${block_size}" --memlimit=${memlimit} --thread=${thread} -o /dev/null) > /dev/null 2>&1
        stop_measuring_time

        total=$(expr ${total} + \( \( ${e1} - ${s1} \) \* 1000 + \( ${e2} - ${s2} \) / 1000000 \))
    done

    echo
    echo "Total time: ${total} ms"
    echo "Average time: $(expr ${total} / ${numberOfTest}) ms"
    echo
done
