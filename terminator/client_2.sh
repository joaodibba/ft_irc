#!/bin/bash
exec 3<>/dev/tcp/localhost/6667
    sleep 1
    echo "PASS RPR" >&3
	sleep 1
    echo "NICK GAB" >&3
	sleep 1
	echo "USER GAB 0 * :GAB" >&3
	sleep 1
    echo "JOIN #BIKE" >&3
	sleep 1
	echo "JOIN #NS" >&3
	
	echo "PRIVMSG #NS :OLA PAI?" >&3
	
	echo "PRIVMSG #BIKE :OLA GALERA QUEM QUER PEGAR UMA TRILHA?" >&3
# Ler saída do servidor em background
cat <&3 &

# Permitir entrada interativa
while true; do
    read -r input
    echo "$input" >&3
done