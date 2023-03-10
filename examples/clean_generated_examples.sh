#!/bin/sh

set -xe

# rm *.png

if [ -e *.png ]; then
    rm *.png
fi

if [ -e *.ppm ]; then
    rm *.ppm
fi