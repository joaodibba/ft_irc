#!/bin/bash

# Caminho do arquivo atual
CURRENT_FILE="/home/rprocopi/02_commoncore/ft_irc/terminator/irc_terminator3"

# Diretório de destino
DEST_DIR="/home/rprocopi/.config/irc_terminator3"

# Nome do link simbólico
LINK_NAME="irc_terminator3"

# Cria o diretório de destino se não existir
mkdir -p "$DEST_DIR"

# Cria o link simbólico
ln -sf "$CURRENT_FILE" "$DEST_DIR/$LINK_NAME"

echo "Link simbólico criado: $DEST_DIR/$LINK_NAME -> $CURRENT_FILE"

# Executa o arquivo
bash "$CURRENT_FILE"