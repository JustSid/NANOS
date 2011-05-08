BASEDIR=`dirname $0`

cd "$BASEDIR/Kernel/"
sudo make

cd "$BASEDIR/Libraries/libkernel/"
sudo make

cd "$BASEDIR/Programs/Test/"
sudo make

cd "$BASEDIR/Programs/Shell/"
sudo make
