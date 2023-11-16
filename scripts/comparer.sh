for umbral1 in $(seq 5.1 0.1 8);do   
    echo -ne "$umbral1\t";
    scripts/run_vad.sh $umbral1 | fgrep TOTAL 
done | sort -t: -k2n

#añadiendo opciones al automata
#al principio hay sonido 
#las pausas entre letras(consonantes sordas)
#maybe voz (otro estado), si estoy mucho rato si es voz, sino no
#