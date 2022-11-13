import socket
import struct

import asyncio
import aioconsole

HOST = "127.0.0.1"
PORT = 8687

CLIENT_ID = 15


class Connection_Manager:
    def __init__(self):
        self.requests = asyncio.Queue(20)
        self.responses = []

def craft_checkin_message():
    # message_type for checkin = 0
    return struct.pack("!I", 0)

def craft_create_game_message():
    # message_type for create game = 3
    return struct.pack("!I", 3)

def craft_join_game_message(game_id: int):
    # message_type for join game = 2
    return struct.pack("!II", 2, game_id)

def craft_roll_dice_message():
    # message_type for roll dice = 4
    return struct.pack("!I", 4)

def craft_make_suggestion_message(card1: str, card2: str, card3: str):
    # message_type for making a suggestion = 5
    return struct.pack("!II", 5, card1, card2, card3)

def craft_make_accusation_message(card1: str, card2: str, card3: str):
    # message_type for making a suggestion = 6
    return struct.pack("!II", 6, card1, card2, card3)

def craft_end_turn_message():
    # message_type for roll dice = 7
    return struct.pack("!I", 7)

def craft_leave_game_message():
    # message_type for roll dice = 8
    return struct.pack("!I", 8)

def prep_msg_for_send(client_id: int, message: bytes):
    magic_bytes = 0x12345678
    # Add a null byte to keep C happy
    message = message + b'\0'
    message_len = len(message)

    header = struct.pack("!III", magic_bytes, client_id, message_len)

    return header + message

async def parse_response(sock_fd) -> bytes:
    magic_bytes_recv = struct.unpack("!I", sock_fd.recv(4))[0]
    if magic_bytes_recv != 0x1234:
        print(f"Magic bytes: {magic_bytes_recv} Problem!")
    msg_len = sock_fd.recv(4)
    msg_len = struct.unpack("!I", msg_len)[0]
    data = sock_fd.recv(msg_len)

    return data

async def manage_connection(conn: Connection_Manager, sock_fd):
    # Send initial checkin:
    sock_fd.send(prep_msg_for_send(CLIENT_ID, craft_checkin_message()))

    # Wait for response
    resp = await parse_response(sock_fd)
    print(resp)

    while True:
        user_req = await conn.requests.get()
        sock_fd.send(user_req)
        resp = await parse_response(sock_fd)
        resp = resp.strip(b'\x00')
        print(f"Message from server received: {resp}")

async def shell(client_id, conn_manager: Connection_Manager):
    while True:
        a = await aioconsole.ainput(">>")
        l = a.split()
        if len(l) == 0:
            print("", end=" ")
            continue
        if len(l) == 1:
            if l[0] == "help":
                print("Help Menu:")
                print("     game join <game id>")
                print("     game create")
                print("     roll")
                print("     suggest <card1> <card2> <card3>")
                print("     accuse <card1> <card2> <card3>")
                print("     end turn")
                print("     leave game")
                continue
            else:
                print(f"Unknown Command: {l[0]}")
                print("type help for help menu")
                continue
        if l[0] == "game":
            if l[1] == "join":
                if len(l) == 3:
                    msg = prep_msg_for_send(client_id, craft_join_game_message(int(l[2])))
                    await conn_manager.requests.put(msg)
                else:
                    print("game: command format not recognized")
                    continue
            elif l[1] == "create":
                msg = prep_msg_for_send(client_id, craft_create_game_message())
                await conn_manager.requests.put(msg)
            else:
                print("command format not recognized")
                continue
        elif l[0] == "roll":
            msg = prep_msg_for_send(client_id, craft_roll_dice_message())
            await conn_manager.requests.put(msg)
        elif l[0] == "suggest":
            if len(l) == 4:
                msg = prep_msg_for_send(client_id, craft_make_suggestion_message(l[1], l[2], l[3]))
                await conn_manager.requests.put(msg)
            else:
                print("suggest: command format not recognized.  Please enter:")
                print("     suggest <card1> <card2> <card3>")
                continue
        elif l[0] == "accuse":
            if len(l) == 4:
                msg = prep_msg_for_send(client_id, craft_make_accusation_message(l[1], l[2], l[3]))
                await conn_manager.requests.put(msg)
            else:
                print("accuse: command format not recognized.  Please enter:")
                print("     accuse <card1> <card2> <card3>")
        elif l[0] == "end":
            if l[1] == "turn":
                msg = prep_msg_for_send(client_id, craft_end_turn_message())
                await conn_manager.requests.put(msg)
            else:
                print("end: command format not recognized.  Did you mean <end turn> ?")
                continue
        elif l[0] == "leave":
            if l[1] == "game":
                msg = prep_msg_for_send(client_id, craft_leave_game_message())
                await conn_manager.requests.put(msg)
            else:
                print("leave: command format not recognized.  Did you mean <leave game> ?")
                continue


async def main():

    client_hello_message = prep_msg_for_send(CLIENT_ID, b'Hello server!')

    conn = Connection_Manager()

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        # s.send(client_hello_message)
        # data = s.recv(8)

        # magic_bytes, data_len = struct.unpack("!II", data)[:2]

        # print(f"Received: ")
        # print(f"Magic bytes: {hex(magic_bytes)}")
        # print(f"Data length: {data_len}")

        # data = s.recv(data_len)

        # print(f"Received: {data}")

        await asyncio.gather(shell(CLIENT_ID, conn), manage_connection(conn, s))
        # await shell(client_id, s)

if __name__ == "__main__":
    asyncio.run(main())