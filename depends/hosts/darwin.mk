OSX_MIN_VERSSPHX=10.8
OSX_SDK_VERSSPHX=10.11
OSX_SDK=$(SDK_PATH)/MacOSX$(OSX_SDK_VERSSPHX).sdk
LD64_VERSSPHX=253.9
darwin_CC=clang -target $(host) -mmacosx-verssphx-min=$(OSX_MIN_VERSSPHX) --sysroot $(OSX_SDK) -mlinker-verssphx=$(LD64_VERSSPHX)
darwin_CXX=clang++ -target $(host) -mmacosx-verssphx-min=$(OSX_MIN_VERSSPHX) --sysroot $(OSX_SDK) -mlinker-verssphx=$(LD64_VERSSPHX) -stdlib=libc++

darwin_CFLAGS=-pipe
darwin_CXXFLAGS=$(darwin_CFLAGS)

darwin_release_CFLAGS=-O2
darwin_release_CXXFLAGS=$(darwin_release_CFLAGS)

darwin_debug_CFLAGS=-O1
darwin_debug_CXXFLAGS=$(darwin_debug_CFLAGS)

darwin_native_toolchain=native_cctools
