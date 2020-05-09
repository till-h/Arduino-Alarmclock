#include "ClockConfig.h"
#include "DS3231.h"
#include <time.h>



void get2Digits(char * arrayFrom, char * arrayTo, uint8_t startIndex);
//int numericalMonth(char * lexicalMonth);

// Clock
DS3231 clk;

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println("Setting clock to system time at preprocessing...");
    Serial.println(__DATE__);
    Serial.println(__TIME__);
  
    // Set clock with system time at compilation
    char d[12];
    char t[9];
    char Y[3];
    char M[4];
    char D[3];
    char h[3];
    char m[3];
    char s[3];
    strcpy(d, __DATE__);
    strcpy(t, __TIME__);
    getChars(d, Y, 9, 2);
    getChars(d, M, 0, 3);
    getChars(d, D, 4, 2);
    getChars(t, h, 0, 2);
    getChars(t, m, 3, 2);
    getChars(t, s, 6, 2);
    aTime time;
    time.yyyy = atoi(Y);
    time.mm = 1;//numericalMonth(M);
    time.dd = atoi(D);
    time.dw = (time.dd += time.mm < 3 ? time.yyyy--: time.yyyy - 2, 23 * time.mm / 9 + time.dd + 4 + time.yyyy / 4 - time.yyyy / 100 + time.yyyy / 400) % 7 + 1;
    time.h = atoi(h);
    time.m = atoi(m)+1; // hack to compensate for upload time (does not work around the full hour!)
    time.s = atoi(s);
    clk.setTime(time);
    clk.printTime();
}

void loop() {
    delayMicroseconds(10000);
}

void getChars(char * arrayFrom, char * arrayTo, uint8_t startIndex, uint8_t numDigits)
{
    for (uint8_t i = 0; i < numDigits; i++)
    {
        *(arrayTo + i) = arrayFrom[startIndex + i];
    }
    *(arrayTo + numDigits) = '\0';
}

//int numericalMonth(char * lexicalMonth)
//{
//    char * lexicalMonths[12] = {"Jan","Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
//    if (strcmp(lexicalMonth, lexicalMonths[0]) == 0) return 1;
//    else if (strcmp(lexicalMonth, lexicalMonths[1]) == 0) return 2;
//    else if (strcmp(lexicalMonth, lexicalMonths[2]) == 0) return 3;
//    else if (strcmp(lexicalMonth, lexicalMonths[3]) == 0) return 4;
//    else if (strcmp(lexicalMonth, lexicalMonths[4]) == 0) return 5;
//    else if (strcmp(lexicalMonth, lexicalMonths[5]) == 0) return 6;
//    else if (strcmp(lexicalMonth, lexicalMonths[6]) == 0) return 7;
//    else if (strcmp(lexicalMonth, lexicalMonths[7]) == 0) return 8;
//    else if (strcmp(lexicalMonth, lexicalMonths[8]) == 0) return 9;
//    else if (strcmp(lexicalMonth, lexicalMonths[9]) == 0) return 10;
//    else if (strcmp(lexicalMonth, lexicalMonths[10]) == 0) return 11;
//    else if (strcmp(lexicalMonth, lexicalMonths[11]) == 0) return 12;
//
//}
