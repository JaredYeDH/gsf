import asyncio
import struct

import robot.message.test_pb2

from robot.connect_mgr import connect_mgr


cur_connect = 0

class LoginCase(asyncio.Protocol):

    account_name = ''
    is_connect = False
    recv_count = 0

    def __init__(self, account_name):
        self.account_name = account_name

    def connection_made(self, transport):
        self.transport = transport
        self.is_connect = True
        global cur_connect
        cur_connect = cur_connect + 1
        print ("connection made", cur_connect)

        msg = robot.message.test_pb2.info()
        msg.id = 10
        msg.name = "hello"
        buf = msg.SerializeToString()
        self.transport.write(struct.pack('<II'+str(len(buf))+'s', 8+len(buf), 1001, buf))
        connect_mgr.try_connect()

    def data_received(self, data):

        fmt = '<III'
        #self.transport.write(struct.pack(fmt, 12, 100, 2))


    def eof_received(self):
        self.is_connect = True
        pass

    def connection_lost(self, exc):
        self.is_connect = False
        print ("connection lost:", exc)