#!/bin/zsh

# 1. Update package-data/CHANGELOG
# 2. Update version variables below
# 3. Run ./package.sh

PACKAGE_VERSION_SOURCE=20220318-1
PACKAGE_VERSION_SOURCE_CONTRIB=20220318-1

PACKAGE_VERSION_PC=$PACKAGE_VERSION_SOURCE
PACKAGE_VERSION_LINUX=$PACKAGE_VERSION_SOURCE
PACKAGE_VERSION_MINGW=$PACKAGE_VERSION_SOURCE
PACKAGE_VERSION_CONTRIB=$PACKAGE_VERSION_SOURCE_CONTRIB
PACKAGE_CPU=x86_64
PACKAGE_BRANCH=core

ROOT=..
PACKAGES_BASE=./packages
SOURCE_AROS=$ROOT/AROS

NAME_PC=AROS-$PACKAGE_VERSION_PC-pc-$PACKAGE_CPU-boot-iso
PACKAGE_PC=$NAME_PC
ARCHIVE_PC=$NAME_PC.zip

NAME_LINUX=AROS-$PACKAGE_VERSION_LINUX-linux-$PACKAGE_CPU-system
PACKAGE_LINUX=$NAME_LINUX
ARCHIVE_LINUX=$NAME_LINUX.tar.bz2

NAME_MINGW=AROS-$PACKAGE_VERSION_MINGW-mingw32-$PACKAGE_CPU-system
PACKAGE_MINGW=$NAME_MINGW
ARCHIVE_MINGW=$NAME_MINGW.tar.bz2

NAME_CONTRIB=AROS-$PACKAGE_VERSION_CONTRIB-any-$PACKAGE_CPU-contrib
PACKAGE_CONTRIB=$NAME_CONTRIB
ARCHIVE_CONTRIB=$NAME_CONTRIB.tar.bz2

NAME_SOURCE=AROS-$PACKAGE_VERSION_SOURCE-source
PACKAGE_SOURCE=$NAME_SOURCE
ARCHIVE_SOURCE=$NAME_SOURCE.tar.bz2

NAME_SOURCE_CONTRIB=AROS-$PACKAGE_VERSION_SOURCE_CONTRIB-contrib-source
PACKAGE_SOURCE_CONTRIB=$NAME_SOURCE_CONTRIB
ARCHIVE_SOURCE_CONTRIB=$NAME_SOURCE_CONTRIB.tar.bz2


alias makedir='execute mkdir -p'
alias  delete='execute rm -rf'
alias    copy='execute cp -pRL'
alias    move='execute mv -f'

source $(pwd)/AROS/scripts/nightly/functions

delete  $PACKAGES_BASE
makedir $PACKAGES_BASE
cd      $PACKAGES_BASE

# --- Binaries ---

# PC ISO
if [[ -f $ROOT/$PACKAGE_BRANCH-pc-$PACKAGE_CPU/distfiles/aros-pc-$PACKAGE_CPU.iso ]]
then
makedir $PACKAGE_PC
copy    $ROOT/$PACKAGE_BRANCH-pc-$PACKAGE_CPU/distfiles/aros-pc-$PACKAGE_CPU.iso $PACKAGE_PC/
copy    $ROOT/package-data/README $PACKAGE_PC
copy    $ROOT/package-data/CHANGELOG $PACKAGE_PC
copy    $SOURCE_AROS/LICENSE $PACKAGE_PC
copy    $SOURCE_AROS/ACKNOWLEDGEMENTS $PACKAGE_PC
sed     -i "s/XXXXXXXX-X/$PACKAGE_VERSION_PC/g" $PACKAGE_PC/README
ziparc  $ARCHIVE_PC $PACKAGE_PC
fi

