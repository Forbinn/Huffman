#!/bin/bash

ext="png"

if [ $# -lt 2 ] ; then
  echo "Usage : $0 file.dot image.$ext"
  exit 1
fi

dot "$1" | gvpr -c -ftree.gv | neato -n -T$ext -o "$2"
