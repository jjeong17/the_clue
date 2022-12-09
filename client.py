import socket
import struct

import asyncio
import aioconsole
import PySimpleGUI as sg

import time

HOST = "127.0.0.1"
PORT = 8687

CLIENT_ID = 15

global_game_id = -1

start_time = time.time()

class Connection_Manager:
    def __init__(self):
        self.requests = asyncio.Queue(20)
        self.responses = []

def craft_checkin_message():
    # message_type for checkin = 0
    return struct.pack("!I", 0)

def craft_periodic_checkin_message():
    # message_type for periodic checkin = 5
    return struct.pack("!I", 5)

def craft_create_game_message():
    # message_type for create game = 14
    return struct.pack("!I", 14) + struct.pack("<I", CLIENT_ID)

def craft_join_game_message(game_id: int):
    # message_type for join game = 15
    return struct.pack("!I", 15) + struct.pack("<I", game_id) + struct.pack("<I", CLIENT_ID)

def craft_make_move_message(move_option: int):
    # message_type for make move = 31
    return struct.pack("!I", 31) + struct.pack("<I", global_game_id) + struct.pack("<I", CLIENT_ID) + struct.pack("<I", move_option)

def craft_make_suggestion_message(other_player_id: int, weapon_id: int, location: int):
    return struct.pack("!I", 36) + struct.pack("<I", global_game_id) + struct.pack("<I", CLIENT_ID) + struct.pack("<I", other_player_id) + struct.pack("<I", weapon_id) + struct.pack("<I", location)

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


async def periodic_checkin(conn: Connection_Manager):
    while True:
        await asyncio.sleep(1)
        msg = prep_msg_for_send(CLIENT_ID, craft_periodic_checkin_message())
        await conn.requests.put(msg)

async def manage_connection_i(conn: Connection_Manager, sock_fd):
    # Wait for response
    out = await parse_response(sock_fd)
    print(out)

    while True:
        user_req = await conn.requests.get()
        sock_fd.send(user_req)
        out = await parse_response(sock_fd)
        print(f"Message from server received: {out}")

async def manage_connection(conn: Connection_Manager, sock_fd):
    # Send initial checkin:
    sock_fd.send(prep_msg_for_send(CLIENT_ID, craft_checkin_message()))

    await asyncio.gather(periodic_checkin(conn), manage_connection_i(conn, sock_fd))

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
                print("     suggestion <other player id> <weapon> <location> <= make a suggestion")
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
        elif l[0] == "suggestion":
            if (global_game_id == -1):
                print("You have not joined a game")
                continue
            target_player = int(l[1])
            weapon = int(l[2])
            location = int(l[3])
            msg = prep_msg_for_send(client_id, craft_make_suggestion_message(target_player, weapon, location))
            await conn_manager.requests.put(msg)
            continue
        else:
            print("command format not recognized")
            continue

def interpretRadioButtons(values):

    if(values['p1'] == True):
        person = "MISS_SCARLETT"
    elif(values['p2'] == True):
        person = "PROFESSOR_PLUM"
    elif(values['p3'] == True):
        person = "COLONEL_MUSTARD"
    elif(values['p4'] == True):
        person = "MRS_PEACOCK"
    elif(values['p5'] == True):
        person = "MR_GREEN"
    elif(values['p6'] == True):
        person = "MRS_WHITE"
    else:
        person = ""

    if(values['w1'] == True):
        weapon = "DAGGER"
    elif(values['w2'] == True):
        weapon = "CANDLESTICK"
    elif(values['w3'] == True):
        weapon = "REVOLVER"
    elif(values['w4'] == True):
        weapon = "ROPE"
    elif(values['w5'] == True):
        weapon = "LEAD_PIPE"
    elif(values['w6'] == True):
        weapon = "SPANNER"
    else:
        weapon = ""

    if(values['l1'] == True):
        location = "STUDY"
    elif(values['l2'] == True):
        location = "HALL"
    elif(values['l3'] == True):
        location = "LOUNGE"
    elif(values['l4'] == True):
        location = "LIBRARY"
    elif(values['l5'] == True):
        location = "BILLIARD_ROOM"
    elif(values['l6'] == True):
        location = "DINING_ROOM"
    elif(values['l7'] == True):
        location = "CONSERVATORY"
    elif(values['l8'] == True):
        location = "BALLROOM"
    elif(values['l9'] == True):
        location = "KITCHEN"
    else:
        location = ""
    return person, weapon, location

def convertPlayer(person):
    if(person == "MISS_SCARLETT"):
        person = 1
    elif(person == "PROFESSOR_PLUM"):
        person = 2
    elif(person == "COLONEL_MUSTARD"):
        person = 3
    elif(person == "MRS_PEACOCK"):
        person = 4
    elif(person == "MR_GREEN"):
        person = 5
    elif(person == "MRS_WHITE"):
        person = 6
    else:
        person = -1

