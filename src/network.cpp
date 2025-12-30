#include "network.h"
#include "config.h"
#include "globals.h"
#include <WiFi.h>
unsigned long lastSignalUpdate = 0;

void publishCommandStatus(const char* message) {
  mqtt_client.publish(mqtt_command_status_topic, message);
  Serial.println(message);
}

void connectWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_PIN, HIGH);
    delay(250);
    digitalWrite(LED_PIN, LOW);
    delay(250);
    Serial.print(".");
  }
  
  Serial.println("\n✓ WiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void connectMQTT() {
  while (!mqtt_client.connected()) {
    Serial.print("Connecting to MQTT broker: ");
    Serial.println(mqtt_broker);
    
    if (mqtt_client.connect(mqtt_client_id)) {
      Serial.println("✓ MQTT Connected!");
      
      mqtt_client.subscribe(mqtt_topic);
      mqtt_client.subscribe(mqtt_led2_topic);
      mqtt_client.subscribe(mqtt_led4_topic);
      mqtt_client.subscribe(mqtt_emergency_light_topic);
      
      mqtt_client.publish(mqtt_status_topic, "OFF");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void checkNetwork() {
  // ... your existing connection logic ...
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }
  if (!mqtt_client.connected()) {
    connectMQTT();
  }
  mqtt_client.loop();

  // --- ADD THIS BLOCK ---
  // Check signal strength every 5 seconds
  unsigned long now = millis();
  if (now - lastSignalUpdate > SIGNAL_UPDATE_INTERVAL) {
    lastSignalUpdate = now;
    
    // Read signal strength (RSSI)
    long rssi = WiFi.RSSI();
    
    // Publish to topic: chami/esp32/stats/signal
    mqtt_client.publish("chami/esp32/stats/signal", String(rssi).c_str());
  }
  // ---------------------
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  Serial.print("Msg on ["); Serial.print(topic); Serial.print("]: "); Serial.println(message);

  // --- LED 1 CONTROL ---
  if (String(topic) == mqtt_topic) {
    if (message == "ON" || message == "1") {
      digitalWrite(LED_PIN, HIGH);
      mqtt_client.publish(mqtt_status_topic, "ON");
    } else if (message == "OFF" || message == "0") {
      digitalWrite(LED_PIN, LOW);
      mqtt_client.publish(mqtt_status_topic, "OFF");
    }
  }
  // --- LED 2 (GPIO13) CONTROL ---
  else if (String(topic) == mqtt_led2_topic) {
    if (message == "ON" || message == "1") {
      digitalWrite(LED2_PIN, LOW);
    } else if (message == "OFF" || message == "0") {
      digitalWrite(LED2_PIN, HIGH);
      delay(1000); // Pulse logic
      if (digitalRead(LED4_PIN) == HIGH) {
         // Double Pulse logic
         digitalWrite(LED4_PIN, LOW); delayMicroseconds(10); digitalWrite(LED4_PIN, HIGH);
         delay(100);
         digitalWrite(LED4_PIN, LOW); delayMicroseconds(10); digitalWrite(LED4_PIN, HIGH);
      } else {
         // Single Pulse
         digitalWrite(LED4_PIN, HIGH); delayMicroseconds(10); digitalWrite(LED4_PIN, LOW);
      }
    } else if (message == "PULSE") {
      digitalWrite(LED2_PIN, HIGH); delay(2000); digitalWrite(LED2_PIN, LOW);
    }
  }
  // --- LED 4 (GPIO14) CONTROL ---
  else if (String(topic) == mqtt_led4_topic) {
    if (message == "ON" || message == "1" || message == "OFF" || message == "0") {
      digitalWrite(LED4_PIN, LOW); delayMicroseconds(10); digitalWrite(LED4_PIN, HIGH);
    } else if (message == "PULSE") {
      digitalWrite(LED4_PIN, HIGH); delay(2000); digitalWrite(LED4_PIN, LOW);
    }
  }
  // --- EMERGENCY LIGHT CONTROL ---
  else if (String(topic) == mqtt_emergency_light_topic) {
    manualEmergencyControl = true;
    if (message == "ON" || message == "1") {
      digitalWrite(POWER_STATUS_PIN, LOW);
      mqtt_client.publish(mqtt_emergency_light_status_topic, "ON");
    } else if (message == "OFF" || message == "0") {
      digitalWrite(POWER_STATUS_PIN, HIGH);
      mqtt_client.publish(mqtt_emergency_light_status_topic, "OFF");
    } else if (message == "AUTO") {
      manualEmergencyControl = false;
      mqtt_client.publish(mqtt_emergency_light_status_topic, "AUTO");
    }
  }
}