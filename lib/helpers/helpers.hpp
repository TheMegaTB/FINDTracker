#ifndef FIND_HELPERS_H
#define FIND_HELPERS_H

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
extern "C" {
    #include <ESP8266WiFi.h>
}
#include <vector>
#include <Ticker.h>

#include <NTPClient.h>
#include <ESP8266WiFi.h>

#include "defaults.hpp"

#define NTP_OFFSET   60 * 60      // In seconds
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "europe.pool.ntp.org"


void blink();
void blinkSync();
void OTA();

int vasprintf(char** strp, const char* fmt, va_list ap, int* size);

#define MAX_SRV_CLIENTS 5

namespace Terminal {
    void begin(int baudrate);
    void handle();

    void printTime();

    void print(char* str, int len);
    void print(String str);

    void println(char* str, int len);
    void println(String str);

    void printf(char* str, ...);
    void printf(String str, ...);
};

template<typename T>
class CircularBuffer {
    int size;
    int index;
    int pushCount;

public:
    std::vector<T> buffer;
    void push(T element) {
        (this->buffer)[index] = element;

        ++(this->pushCount);

        if (this->index == size - 1) this->index = 0;
        else ++(this->index);
    }

    void clear() {
        this->buffer.clear();
    }

    std::vector<T> get() {
        std::vector<T> elements;
        int cursor;

        elements.reserve(this->pushCount);
        cursor = this->index;
        do {
            if (!((this->pushCount) < (this->size) && cursor >= (this->pushCount)))
                elements.push_back((this->buffer)[cursor]);

            if (cursor >= (this->size - 1)) cursor = 0;
            else cursor++;
        } while(cursor != index);

        return elements;
    }

    CircularBuffer(int size) : buffer(size) {
        this->size = size;
        this->buffer.reserve(size);
        this->pushCount = 0;
        this->index = 0;
    }
};


#endif
