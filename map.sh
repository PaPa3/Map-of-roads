#!/bin/bash

# Wzorzec poprawnego numeru drogi krajowej, czyli
# liczba całkowita z zakresu [1, 999].
validRouteIds='^[1-9][0-9]{0,2}$'

# Jeśli za mało parametrów.
if (( $# < 2 ))
	then
		exit 1
fi

# Plik zawierający opisy dróg krajowych.
file="$1"

# Jeśli ścieżka do pliku jest niepoprawna.
if ! [ -f $file ]
	then
		exit 1
fi

argumentNumber=0
for routeId
do
	let "argumentNumber += 1"
	
	# Pomijamy pierwszy argument, bo to ścieżka do pliku.
	if [[ $argumentNumber = 1 ]]
		then
			continue
	fi
	
	# Czy numer drogi krajowej jest poprawny.
	if ! [[ "$routeId" =~ $validRouteIds ]]
		then
			exit 1
	fi
	
	# Linia z pliku zawierająca opis danej drogi krajowej.
	routeDescription=$(grep "^$routeId;" $file)
	
	# Jeśli nie ma informacji o danej drodze krajowej w pliku.
	if [[ $routeDescription = "" ]]
		then
			continue
	fi
	
	# Obliczanie długości danej drogi krajowej.
	result=0
	for (( i = 3; ; i += 3 ))
	do
		# i-te słowo z ciągu słów $routeDescription, gdzie słowa
		# to oddzielone znakiem ; ciągi znaków.
		x=$(echo $routeDescription | cut -d ";" -f $i)
		if [[ $x = "" ]]
			then
				break
		fi
		
		let "result += $x"
	done
	
	echo "$routeId;$result"
done

exit 0
