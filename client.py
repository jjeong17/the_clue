import socket
import struct

import asyncio
import aioconsole

HOST = "127.0.0.1"
PORT = 8687

CLIENT_ID = 15

global_game_id = -1

class Connection_Manager:
    def __init__(self):
        self.requests = asyncio.Queue(20)
        self.responses = []

def craft_checkin_message():
    # message_type for checkin = 0
    return struct.pack("!I", 0)

def craft_create_game_message():
    # message_type for create game = 14
    return struct.pack("!I", 14)

def craft_join_game_message(game_id: int):
    # message_type for join game = 15
    return struct.pack("!I", 15) + struct.pack("<I", game_id) + struct.pack("<I", CLIENT_ID)

def craft_make_move_message(move_option: int):
    # message_type for make move = 31
    return struct.pack("!I", 31) + struct.pack("<I", global_game_id) + struct.pack("<I", CLIENT_ID) + struct.pack("<I", move_option)


def prep_msg_for_send(client_id: int, message: bytes):
    magic_bytes = 0x12345678
    # Add a null byte to keep C happy
    message = message + b'\0'
    message_len = len(message)

    header = struct.pack("!III", magic_bytes, client_id, message_len)

    return header + message

async def parse_response(sock_fd) -> bytes:
    global global_game_id
    magic_bytes_recv = struct.unpack("!I", sock_fd.recv(4))[0]
    if magic_bytes_recv != 0x1234:
        print(f"Magic bytes: {magic_bytes_recv} Problem!")
    msg_len = sock_fd.recv(4)
    msg_len = struct.unpack("!I", msg_len)[0]
    data = sock_fd.recv(msg_len)

    response_opcode = struct.unpack("!I", data[:4])[0]

    if response_opcode == 10:
        return "Connection to server successfully established"

    content_len = struct.unpack("!I", data[4:8])[0]
    if content_len + 8 != len(data):
        print("Error with response")

    if response_opcode == 21:
        game_id = struct.unpack("<I", data[8:12])[0]
        global_game_id = game_id
        return f"Game with id: {global_game_id} successfully created!"
    elif response_opcode == 23:
        game_id = struct.unpack("<I", data[8:12])[0]
        global_game_id = game_id
        return f"Game with id: {global_game_id} successfully joined!"
    else:
        return data[8:].strip(b'\x00')


async def manage_connection(conn: Connection_Manager, sock_fd):
    # Send initial checkin:
    sock_fd.send(prep_msg_for_send(CLIENT_ID, craft_checkin_message()))

    # Wait for response
    out = await parse_response(sock_fd)
    print(out)

    while True:
        user_req = await conn.requests.get()
        sock_fd.send(user_req)
        out = await parse_response(sock_fd)
        print(f"Message from server received: {out}")

async def shell(client_id, conn_manager: Connection_Manager):
    global global_game_id
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
                print("     gameid <= Shows current game")
                print("")
                print("     move <move option> <= make a move")
                continue
            if l[0] == "gameid":
                if (global_game_id == -1):
                    print("You have not joined a game")
                    continue
                print(f"You are in game with id: {global_game_id}")
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
                print("game: command format not recognized")
                continue
        elif l[0] == "move":
            if (global_game_id == -1):
                    print("You have not joined a game")
                    continue
            move_option = int(l[1])
            if move_option < 0 or move_option > 20:
                print("Invalid move option!")
                continue
            msg = prep_msg_for_send(client_id, craft_make_move_message(move_option))
            await conn_manager.requests.put(msg)
            continue
        else:
            print("command format not recognized")
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