#!/bin/bash
# create multiresolution windows icon
#mainnet
ICON_SRC=../../src/qt/res/icons/sphx.png
ICON_DST=../../src/qt/res/icons/sphx.ico
convert ${ICON_SRC} -resize 16x16 sphx-16.png
convert ${ICON_SRC} -resize 32x32 sphx-32.png
convert ${ICON_SRC} -resize 48x48 sphx-48.png
convert sphx-16.png sphx-32.png sphx-48.png ${ICON_DST}
#testnet
ICON_SRC=../../src/qt/res/icons/sphx_testnet.png
ICON_DST=../../src/qt/res/icons/sphx_testnet.ico
convert ${ICON_SRC} -resize 16x16 sphx-16.png
convert ${ICON_SRC} -resize 32x32 sphx-32.png
convert ${ICON_SRC} -resize 48x48 sphx-48.png
convert sphx-16.png sphx-32.png sphx-48.png ${ICON_DST}
rm sphx-16.png sphx-32.png sphx-48.png
#regtest
ICON_SRC=../../src/qt/res/icons/sphx_regtest.png
ICON_DST=../../src/qt/res/icons/sphx_regtest.ico
convert ${ICON_SRC} -resize 16x16 sphx-16.png
convert ${ICON_SRC} -resize 32x32 sphx-32.png
convert ${ICON_SRC} -resize 48x48 sphx-48.png
convert sphx-16.png sphx-32.png sphx-48.png ${ICON_DST}
rm sphx-16.png sphx-32.png sphx-48.png
#unittest
ICON_SRC=../../src/qt/res/icons/sphx_unittest.png
ICON_DST=../../src/qt/res/icons/sphx_unittest.ico
convert ${ICON_SRC} -resize 16x16 sphx-16.png
convert ${ICON_SRC} -resize 32x32 sphx-32.png
convert ${ICON_SRC} -resize 48x48 sphx-48.png
convert sphx-16.png sphx-32.png sphx-48.png ${ICON_DST}
rm sphx-16.png sphx-32.png sphx-48.png