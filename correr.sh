#! /bin/bash 
make clean;
make;

RESULTADO=$PWD/salidaMari.txt;
echo $RESULTADO;



echo "Corriendo para archivo pruebaLongLong.txt"  > $RESULTADO;

for y in "_p" "_t";
do
    for z in 5 10;
    do
        for x in {1..10};
        do  
            echo -n "prueba "$x" cript"$y" -c" $z" "  >> $RESULTADO;
            ./cript$y -c $z pruebaLongLong.txt encriptado.txt >> $RESULTADO;
            echo -n "         cript"$y" -d" $z" "  >> $RESULTADO;
            ./cript$y -d $z encriptado.txt desencriptado.txt  >> $RESULTADO;
            echo " " >> $RESULTADO;
        done
    done
done

for x in {1..10};
    do  
        echo $x >> $RESULTADO;
        echo -n "   cript tam:" >> $RESULTADO;
        ./cript -c pruebaLongLong.txt encriptado.txt >> $RESULTADO;
        echo -n "   decript" >> $RESULTADO;
        ./cript -d encriptado.txt desencriptado.txt  >> $RESULTADO;
    done

echo "Corriendo para archivo pruebaShort.txt"  >> $RESULTADO;

for y in "_p" "_t";
do
    for z in 2 3;
    do
        for x in {1..10};
        do  
            echo -n "prueba "$x" cript"$y" -c" $z" "  >> $RESULTADO;
            ./cript$y -c $z pruebaShort.txt encriptado.txt >> $RESULTADO;
            echo -n "         cript"$y" -d" $z" "  >> $RESULTADO;
            ./cript$y -d $z encriptado.txt desencriptado.txt  >> $RESULTADO;
            echo " " >> $RESULTADO;
        done
    done
done

for x in {1..10};
    do  
        echo $x >> $RESULTADO;
        echo -n "   cript tam:" >> $RESULTADO;
        ./cript -c pruebaShort.txt encriptado.txt >> $RESULTADO;
        echo -n "   decript" >> $RESULTADO;
        ./cript -d encriptado.txt desencriptado.txt  >> $RESULTADO;
    done

