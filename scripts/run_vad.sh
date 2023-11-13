#!/bin/bash

#if [ $# != 1 ]; then 
#    echo "usage: $0 umbral1"
#    exit 1 
#fi
#if [ $# != 3]; then
#    echo "Usage: $0 missing parameters"
#    exit -1
#fi
    umbral1=$1
    umbral2=$2
    tvoice=$3
    tsilence=$4

# Be sure that this file has execution permissions:
# Use the nautilus explorer or chmod +x run_vad.sh

# Establecemos que el código de retorno de un pipeline sea el del último programa con código de retorno
# distinto de cero, o cero si todos devuelven cero.
set -o pipefail

# Write here the name and path of your program and database
DIR_P2=$HOME/PAV/P2
DB=$DIR_P2/db.v4
CMD="$DIR_P2/bin/vad --umbral1=$alfa1 --umbral2=$alfa2 --tvoice=$tvoice --tsilence=$tsilence " 

for filewav in $DB/*/*wav; do
#    echo
    echo "**************** $filewav ****************"
    if [[ ! -f $filewav ]]; then 
	    echo "Wav file not found: $filewav" >&2
	    exit 1
    fi

    filevad=${filewav/.wav/.vad}

    $CMD -i $filewav -o $filevad || exit 1

# Alternatively, uncomment to create output wave files
#    filewavOut=${filewav/.wav/.vad.wav}
#    $CMD $filewav $filevad $filewavOut || exit 1

done

scripts/vad_evaluation.pl $DB/*/*lab

exit 0
