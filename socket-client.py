from socket import *
import sys

address = ('192.168.1.64', 5000)
client_socket = socket(AF_INET, SOCK_DGRAM)
client_socket.settimeout(1)

print("Comandi:\n")
print("red/green/blue  accende/spegne il led corrispondente")
print("on              accende tutti il led")
print("off             spegne tutti i led")
print("temp            legge la temperatura ambiente")
print("hum             legge l'umidità relativa")
print("bye             termina il programma\n")

while(1):
    data = input("-> ")
    if data == "bye":
        sys.exit(0)
    client_socket.sendto(data.encode(), address)
    try:
        rec_data, addr = client_socket.recvfrom(2048)
        print(rec_data.decode())
    except:
        pass
