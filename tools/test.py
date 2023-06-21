import socket

HOST = "192.168.111.177"  # Standard loopback interface address (localhost)
PORT = 20003  # Port to listen on (non-privileged ports are > 1023)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    while True:
        cmd = input(":")
        s.sendall(bytes(cmd, 'ascii'))
        data = s.recv(1024)
        print("\n")
        print(data)