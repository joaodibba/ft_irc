#!/bin/bash
NICK="KIKA"
# Abrir conexão e redirecionar entrada e saída
exec 3<>/dev/tcp/localhost/6667
sleep 1
# Enviar comandos iniciais
    echo "PASS pass" >&3
	sleep 1
    echo "NICK $NICK" >&3
	sleep 1
	echo "USER $NICK 0 * :$NICK" >&3
	sleep 1
    echo "JOIN #TRAUMA" >&3	
	sleep 1
	ecoh "PRIVMSG #TRAUMA :cuidado pessoal"  >&3
	
# Ler saída do servidor em background
cat <&3 &

# Permitir entrada interativa
while true; do
    read -r input
    echo "$input" >&3
done