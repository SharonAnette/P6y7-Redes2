 # Servidor para enviar video a través de UDP
import cv2
import imutils
import socket
import numpy as np
import time
import base64

BUFF_SIZE = 65536

# Crear socket UDP
server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, BUFF_SIZE)

# Configuración de IP y puerto
host_ip = '172.100.75.142'  # IP del servidor
port = 8080
socket_address = (host_ip, port)
server_socket.bind(socket_address)

print(f'Escuchando en: {socket_address}')

# Capturar video desde la webcam (0 para webcam predeterminada)
vid = cv2.VideoCapture(0)

fps, st, frames_to_count, cnt = (0, 0, 20, 0)

while True:
    # Esperar mensaje de conexión del cliente
    msg, client_addr = server_socket.recvfrom(BUFF_SIZE)
    print(f'Se tiene conexión de: {client_addr}')

    # Parámetros del video
    WIDTH = 400

    while vid.isOpened():
        # Leer un frame de la webcam
        ret, frame = vid.read()
        
        if not ret:
            print("No se pudo capturar el video. Saliendo...")
            break
        
        # Redimensionar el frame
        frame = imutils.resize(frame, width=WIDTH)

        # Codificar el frame a formato JPEG
        encoded, buffer = cv2.imencode('.jpg', frame, [cv2.IMWRITE_JPEG_QUALITY, 80])
        
        # Codificar en base64
        message = base64.b64encode(buffer)

        # Enviar el frame al cliente
        server_socket.sendto(message, client_addr)

        # Mostrar el FPS en el frame
        frame = cv2.putText(frame, f'FPS: {fps}', (10, 40), 
                            cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
        
        # Mostrar el frame en la ventana de transmisión
        cv2.imshow('Transmitiendo video', frame)

        # Salir si se presiona 'q'
        key = cv2.waitKey(1) & 0xFF
        if key == ord('q'):
            server_socket.close()
            vid.release()
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
