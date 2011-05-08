BASEDIR=`dirname $0`

cd "$BASEDIR/Kernel/"
sudo make clean

cd "$BASEDIR/Libraries/libkernel"
sudo make clean

cd "$BASEDIR/Programs/Test"
sudo make clean

cd "$BASEDIR/Programs/Shell/"
sudo make clean

cd "$BASEDIR/Build/"
rm -f -r "./NANOS.iso"
rm -f -r "./image/"
