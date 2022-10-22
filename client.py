import socket
import struct


HOST = "127.0.0.1"
PORT = 8687

# def craft__message():


def prep_msg_for_send(client_id, message):
    magic_bytes = 0x12345678
    # Add a null byte to keep C happy
    message = message + b'\0'
    message_len = len(message)

    header = struct.pack("!III", magic_bytes, client_id, message_len)

    return header + message

client_hello_message = prep_msg_for_send(0x1234, b'Hello server!')

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.send(client_hello_message)
    data = s.recv(8)

    magic_bytes, data_len = struct.unpack("!II", data)[:2]

    print(f"Received: ")
    print(f"Magic bytes: {hex(magic_bytes)}")
    print(f"Data length: {data_len}")

    data = s.recv(data_len)

print(f"Received: {data}")