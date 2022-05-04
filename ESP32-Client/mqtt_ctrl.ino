void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("[");
  Serial.print(topic);
  Serial.print("] ");
  String cmd = "";
  for (int i = 0; i < length; i++) {
    cmd += (char)payload[i];
  }
  Serial.println();
  executeCommand(cmd);
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(MQTT_PUBLISH_TOPIC, "Ready");
      // ... and resubscribe
      client.subscribe(MQTT_SUBSCRIBE_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void sendText(String text) {
  String clientId = "ESP32-";
  clientId += String(random(0xffff), HEX);
  if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD)) {
    client.publish(MQTT_PUBLISH_TOPIC, text.c_str());
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
  }
}
