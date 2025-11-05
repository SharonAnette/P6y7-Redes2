# Cliente para recibir video a través de UDP
import cv2
import imutils
import socket
import numpy as np
import time
import base64

BUFF_SIZE = 65536

# Crear socket UDP
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
client_socket.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, BUFF_SIZE)

# Configuración de IP y puerto
host_ip = '172.100.75.142'  # IP del servidor
port = 8080
message = b'Conectado'
client_socket.sendto(message, (host_ip, port))

fps, st, frames_to_count, cnt = (0, 0, 20, 0)

while True:
    # Recibir paquete de datos
    packet, _ = client_socket.recvfrom(BUFF_SIZE)
    
    # Decodificar el paquete base64
    data = base64.b64decode(packet, ' /')
    
    # Convertir datos a un array de numpy
    npdata = np.frombuffer(data, dtype=np.uint8)
    
    # Decodificar la imagen
    frame = cv2.imdecode(npdata, 1)
    
    # Mostrar el FPS en el frame
    frame = cv2.putText(frame, f'FPS: {fps}', (10, 40), 
                        cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
    
    # Mostrar el frame en la ventana del cliente
    cv2.imshow("Recibiendo video", frame)

    # Salir si se presiona 'q'
    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        client_socket.close()
        cv2.destroyAllWindows()
        print("Conexión cerrada.")
        break

    # Calcular FPS
    if cnt == frames_to_count:
        try:
            fps = round(frames_to_count / (time.time() - st))
            st = time.time()
            cnt = 0
        except ZeroDivisionError:
            pass

    cnt += 1

