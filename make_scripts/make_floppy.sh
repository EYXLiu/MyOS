#!/bin/bash

TARGET="$1"
STAGE1_STAGE2_LOCATION_OFFSET=480

# generate image
dd if=/dev/zero of="$TARGET" bs=512 count=2880 >/dev/null

# determine number of reserved sections
STAGE2_SIZE=$(stat -f "%z" -- "${BUILD_DIR}/stage2.bin")
STAGE2_SECTORS=$(( (${STAGE2_SIZE} + 511) / 512 ))
RESERVED_SECTORS=$(( 1 + $STAGE2_SECTORS ))

# create file system
mkfs.fat -F 12 -R $RESERVED_SECTORS -n "NBOS" $TARGET

# install bootloader
dd if=${BUILD_DIR}/stage1.bin of=$TARGET conv=notrunc bs=1 count=3 >/dev/null
dd if=${BUILD_DIR}/stage1.bin of=$TARGET conv=notrunc bs=1 seek=62 skip=62 >/dev/null
dd if=${BUILD_DIR}/stage2.bin of=$TARGET conv=notrunc bs=512 seek=1 >/dev/null

# write lba of stage2 to bootloader
echo "01 00 00 00" | xxd -r -p | dd of=$TARGET conv=notrunc bs=1 seek=$STAGE1_STAGE2_LOCATION_OFFSET
printf "%x" ${STAGE2_SECTORS} | xxd -r -p | dd of=$TARGET conv=notrunc bs=1 seek=$(( $STAGE1_STAGE2_LOCATION_OFFSET + 4 ))

# files
mcopy -i "$TARGET" "${BUILD_DIR}/kernel.bin" "::kernel.bin"
mcopy -i "$TARGET" text.txt "::text.txt"
mmd -i "$TARGET" "::dir"
