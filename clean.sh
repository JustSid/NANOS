BASEDIR=`dirname $0`

cd "$BASEDIR/Kernel/"
sudo make clean

cd "$BASEDIR/IDE/"
rm -f -r "./build/"

cd "$BASEDIR/Build/"
rm -f -r "./NANOS.iso"
rm -f -r "./image/"
