# filepath: /home/ronaldpr/.oh-my-zsh/custom/monitor_ircserv.sh
#!/bin/bash

previous_status=""

while true; do
    current_status=$(lsof -i -n -P | grep ircserv)
    if [ "$current_status" != "$previous_status" ]; then
        echo "Status changed:"
        echo "$current_status"
        previous_status="$current_status"
    fi
    sleep 5  # Intervalo de verificação em segundos
done