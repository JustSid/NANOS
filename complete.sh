BASEDIR=`dirname $0`

cd "$BASEDIR/Kernel/Source/"
sudo make

cd "$BASEDIR/Build/"
./createImage.sh
