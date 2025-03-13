import socket
import time

class IRCClient:
    def __init__(self, server, port, nickname, password, username="testuser", realname="Test User"):
        self.server = server
        self.port = port
        self.nickname = nickname
        self.username = username
        self.realname = realname
        self.password = password
        self.irc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def connect(self):
        """Connect to the IRC server and perform initial authentication."""
        self.irc.connect((self.server, self.port))
        self.send_command(f"PASS {self.password}")
        self.send_command(f"NICK {self.nickname}")
        self.send_command(f"USER {self.username} 0 * :{self.realname}")
        time.sleep(1)  # Allow server response time
        return self.get_response()

    def send_command(self, command):
        """Send a command to the IRC server."""
        self.irc.send((command + "\r\n").encode())
        time.sleep(0.5)

    def get_response(self, buffer_size=1024):
        """Receive data from the server."""
        return self.irc.recv(buffer_size).decode(errors="ignore")

    def disconnect(self):
        """Disconnect from the IRC server."""
        self.send_command("QUIT :Testing module quit")
        self.irc.close()
