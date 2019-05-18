#!/bin/bash

# Skrypt, który dla danej drogi krajowej liczy jej długość.
#
# Użycie:
# ./map.sh FILE_PATH ROUTE_ID ROUTE_ID ... ROUTE_ID
# gdzie FILE_PATH to ścieżka do pliku zawierająca wyniki funkcji
# "getRouteDescription". Opisy dróg krajowych w pliku FILE_PATH
# muszą być poprawne oraz dla danej drogi krajowej plik zawera
# co najwyżej jedną informacje o tejże drodze.
# ROUTE_ID to numer drogi krajowej, o którą zadawane jest pytanie.
# ROUTE_ID to liczba z zakresu [1, 999] bez zer wiodących.
#
# Działanie:
# Skrypt po kolei przegląda argumenty i dla każdego argumentu typu
# ROUTE_ID zwraca długość danej drogi, jeśli jest o niej informacja we
# wskazanym pliku, krajowej w formacie
# "numer drogi krajowej;długość". Jeśli aktualnie przeglądany argument
# jest niepoprawny to skrypt kończy działanie z kodem wyjścia 1.
# Jeśli parametry są poprawne to skrypt kończy się kodem wyjścia 0.


# Wzorzec poprawnego numeru drogi krajowej, czyli
# liczba całkowita z zakresu [1, 999] bez zer wiodących.
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
		
		# Jeśli nie istnieje już i-te (oraz każde kolejne) słowo.
		if [[ $x = "" ]]
			then
				break
		fi
		
		let "result += $x"
	done
	
	echo "$routeId;$result"
done

exit 0
