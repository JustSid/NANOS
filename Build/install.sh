BASEDIR=`dirname $0`

sudo rm -f -r "/boot/nanos/"
sudo mkdir -p "/boot/nanos/"

sudo cp "$BASEDIR/../Kernel/NANOS Core/core" "/boot/nanos/core"