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


## em run_server
em isNewClient, evs[i]].data.fd, caso o fd retornado pelo evento sejam igual ao fd o server, significa que se trata de um novo cliente.
Na funcao acceptClinet é iniciado um novo cliente com:
	* criação de um novo Sock e respectivo fd.
	* na criação do novo Sock é feita a parametrização de:
    	* sockaddr_in
    	* Associacao do n


cockpit:
[[[terminal2]]]
title = server
command = c; rm; ./ircserv 6667 rpr

[[[terminal6]]]
title = fd


terminator/count_fd.sh
terminator/monitor_fd.sh

[[[terminal8]]]
title = file_client
command = tail -f file_client.txt

command = c; cn localhost 6667