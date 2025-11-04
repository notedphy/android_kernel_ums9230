currentdir=/tmp
cd $currentdir
mkdir -p clang
cd clang
wget https://android.googlesource.com/platform/prebuilts/clang/host/linux-x86/+archive/refs/tags/android-12.0.0_r12/clang-r416183b.tar.gz
tar xf clang-r416183b.tar.gz
cd $currentdir

git clone https://github.com/cooked71/arm-linux-androideabi-4.9-12-5
armm=$(pwd)/arm-linux-androideabi-4.9-12-5/bin/arm-linux-androideabi-
clang=$(pwd)/clang/bin
export PATH="$(pwd)/clang/bin:$(pwd)/aarch64-linux-android-4.9-12-5/bin:${PATH}"

sudo fallocate -l 30G $currentdir/swapfile
sudo dd if=/dev/zero of=$currentdir/swapfile bs=1M count=30720
sudo chmod 600 $currentdir/swapfile
sudo mkswap $currentdir/swapfile
sudo swapon $currentdir/swapfile
echo "vm.swappiness=100" | sudo tee -a /etc/sysctl.conf

make unisoc_defconfig O=out \
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