#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <WiFi.h>           // <--- Critical Fix
#include <PubSubClient.h>
#include <Wire.h>
#include "INA3221.h"

// --- SHARED OBJECTS ---
extern WiFiClient espClient;
extern PubSubClient mqtt_client;
extern INA3221 INA;

// --- SHARED STATE VARIABLES ---
extern bool powerCutDetected;
extern bool emergencyModeActive;
extern bool manualEmergencyControl;
extern bool gpio14Activated;

extern float currentLightIntensity;
extern float totalEnergyConsumed;
extern float startVoltage;
extern float endVoltage;

// --- SHARED SENSOR VALUES ---
extern float shared_v1; // Main Voltage
extern float shared_c1; // Main Current
extern float shared_p1; // Main Power
extern float shared_v2; // System Voltage
extern float shared_c2; // System Current
extern float shared_p2; // System Power
extern float shared_intensity; // Light Intensity

extern unsigned long emergencyStartTime;
extern unsigned long gpio14ActivationTime;
extern unsigned long powerCutStartTime;
extern unsigned long lastEnergyCalcTime;

#endif