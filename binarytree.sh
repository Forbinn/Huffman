#!/bin/bash

ext="png"

if [ $# -lt 1 ] ; then
  echo "Usage : $0 file.dot"
  exit 1
fi

dot "$1" | gvpr -c -ftree.gv | neato -n -T$ext -o binarytree.$ext
