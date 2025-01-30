#aliases customizados
##docker
alias c='clear'
alias d='docker'
alias dc='docker-compose'

alias dup='docker-compose up -d'
alias ddown='docker-compose stop'

alias dstart='docker start'
alias dstop='docker stop'

alias db='docker image build'
alias dp='docker ps -a --format "table {{.ID}}\t{{.Status}}\t{{.Ports}}\t{{.Names}}"'
alias di='docker images'
alias dr='docker container run'
alias de='docker exec'
alias dpa='docker ps -a'
alias dpqa='docker ps -qa'alias c='clear'

alias dstopall='docker stop $(docker ps -qa)'
alias drmall='docker rm $(docker ps -qa)'
alias daemons='systemctl list-units --type=service --state=running'
alias dv='docker volume'
alias dsy='docker system'alias c='clear'
alias dclean='dsall ; drm ; drmi ; c'
alias dloge='docker logs'
alias dspall='docker system prune --all -f'

deit() {     
  docker exec -it "$1" /bin/sh
  }

#makefile
alias m='make all'
alias mc='make clean'
alias mf='make fclean'
alias ml='make list'
alias mi='make inspect'
alias mb='make build'
alias mr='make re'
alias mb='make build'


alias cc='~/.confenv/.cc.sh'
alias sp='~/.confenv/.sp.sh'
alias fd='lsof -i -n -P | grep ircserv'

alias t='terminator -g ~/.config/terminator/default'
alias tirc='terminator -g ~/.config/terminator/irc_terminator'

alias rl='source ~/.zshrc'
