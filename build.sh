armm=$(pwd)/arm-linux-androideabi-4.9-12-5/bin/arm-linux-androideabi-
clang=$(pwd)/clang/bin
export PATH="$(pwd)/clang/bin:$(pwd)/aarch64-linux-android-4.9-12-5/bin:${PATH}"

echo "vm.swappiness=100" | sudo tee -a /etc/sysctl.conf

make mikasa_defconfig O=out \
            ARCH=arm64 \
            LLVM=1 \
            CC=clang\
            AR=$clang/llvm-ar \
            AS=$clang/llvm-as \
            NM=$clang/llvm-nm \
            OBJCOPY=$clang/llvm-objcopy \
            OBJDUMP=$clang/llvm-objdump \
            STRIP=$clang/llvm-strip \
            CLANG_TRIPLE=aarch64-linux-gnu- \
            CROSS_COMPILE_COMPAT=arm-linux-androideabi- \
            CROSS_COMPILE=aarch64-linux-android-


make O=out \
            ARCH=arm64 \
            LLVM=1 \
            CC=clang\
            AR=$clang/llvm-ar \
            AS=$clang/llvm-as \
            NM=$clang/llvm-nm \
            OBJCOPY=$clang/llvm-objcopy \
            OBJDUMP=$clang/llvm-objdump \
            STRIP=$clang/llvm-strip \
            CLANG_TRIPLE=aarch64-linux-gnu- \
            CROSS_COMPILE_COMPAT="$armm" \
            CROSS_COMPILE=aarch64-linux-android-




