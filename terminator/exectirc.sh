#!/bin/bash
# Obter o caminho do diretório onde o script está localizado
SCRIPT_DIR=$(dirname "$(realpath "$0")")

# Caminho do arquivo atual
CURRENT_FILE="$SCRIPT_DIR/irc_terminator3"
CURRENT_FILE="$SCRIPT_DIR/irc_terminator3"

# Diretório de destino
DEST_DIR="$HOME/.config/terminator"
DEST_DIR="$HOME/.config/terminator"

# Nome do link simbólico
LINK_NAME="irc_terminator3"

# Cria o diretório de destino se não existir
mkdir -p "$DEST_DIR"

# Cria o link simbólico
ln -sf "$CURRENT_FILE" "$DEST_DIR/$LINK_NAME"

echo "Link simbólico criado: $DEST_DIR/$LINK_NAME -> $CURRENT_FILE"

# # Executa o arquivo
# bash "$CURRENT_FILE"
# # Executa o arquivo
# bash "$CURRENT_FILE"