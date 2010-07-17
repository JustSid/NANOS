BASEDIR=`dirname $0`
rm -f -r "$BASEDIR/NANOS.iso"
rm -f -r "$BASEDIR/image"

mkdir -p "$BASEDIR/image/boot/grub"
mkdir -p "$BASEDIR/image/nanos"

cp "$BASEDIR/grub.cfg" "$BASEDIR/image/boot/grub"
cp "$BASEDIR/../Kernel/Source/kernel" "$BASEDIR/image/nanos/kernel"

cd "$BASEDIR/"
grub-mkrescue --modules="linux ext2 fshelp ls boot pc" --output=NANOS.iso "$BASEDIR/image"

rm -f -r "$BASEDIR/image"
