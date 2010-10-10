BASEDIR=`dirname $0`

"$BASEDIR/build.sh"

cd "$BASEDIR/Build/"
./createImage.sh
