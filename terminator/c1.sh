#!/bin/bash
NICK="$RPR"
# Abrir conexão e redirecionar entrada e saída
exec 3<>/dev/tcp/localhost/6667
sleep 1
# Enviar comandos iniciais
echo "PASS $NICK" >&3
sleep 1
echo "NICK $NICK" >&3
sleep 1
echo "USER $NICK 0 * :$NICK" >&3
sleep 1
echo "JOIN #NS" >&3
sleep 1

echo "MODE #NS -i" >&3
echo "MODE #NS -t" >&3
echo "MODE #NS +l 2" >&3
echo "MODE #NS +k NS" >&3

echo "PRIVMSG #NS :OLA NEUROSEMANTICISTAS" >&3

# Ler saída do servidor em background
cat <&3 &

# Permitir entrada interativa
while true; do
    read -r input
    echo "$input" >&3
done