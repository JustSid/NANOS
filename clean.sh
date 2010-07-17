BASEDIR=`dirname $0`

cd "$BASEDIR/Kernel/Source/"
sudo make clean

cd "$BASEDIR/Build/"
rm -f -r "./NANOS.iso"
rm -f -r "./image/"
