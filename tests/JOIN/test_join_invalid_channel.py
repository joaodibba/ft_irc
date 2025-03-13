import unittest
import os
from dotenv import load_dotenv
from irc_client import IRCClient

load_dotenv()

class TestJoinInvalidChannel(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.server = os.getenv("IRC_SERVER", "localhost")
        cls.port = int(os.getenv("IRC_PORT", 6667))
        cls.nickname = os.getenv("IRC_NICKNAME", "TestBot")
        cls.password = os.getenv("IRC_PASSWORD", "pass")

    def setUp(self):
        self.irc = IRCClient(self.server, self.port, self.nickname, self.password)
        self.irc.connect()

    def tearDown(self):
        self.irc.disconnect()
        
    def test_join_channel(self):
        self.irc.send_command(f"JOIN cenas")

if __name__ == "__main__":
    unittest.main()
