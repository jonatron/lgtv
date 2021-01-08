#!/bin/bash
PATH=/home/jon/projects/toolchain/bin:$PATH armv7a-mediatek451_001_vfp-linux-gnueabi-gcc handle_input.c -o handle_input
PATH=/home/jon/projects/toolchain/bin:$PATH armv7a-mediatek451_001_vfp-linux-gnueabi-gcc read_mtal.c -o read_mtal -ldl

