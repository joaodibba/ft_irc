#!/bin/bash
NICK="GAB"
CH="#BIKE"
exec 3<>/dev/tcp/localhost/6667
    sleep 1
    echo "PASS RPR" >&3
	sleep 1
    echo "NICK $NICK" >&3
	sleep 1
	echo "USER $NICK 0 * :$NICK" >&3
	sleep 1
    echo "JOIN $CH" >&3
	sleep 1
	echo "JOIN #NS NS" >&3
	
	echo "PRIVMSG #NS :OLA PAI?" >&3
	
	echo "PRIVMSG #BIKE :OLA GALERA QUEM QUER PEGAR UMA TRILHA?" >&3
# Ler saída do servidor em background
cat <&3 &

# Permitir entrada interativa
while true; do
    read -r input
    echo "$input" >&3
done