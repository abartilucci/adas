#!/bin/bash

i=0
bpath="./bin"
normal="NORMALE"
artificial="ARTIFICIALE"
cd $bpath 

   	printf "Inserisci la modalita' di avvio del sistema : NORMALE o ARTIFICIALE\n - "

while (($i == 0))
do
	i=$((i+1))
	read mode
	if [ $mode = $normal ]; then
		./hmi_input 0
	elif [ $mode = $artificial ]; then
		./hmi_input 1
	else
   		 printf "\nComando non corretto. Selezionare una delle due modalita' di funzionamento\n - "
		i=$((0))
	fi
done


