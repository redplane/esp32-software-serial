// On ESP8266:
// At 80MHz runs up 57600ps, and at 160MHz CPU frequency up to 115200bps with only negligible errors.
// Connect pin 12 to 14.

#include <SoftwareSerial.h>
#include <WiFi.h>
#include <ModbusRTU.h>

#ifdef ESP32
#define BAUD_RATE 9600
#else
#define BAUD_RATE 9600
#endif

SoftwareSerial swSer;


#if defined(ESP8266)
#include <SoftwareSerial.h>
// SoftwareSerial S(D1, D2, false, 256);

// receivePin, transmitPin, inverse_logic, bufSize, isrBufSize
// connect RX to D2 (GPIO4, Arduino pin 4), TX to D1 (GPIO5, Arduino pin 4)
SoftwareSerial S(4, 5);
#elif defined(ESP32)

#endif

ModbusRTU mb;

bool cbWrite(Modbus::ResultCode event, uint16_t transactionId, void *data) {
#ifdef ESP8266
    Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
#elif ESP32
    Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
#else
    Serial.print("Request result: 0x");
  Serial.print(event, HEX);
#endif

    Serial.print("Data size: ");
    Serial.println((uint16_t) sizeof(data));

    return true;
}

void setup() {

    Serial.begin(115200);
    swSer.begin(BAUD_RATE, SWSERIAL_8N1, 2, 0, false, 95, 11);

    mb.begin(&swSer);
    mb.slave(1);
    mb.removeHreg(0, 10);

    for (auto i = 0; i < 10; i++) {
        mb.addHreg(i, i + 10, 1);
    }

//    mb.master();
    Serial.println("Started modbus rtu");
}

uint16_t holdingRegisters[20];

void loop() {
//    if (!mb.slave()) {
//        mb.readHreg(1, 1, holdingRegisters, 20, cbWrite);
//    }
    mb.task();
    yield();
}