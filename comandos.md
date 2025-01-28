telnet localhost 6667
PASS rpr
NICK kika
USER kika 1 * :cristiane
USER kika 1 * :kika!


/NICK kika
/NICK gio
/NICK bab

/KICK - Eject a client from the channel: 
/INVITE - Invite a client to a channel
/TOPIC - Change or view the channel topic
/TOPIC #channelName <topicName>

/MODE - Change the channel’s mode:

/MODE #chanelName <i, t, k, o, l>

· i: Set/remove Invite-only channel
	/MODE #channelName +i ou -i

· t: Set/remove the restrictions of the TOPIC command to channel operators
	/MODE #channelName +t ou -t

· k: Set/remove the channel key (password)

· o: Give/take channel operator privilege

· l: Set/remove the user limit to channel

# Etapas de processmento

validacao dos dados de chamada do sevidor
	quantidade de parametros
	inicializacao do objeto irc, que por sua vez inicializa a estrutura base do irc
	

## Em receiveRequest
O servidor avaliar se o fd do cliente esta na lista de clientes.
Se houver conteudo o dado e armazenado no buffer do cliente e incluido na lista de comandos enviados pelo clinete no map<fd,cliente->buffer>
E altera o status do epoll para o fd(socket) do cliente para pronto para enviar mensagem ao cliente.


## Em sendResponse 


## Inicialização
