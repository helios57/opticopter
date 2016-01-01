sudo apt-get -y install build-essential cvs git subversion mercurial texinfo flex bison automake python-serial python-argparse openocd libncurses5-dev autoconf texinfo libftdi-dev libtool zlib1g-dev genromfs ia32-libs git-core wget
sudo mkdir /tmp/ramdisk; chmod 777 /tmp/ramdisk
sudo mount -t tmpfs -o size=6G tmpfs /tmp/ramdisk/
cd /tmp/ramdisk
mkdir avrToolchain
cd avrToolchain
git clone git://sourceware.org/git/binutils-gdb.git
cd binutils-gdb
mkdir obj-avr
cd obj-avr
../configure --target=avr --disable-nls
make
sudo make install
cd ..
cd ..
rm -fr *

svn checkout svn://gcc.gnu.org/svn/gcc/trunk
cd trunk
./contrib/download_prerequisites
mkdir obj-avr
cd obj-avr
../configure --target=avr --enable-languages=c,c++ --disable-nls --disable-libssp --with-dwarf2
make
sudo make install
cd ..
cd ..
rm -fr trunk


mkdir avr-libc
cd avr-libc
svn co svn://svn.savannah.nongnu.org/avr-libc/trunk
cd trunk
cd avr-libc
./bootstrap
./configure --build=`./config.guess` --host=avr
make
sudo make install
cd ..
cd ..
cd ..
rm -fr avr-libc

svn co svn://svn.savannah.nongnu.org/avrdude/trunk
cd trunk/avrdude
./bootstrap
mkdir obj-avr
cd obj-avr
../configure
make
sudo make install
cd ..
cd ..
cd ..
rm -fr trunk


git clone ssh://helios157@git.code.sf.net/u/helios157/avr-eclipse u-helios157-avr-eclipse

//Arm
add-apt-repository ppa:terry.guo/gcc-arm-embedded
apt-get update
apt-get install gcc-arm-none-eabi