# Linux System
if [[ -d $ROOT/$PACKAGE_BRANCH-linux-$PACKAGE_CPU/bin/linux-$PACKAGE_CPU/AROS ]]
then
makedir $PACKAGE_LINUX
copy    $ROOT/$PACKAGE_BRANCH-linux-$PACKAGE_CPU/bin/linux-$PACKAGE_CPU/AROS/* $PACKAGE_LINUX/
copy    $ROOT/package-data/README $PACKAGE_LINUX
copy    $ROOT/package-data/CHANGELOG $PACKAGE_LINUX
copy    $SOURCE_AROS/LICENSE $PACKAGE_LINUX
copy    $SOURCE_AROS/ACKNOWLEDGEMENTS $PACKAGE_LINUX
sed     -i "s/XXXXXXXX-X/$PACKAGE_VERSION_LINUX/g" $PACKAGE_LINUX/README
archive $ARCHIVE_LINUX $PACKAGE_LINUX
fi

# Mingw32 System
if [[ -d $ROOT/$PACKAGE_BRANCH-mingw32-$PACKAGE_CPU/bin/mingw32-$PACKAGE_CPU/AROS ]]
then
makedir $PACKAGE_MINGW
copy    $ROOT/$PACKAGE_BRANCH-mingw32-$PACKAGE_CPU/bin/mingw32-$PACKAGE_CPU/AROS/* $PACKAGE_MINGW/
copy    $ROOT/package-data/README $PACKAGE_MINGW
copy    $ROOT/package-data/CHANGELOG $PACKAGE_MINGW
copy    $SOURCE_AROS/LICENSE $PACKAGE_MINGW
copy    $SOURCE_AROS/ACKNOWLEDGEMENTS $PACKAGE_MINGW
sed     -i "s/XXXXXXXX-X/$PACKAGE_VERSION_MINGW/g" $PACKAGE_MINGW/README
archive $ARCHIVE_MINGW $PACKAGE_MINGW
fi

# Contrib
if [[ -d $ROOT/$PACKAGE_BRANCH-contrib-$PACKAGE_CPU/bin/linux-$PACKAGE_CPU/AROS ]]
then
makedir $PACKAGE_CONTRIB
copy    $ROOT/$PACKAGE_BRANCH-contrib-$PACKAGE_CPU/bin/linux-$PACKAGE_CPU/AROS/* $PACKAGE_CONTRIB/
copy    $ROOT/package-data/README $PACKAGE_CONTRIB
copy    $ROOT/package-data/CHANGELOG-CONTRIB $PACKAGE_CONTRIB/CHANGELOG
copy    $SOURCE_AROS/LICENSE $PACKAGE_CONTRIB
copy    $SOURCE_AROS/ACKNOWLEDGEMENTS $PACKAGE_CONTRIB
sed     -i "s/XXXXXXXX-X/$PACKAGE_VERSION_CONTRIB/g" $PACKAGE_CONTRIB/README
archive $ARCHIVE_CONTRIB $PACKAGE_CONTRIB
fi

# --- Sources ---

# Source
makedir $PACKAGE_SOURCE
copy    $ROOT/AROS/* $PACKAGE_SOURCE/
delete  $PACKAGE_SOURCE/contrib
delete  $PACKAGE_SOURCE/.git
copy    $ROOT/package-data/README $PACKAGE_SOURCE
copy    $ROOT/package-data/CHANGELOG $PACKAGE_SOURCE
copy    $SOURCE_AROS/LICENSE $PACKAGE_SOURCE
copy    $SOURCE_AROS/ACKNOWLEDGEMENTS $PACKAGE_SOURCE
sed     -i "s/XXXXXXXX-X/$PACKAGE_VERSION_SOURCE/g" $PACKAGE_SOURCE/README
####archive $ARCHIVE_SOURCE $PACKAGE_SOURCE

# Contrib Source
makedir $PACKAGE_SOURCE_CONTRIB
copy    $ROOT/AROS/contrib/* $PACKAGE_SOURCE_CONTRIB/
delete  $PACKAGE_SOURCE_CONTRIB/.git
copy    $ROOT/package-data/README $PACKAGE_SOURCE_CONTRIB
copy    $ROOT/package-data/CHANGELOG-CONTRIB $PACKAGE_SOURCE_CONTRIB/CHANGELOG
copy    $SOURCE_AROS/LICENSE $PACKAGE_SOURCE_CONTRIB
copy    $SOURCE_AROS/ACKNOWLEDGEMENTS $PACKAGE_SOURCE_CONTRIB
sed     -i "s/XXXXXXXX-X/$PACKAGE_VERSION_SOURCE_CONTRIB/g" $PACKAGE_SOURCE_CONTRIB/README
archive $ARCHIVE_SOURCE_CONTRIB $PACKAGE_SOURCE_CONTRIB

