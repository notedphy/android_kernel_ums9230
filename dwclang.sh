cd clang
wget https://android.googlesource.com/platform/prebuilts/clang/host/linux-x86/+archive/refs/tags/android->
tar xf clang-r416183b.tar.gz && cd ..
git clone https://github.com/cooked71/arm-linux-androideabi-4.9-12-5
armm=$(pwd)/arm-linux-androideabi-4.9-12-5/bin/arm-linux-androideabi-
clang=$(pwd)/clang/bin
export PATH="$(pwd)/clang/bin:$(pwd)/aarch64-linux-android-4.9-12-5/bin:${PATH}"
