BASEDIR=`dirname $0`

cd "$BASEDIR/Kernel/"
sudo make

cd "$BASEDIR/Programs/"
sudo make
