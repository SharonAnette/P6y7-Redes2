import socket
import struct

# Configuración del grupo de multidifusión y el puerto
MCAST_GRP = '224.1.1.1'
MCAST_PORT = 5004

# Crear socket UDP
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.bind(('', MCAST_PORT))

# Unirse al grupo de multidifusión
mreq = struct.pack("4sl", socket.inet_aton(MCAST_GRP), socket.INADDR_ANY)
sock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)

print("Servidor iniciado...")
try:
    while True:
        data, addr = sock.recvfrom(1024)  # Tamaño del buffer ajustado
        print(f"Mensaje recibido desde {addr}: {data.decode('utf-8')}")
except KeyboardInterrupt:
    print("\nServidor detenido.")
finally:
    sock.close()
