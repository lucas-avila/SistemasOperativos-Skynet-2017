#!usr/bin/sh

clear

cd Sharedlib
cd Debug
make clean
make all
cd ..
cd ..

cd SistemaConsola
cd Debug
make clean
make all
cd ..
cd ..

cd SistemaCPU
cd Debug
make clean
make all
cd ..
cd ..

cd SistemaFILESYSTEM
cd Debug
make clean
make all
cd ..
cd ..

cd SistemaKERNEL
cd Debug
make clean
make all
cd ..
cd ..

cd SistemaMEMORIA
cd Debug
make clean
make all
cd ..
cd ..

echo ""
echo "Listo."
