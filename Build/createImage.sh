BASEDIR=`dirname $0`
rm -f -r "$BASEDIR/NANOS.iso"
rm -f -r "$BASEDIR/image"

mkdir -p "$BASEDIR/image/boot/grub"
mkdir -p "$BASEDIR/image/nanos"
mkdir -p "$BASEDIR/image/modules"

cp "$BASEDIR/grub.cfg" "$BASEDIR/image/boot/grub"
cp "$BASEDIR/../Kernel/nanos" "$BASEDIR/image/nanos/nanos"
cp "$BASEDIR/../Programs/test.bin" "$BASEDIR/image/modules/test.bin"

cd "$BASEDIR/"
grub-mkrescue --modules="linux ext2 fshelp ls boot pc" --output=NANOS.iso "$BASEDIR/image"

rm -f -r "$BASEDIR/image"
