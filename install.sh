#!/bin/bash

echo
echo "Elimino file di log e creo le directory necessarie"

rm -r log
mkdir log

rm -r obj
mkdir obj

rm -r bin
mkdir bin

cd data
if [ -e speed.data ]
then 
	rm speed.data
	echo
else
	echo
fi

echo "Eseguo compilazione del makefile"
echo

cd ../

make
