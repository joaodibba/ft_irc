import unittest
import os
from dotenv import load_dotenv
from irc_client import IRCClient

load_dotenv()

class TestPrivateMessage(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.server = os.getenv("IRC_SERVER", "localhost")
        cls.port = int(os.getenv("IRC_PORT", 6667))
        cls.nickname = os.getenv("IRC_NICKNAME", "TestBot")
        cls.password = os.getenv("IRC_PASSWORD", "pass")
        cls.channel = os.getenv("IRC_CHANNEL", "#testchannel")

    def setUp(self):
        self.irc = IRCClient(self.server, self.port, self.nickname, self.password)
        self.irc.connect()

    def tearDown(self):
        self.irc.disconnect()
        
    def test_join_channel(self):
        self.irc.send_command(f"JOIN {self.channel}")

    def test_mode_o1(self):
        password = "pass"
        flag = "+o"
        self.irc.send_command(f"MODE {self.channel} {flag} {password}")
    def test_mode_o0(self):
        password = "pass"
        flag = "-o"
        self.irc.send_command(f"MODE {self.channel} {flag} {password}")

if __name__ == "__main__":
    unittest.main()
