function updatePosition(msg){
    if (mqtt.isConnected() ){
        // message = new Paho.MQTT.Message("Hello");
        // message.destinationName = "World";
        // client.send(message);
        mqtt.send('camera/test/get',msg);
    }
}