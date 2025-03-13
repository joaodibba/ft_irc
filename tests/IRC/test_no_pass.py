import unittest
import os
from dotenv import load_dotenv
from irc_client import IRCClient

load_dotenv()

class TestNoPass(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.server = os.getenv("IRC_SERVER", "localhost")
        cls.port = int(os.getenv("IRC_PORT", 6667))
        cls.nickname = os.getenv("IRC_NICKNAME", "TestBot")
        cls.channel = os.getenv("IRC_CHANNEL", "#testchannel")

    def setUp(self):
        self.irc = IRCClient(self.server, self.port, self.nickname, "")
        self.irc.connect()
        

    def test_info(self):
        pass

    def tearDown(self):
        pass

if __name__ == "__main__":
    unittest.main()