def convertWeapon(weapon):
    if(weapon == "DAGGER"):
        weapon = 0
    elif(weapon == "CANDLESTICK"):
        weapon = 1
    elif(weapon == "REVOLVER"):
        weapon = 2
    elif(weapon == "ROPE"):
        weapon = 3
    elif(weapon == "LEAD_PIPE"):
        weapon = 4
    elif(weapon == "SPANNER"):
        weapon = 5
    else:
        weapon = -1


def convertLocation(place):
    if(place == "STUDY"):
        return 0
    elif(place == "HALL"):
        return 2
    elif(place == "LOUNGE"):
        return 4
    elif(place == "LIBRARY"):
        return 8
    elif(place == "BILLIARD_ROOM"):
        return 10
    elif(place == "DINING_ROOM"):
        return 12
    elif(place == "CONSERVATORY"):
        return 16
    elif(place == "BALLROOM"):
        return 18
    elif(place == "KITCHEN"):
        return 20
    else:
        return -1

async def main():

    client_hello_message = prep_msg_for_send(CLIENT_ID, b'Hello server!')

    conn = Connection_Manager()
    #horizontal_hallway = sg.Button('Hallway', size=(16, 4))
    #vertical_hallway = sg.Button('Hallway', size=(8, 12))
    layout = [
        [
            sg.Frame('Study',
                [
                    [sg.Button('Move To', key='study')],
                    [sg.Text('', size=(16,6), key='studytext')]
                    
                ]
            ),

            sg.Frame('Hallway',
                [
                    [sg.Button('Move To',  key='hw1')],
                    [sg.Text('', size=(16,6), key='hw1text')]
                    
                ]
            ),

            sg.Frame('Hall',
                [
                    [sg.Button('Move To',  key='hall')],
                    [sg.Text('', size=(16,6), key='halltext')]
                    
                ]
            ),

            sg.Frame('Hallway',
                [
                    [sg.Button('Move To',  key='hw2')],
                    [sg.Text('', size=(16,6), key='hw2text')]
                    
                ]
            ),

            sg.Frame('Lounge',
                [
                    [sg.Button('Move To',  key='lounge')],
                    [sg.Text('', size=(16,6), key='loungetext')]
                    
                ]
            )
        ],
        [
            sg.Frame('Hallway',
                [
                    [sg.Button('Move To',  key='hw3')],
                    [sg.Text('', size=(16,6), key='hw3text')]
                    
                ]
            ),
            sg.HSeparator(pad=(14, 0)), 
            sg.Frame('Hallway',
                [
                    [sg.Button('Move To',  key='hw4')],
                    [sg.Text('', size=(16,6), key='hw4text')]
                    
                ]
            ),
            sg.HSeparator(pad=(14, 0)), 
            sg.Frame('Hallway',
                [
                    [sg.Button('Move To',  key='hw5')],
                    [sg.Text('', size=(16,6), key='hw5text')]
                    
                ]
            )
        ],
        [
        
            sg.Frame('Library',
                [
                    [sg.Button('Move To',  key='library')],
                    [sg.Text('', size=(16,6), key='librarytext')]
                    
                ]
            ),

            sg.Frame('Hallway',
                [
                    [sg.Button('Move To',  key='hw6')],
                    [sg.Text('', size=(16,6), key='hw6text')]
                    
                ]
            ), 

            sg.Frame('Billiard Room',
                [
                    [sg.Button('Move To',  key='billiardroom')],
                    [sg.Text('', size=(16,6), key='billiardroomtext')]
                    
                ]
            ),
            sg.Frame('Hallway',
                [
                    [sg.Button('Move To',  key='hw7')],
                    [sg.Text('', size=(16,6), key='hw7text')]
                    
                ]
            ),
            sg.Frame('Dining Room',
                [
                    [sg.Button('Move To',  key='diningroom')],
                    [sg.Text('', size=(16,6), key='diningroomtext')]
                    
                ]
            )
        ],
        [
        
            sg.Frame('Hallway',
                [
                    [sg.Button('Move To',  key='hw8')],
                    [sg.Text('', size=(16,6), key='hw8text')]
                    
                ]
            ),
            sg.HSeparator(pad=(14, 0)),
            sg.Frame('Hallway',
                [
                    [sg.Button('Move To',  key='hw9')],
                    [sg.Text('', size=(16,6), key='hw9text')]
                    
                ]
            ),
            sg.HSeparator(pad=(14, 0)), 
            sg.Frame('Hallway',
                [
                    [sg.Button('Move To',  key='hw10')],
                    [sg.Text('', size=(16,6), key='hw10text')]
                    
                ]
            )
        ],
        [
        
            sg.Frame('Conservatory',
                [
                    [sg.Button('Move To',  key='conservatory')],
                    [sg.Text('', size=(16,6), key='conservatorytext')]
                    
                ]
            ), 
            sg.Frame('Hallway',
                [
                    [sg.Button('Move To',  key='hw11')],
                    [sg.Text('', size=(16,6), key='hw11text')]
                    
                ]
            ), 
            sg.Frame('Ballroom',
                [
                    [sg.Button('Move To',  key='ballroom')],
                    [sg.Text('', size=(16,6), key='ballroomtext')]
                    
                ]
            ), 
            sg.Frame('Hallway',
                [
                    [sg.Button('Move To',  key='hw12')],
                    [sg.Text('', size=(16,6), key='hw12text')]
                    
                ]
            ),
            sg.Frame('Kitchen',
                [
                    [sg.Button('Move To',  key='kitchen')],
                    [sg.Text('', size=(16,6), key='kitchentext')]
                    
                ]
            )
        ],
        
        [sg.Button('Suggest'), sg.Button('Accuse')],
        

        [sg.Radio('Miss Scarlett', "PEOPLE", key = 'p1', default=False, visible=True),
        sg.Radio('Professor Plum', "PEOPLE", key = 'p2', default=False, visible=True),
        sg.Radio('Colonel Mustard', "PEOPLE", key = 'p3',default=False, visible=True),
        sg.Radio('Mrs Peacock', "PEOPLE", key = 'p4',default=False, visible=True),
        sg.Radio('Mr Green', "PEOPLE", key = 'p5',default=False, visible=True),
        sg.Radio('Mrs White', "PEOPLE", key = 'p6',default=False, visible=True),
        ],
        [sg.Radio('Dagger', "WEAPON", key = 'w1', default=False, visible=True),
        sg.Radio('Candlestick', "WEAPON", key = 'w2',default=False, visible=True),
        sg.Radio('Revolver', "WEAPON", key = 'w3',default=False, visible=True),
        sg.Radio('Rope', "WEAPON", key = 'w4',default=False, visible=True),
        sg.Radio('Lead Pipe', "WEAPON", key = 'w5',default=False, visible=True),
        sg.Radio('Spanner', "WEAPON", key = 'w6',default=False, visible=True),
        ],
        [sg.Radio('Study', "LOCATION", key = 'l1', default=False, visible=True),
        sg.Radio('Hall', "LOCATION", key = 'l2', default=False, visible=True),
        sg.Radio('Lounge', "LOCATION", key = 'l3', default=False, visible=True),
        sg.Radio('Library', "LOCATION", key = 'l4', default=False, visible=True),
        sg.Radio('Billiard Room', "LOCATION", key = 'l5', default=False, visible=True),
        sg.Radio('Dining Room', "LOCATION", key = 'l6', default=False, visible=True),
        sg.Radio('Conservatory', "LOCATION", key = 'l7', default=False, visible=True),
        sg.Radio('Ballroom', "LOCATION", key = 'l8', default=False, visible=True),
        sg.Radio('Kitchen', "LOCATION", key = 'l9', default=False, visible=True),
        ]

    ]
    window = sg.Window('THE CLUE', layout)
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        while True:
            event, values = window.read()
            print('You entered ', event)
            if(event == 'Suggest' or event == 'Accuse'):
                interpretRadioButtons(values)
                
                p, w, l = interpretRadioButtons(values)
                print(p)
                print(w)
                print(l)
                p_byte = convertPlayer(p)
                w_byte = convertWeapon(w)
                l_byte = convertLocation(l)
                #we need to pack the p_byte, w_byte, and l_byte
                # to the server (these will match what the backend expects)
            if(event == 'MOVE'):
                
            if event == sg.WIN_CLOSED or event == 'Cancel':
                break
            
        
        #THIS SECTION OF CODE NEEDS TO BE ABLE TO READ IN A RESPONSE
        #FROM BACK END AND BE ABLE TO SEND OUT THE BYTES FROM THE ACTION
        #TO THE BACK END


        # s.send(client_hello_message)
        # data = s.recv(8)

        # magic_bytes, data_len = struct.unpack("!II", data)[:2]

        # print(f"Received: ")
        # print(f"Magic bytes: {hex(magic_bytes)}")
        # print(f"Data length: {data_len}")

        # data = s.recv(data_len)

        # print(f"Received: {data}")

            #await asyncio.gather(shell(CLIENT_ID, conn), manage_connection(conn, s))
        # await shell(client_id, s)
        window.close()
if __name__ == "__main__":
    asyncio.run(main())