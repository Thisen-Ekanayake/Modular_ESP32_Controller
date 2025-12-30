#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// --- PIN DEFINITIONS ---
#define LED_PIN 2             
#define LED2_PIN 13           
#define LED4_PIN 14           
#define POWER_STATUS_PIN 27   
#define INTENSITY_B0_PIN 32   
#define INTENSITY_B1_PIN 33   
#define INTENSITY_B2_PIN 35   

// --- WIFI CREDENTIALS ---
extern const char* ssid;
extern const char* password;

// --- MQTT SETTINGS ---
extern const char* mqtt_broker;
extern const int mqtt_port;
extern const char* mqtt_client_id;

// --- MQTT TOPICS ---
extern const char* mqtt_topic;
extern const char* mqtt_status_topic;
extern const char* mqtt_led2_topic;
extern const char* mqtt_led2_status_topic;
extern const char* mqtt_led4_topic;
extern const char* mqtt_led4_status_topic;
extern const char* mqtt_sensor_voltage_topic;
extern const char* mqtt_sensor_current_topic;
extern const char* mqtt_sensor2_voltage_topic;
extern const char* mqtt_sensor2_current_topic;
extern const char* mqtt_powercut_topic;
extern const char* mqtt_command_status_topic;
extern const char* mqtt_emergency_light_topic;
extern const char* mqtt_emergency_light_status_topic;
extern const char* mqtt_powercut_history_topic;
extern const char* mqtt_light_intensity_topic;

// --- CONSTANTS ---
const float POWER_CUT_THRESHOLD = 1.0; 
const unsigned long EMERGENCY_DURATION = 60000; 
const unsigned long GPIO14_DELAY = 200;
const long SIGNAL_UPDATE_INTERVAL = 5000;

#endif