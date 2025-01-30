#!/bin/bash

PROCESS_NAME="ircserv"  # Nome do processo a ser monitorado
INTERVAL=1              # Intervalo de verificação em segundos
LAST_FD_COUNT=0         # Inicializa a contagem anterior

echo "Monitorando quantidade de File Descriptors (FDs) do processo '$PROCESS_NAME'..."
echo "Aguardando mudanças..."

while true; do
    # Conta a quantidade de FDs abertos pelo processo
    FD_COUNT=$(lsof -i -n -P | grep "$PROCESS_NAME" | wc -l)

    # Só exibe a contagem se houver mudança
    if [[ "$FD_COUNT" -ne "$LAST_FD_COUNT" ]]; then
        echo "FDs abertos: $FD_COUNT"
        LAST_FD_COUNT=$FD_COUNT
    fi

    sleep $INTERVAL
done
