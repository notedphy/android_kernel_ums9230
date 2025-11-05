#!/bin/bash
# Build script for overclocked kernel

export ARCH=arm64
export CROSS_COMPILE=aarch64-linux-gnu-

# Enable overclock config
echo "CONFIG_SPRD_UMS9230_OVERCLOCK=y" >> .config

# Build kernel
make -j$(nproc) 2>&1 | tee build.log

if [ $? -eq 0 ]; then
    echo "[SUCCESS] Kernel compiled successfully!"
    echo "Output: arch/arm64/boot/Image.gz"
else
    echo "[ERROR] Kernel compilation failed!"
    exit 1
fi
