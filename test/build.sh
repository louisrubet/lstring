#!/bin/bash
rm -rf build && mkdir build && cmake -B build . && make -C build -j
