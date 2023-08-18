#!/bin/bash
# based on the instructions from edk2-platform
set -e
. build_common.sh
# not actually GCC5; it's GCC7 on Ubuntu 18.04.
GCC5_AARCH64_PREFIX=aarch64-linux-gnu- build -s -n 0 -a AARCH64 -t GCC5 -p MSM8916Pkg/Devices/j5lte.dsc
cd BootShim
make UEFI_BASE=0x830000000 UEFI_SIZE=0x00200000
rm BootShim.elf
cd ..
rm -rf UEFI
cat BootShim/BootShim.bin workspace/Build/MSM8916Pkg/DEBUG_GCC5/FV/MSM8916PKG_UEFI.fd > UEFI
# gzip -c < workspace/Build/MSM8916Pkg/DEBUG_GCC5/FV/MSM8916PKG_UEFI.fd >uefi.img
