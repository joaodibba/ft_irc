#!/bin/bash

# Cria um arquivo de layout temporário
cat << EOF > layout.cfg
[global_config]
[keybindings]
[profiles]
  [[default]]
    split_to_group = True
[layouts]
  [[default]]
    [[[window0]]]
      type = Window
      parent = ""
      size = 1000, 800 # Tamanho inicial da janela (opcional)
    [[[child1]]]
      type = Terminal
      parent = window0
    [[[child2]]]
      type = Terminal
      parent = window0
      split_type = vertical
    [[[child3]]]
      type = Terminal
      parent = child2
      split_type = horizontal
    [[[child4]]]
      type = Terminal
      parent = child1
      split_type = horizontal
[plugins]
EOF

# Inicia o Terminator usando o arquivo de layout
terminator -l ./layout.cfg &

# Aguarda um pouco para que o Terminator seja iniciado
sleep 1

# Obtém o PID do Terminator (opcional, mas recomendado)
TERMINATOR_PID=$!

# Executa os comandos em cada terminal (você precisa especificar o grupo correto)
terminator -x bash -c 'c; mr ; ./ircserv 6667 rpr; exec zsh' -g 0 -e & # Servidor IRC (canto superior esquerdo)
terminator -x bash -c './monitor_fd.sh' -g 1 -e & # Monitor de FDs (canto superior direito)
terminator -x bash -c 'nc localhost 6667' -g 2 -e & # Cliente 1 (canto inferior esquerdo)
terminator -x bash -c 'nc localhost 6667' -g 3 -e & # Cliente 2 (canto inferior direito)

# Remove o arquivo de layout temporário (opcional)
# rm layout.cfg

# Espera os comandos terminarem (opcional)
wait $TERMINATOR_PID