#!/bin/sh

set -xe

# rm *.png

if ls *.png 1> /dev/null 2>&1; then
    rm *.png
fi

if ls *.ppm 1> /dev/null 2>&1; then
    rm *.ppm
fi