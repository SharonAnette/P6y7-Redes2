const dgram = require('dgram'); 
const PORT = 8080; 
const HOST = '127.0.0.1'; 

setInterval(() => { 
    const cliente = dgram.createSocket('udp4'); 

    // Enviar mensaje al servidor
    const mensaje = Buffer.from('Mando paquete UDP');
    cliente.send(mensaje, PORT, HOST, (err) => { 
        if (err) { 
            console.error('Error al enviar el mensaje:', err); 
        } else {
            console.log('Mensaje UDP enviado');
        }
    }); 

    // Escuchar respuesta del servidor
    cliente.on('message', (respuesta, rinfo) => {
        console.log(`Respuesta del servidor: ${respuesta} de ${rinfo.address}:${rinfo.port}`);
        cliente.close();
    });

    // Manejador de errores en el cliente
    cliente.on('error', (err) => {
        console.error('Error en el cliente:', err);
        cliente.close();
    });

}, 1000);  // Envía un mensaje cada segundo

