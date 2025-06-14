# Ton répertoire racine « sgoinfre »
export BASE=/home/rodolfo/Documents/Campus19/PostCommon/RNCP6/ApplicativeDevelopment/Suite/kfs

# Dossiers que l'on va créer
export SRC=$BASE/sources          # archives + répertoires sources
export PREFIX=$BASE/opt/cross     # où l'on installera binutils & gcc
export TARGET=i686-elf
export JOBS=$(nproc)
mkdir -p "$SRC" "$PREFIX"



cd "$SRC"
wget https://ftp.gnu.org/gnu/binutils/binutils-2.44.tar.xz
tar -xf binutils-2.44.tar.xz
mkdir -p build-binutils && cd build-binutils

../binutils-2.44/configure \
  --target=$TARGET \
  --prefix=$PREFIX \
  --disable-nls   \
  --disable-werror

make -j$JOBS
make install


cd "$SRC"
wget https://ftp.gnu.org/gnu/gcc/gcc-15.1.0/gcc-15.1.0.tar.xz
tar -xf gcc-15.1.0.tar.xz
cd gcc-15.1.0
./contrib/download_prerequisites        # télécharge GMP, MPFR, MPC, ISL et crée les liens


mkdir -p "$SRC/build-gcc" && cd "$SRC/build-gcc"

../gcc-15.1.0/configure \
  --target=$TARGET \
  --prefix=$PREFIX \
  --disable-nls \
  --enable-languages=c \
  --without-headers

make all-gcc           -j$JOBS
make all-target-libgcc -j$JOBS
make install-gcc
make install-target-libgcc

export PATH="$BASE/opt/cross/bin:$PATH"

# source ~/.zshrc