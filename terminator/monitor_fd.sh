#!/bin/bash

PROCESS_NAME="ircserv"  # Nome do processo a ser monitorado
INTERVAL=1              # Intervalo de verificação em segundos
OLD_FDS=()              # Array para armazenar FDs antigos

echo "Monitorando File Descriptors (FDs) do processo '$PROCESS_NAME'..."
echo "Aguardando mudanças..."

while true; do
    # Obtém a lista de FDs abertos pelo processo
    CURRENT_FDS=($(lsof -i -n -P | grep "$PROCESS_NAME" | awk '{print $4}'))

    # Só exibe se houver mudança nos FDs
    if [[ "${CURRENT_FDS[*]}" != "${OLD_FDS[*]}" ]]; then
        echo "FDs ativos: ${CURRENT_FDS[*]}"
        OLD_FDS=("${CURRENT_FDS[@]}")
    fi

    # Aguarda antes da próxima verificação
    sleep $INTERVAL
done


