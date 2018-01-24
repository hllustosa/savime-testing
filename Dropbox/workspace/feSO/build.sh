#!/bin/bash

cd Desenvolvimento/kernel
make
cd ../..
cp Desenvolvimento/kernel/bin/kernel.bin isofiles/boot
cp Desenvolvimento/kernel/bin/trampolim.o isofiles/drivers/tramp.o

cd Desenvolvimento/dev/Console
make
cd ../../..
cp Desenvolvimento/dev/Console/bin/console.o isofiles/drivers

cd Desenvolvimento/dev/Timer
make
cd ../../..
cp Desenvolvimento/dev/Timer/bin/timer.o isofiles/drivers

cd Desenvolvimento/dev/Fat
make
cd ../../..
cp Desenvolvimento/dev/Fat/bin/fat.o isofiles/drivers

cd Desenvolvimento/dev/Iso9660
make
cd ../../..
cp Desenvolvimento/dev/Iso9660/bin/iso9660.o isofiles/drivers

cd Desenvolvimento/dev/Ide
make
cd ../../..
cp Desenvolvimento/dev/Ide/bin/ide.o isofiles/drivers

cd Desenvolvimento/app/Editor
make
cd ../../..
cp Desenvolvimento/app/Editor/bin/editor.o isofiles/app

cd Desenvolvimento/app/Idle
make
cd ../../..
cp Desenvolvimento/app/Idle/bin/idle.o isofiles/app

cd Desenvolvimento/app/Shell
make
cd ../../..
cp Desenvolvimento/app/Shell/bin/shell.o isofiles/app

cd Desenvolvimento/dev/Hdd
make
cd ../../..
cp Desenvolvimento/dev/Hdd/bin/hdd.o isofiles/drivers

cd Desenvolvimento/app/Egauss
make
cd ../../..
cp Desenvolvimento/app/Egauss/bin/egauss.o isofiles/app

cd Desenvolvimento/app/Strassen
make
cd ../../..
cp Desenvolvimento/app/Strassen/bin/strassen.o isofiles/app

cd Desenvolvimento/app/Matrizes
make
cd ../../..
cp Desenvolvimento/app/Matrizes/bin/matrizes.o isofiles/app

cd Desenvolvimento/app/Tetris
make
cd ../../..
cp Desenvolvimento/app/Tetris/bin/tetris.o isofiles/app

mkisofs -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o  ISO/bootcd.iso isofiles
