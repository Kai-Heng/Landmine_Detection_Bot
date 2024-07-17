

/**
 * main.c
 */
#include "ping.h"
#include "servo.h"
#include "Timer.h"
#include "lcd.h"
#include "adc.h"
#include "uart.h"
#include <stdint.h>
#include <stdbool.h>
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#include "Movement.h"
#include <math.h>
#include "bumperMovement.h"
#include "button.h"
#include "music.h"


#define PI 3.14159265359

void main()
{
    timer_init();
    lcd_init();
    adc_init();
    uart_init();
    ping_sensor_init();
    servo_init();

    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data); // should turn the iRobot Create's "Dirt Detect" LED to blue

    int i;
    int ran0 = 0;
    int loop = 1;
    int object1[10];
    int angle1[10];
    int distance1[10];
    int width1[10];

    while(loop)
    {
        int command = uart_receive();
//        char com = '\0';
//        scanf("%c", &com);
        int f = ping_read()/2;
        double time = f*6.25*(pow(10,-8)); //6.25*pow10,-8
        int ping_dist = 34000*time;
////        lcd_clear();
////
        if(command == 'w'){
            oi_setWheels(100, 100);
            printf("%s","Move forward");
            oi_moveForward(sensor_data, 10);
            uart_sendStr("end");
            oi_setWheels(0, 0);
        }

        else if(command == 's'){
            oi_setWheels(-100,-100);
            oi_moveBackward(sensor_data, -5);
            uart_sendStr("end");
            lcd_printf("%s", "Move Backward");
            oi_setWheels(0, 0);

        }
        else if(command == 'a'){
            oi_setWheels(50, -50);
            oi_turnCounterClockwise(sensor_data, 10);
            uart_sendStr("end");
            lcd_printf("%s", "Turn left");
            oi_setWheels(0, 0);

        }
        else if(command == 'd'){
            oi_setWheels(-50, 50);
            oi_turnClockwise(sensor_data, -10);
            uart_sendStr("end");
            lcd_printf("%s", "Turn right");
            oi_setWheels(0, 0);

        }
        else if(command == 'n'){
            int i, j;
            int x = 0;
            int k = 1;

            char partTwoF[] = "Degrees";
            char partTwoF1[] = "Distance (cm)";
            char partTwoF2[] = "IR Distance";

            char format[] = "Object#";
            char format1[] = "Angle";
            char format2[] = "Distance";
            char format3[] = "Width";
            int min = 0;
            int minAngle = 0;
            int minAngleDist = 0;
            int minActualWidth = 0;
            int flag = 0;

            servo_move(0);
            timer_waitMillis(1000);


//            for(i = 0; i<strlen(format); i++){
//                uart_sendChar(format[i]);
//            }
//
//            for(i = 0; i<4; i++){
//                uart_sendChar(' ');
//            }
//
//            for(i = 0; i<strlen(format1); i++){
//                uart_sendChar(format1[i]);
//            }
//
//            for(i = 0; i<8; i++){
//                uart_sendChar(' ');
//            }
//
//            for(i = 0; i<strlen(format2); i++){
//                uart_sendChar(format2[i]);
//            }
//
//            for(i = 0; i<4; i++){
//                uart_sendChar(' ');
//            }
//
//            for(i = 0; i<strlen(format3); i++){
//                uart_sendChar(format3[i]);
//            }
//
//            uart_sendChar('\r');
//            uart_sendChar('\n');

            //Object detection
            for(i = 0; i<=180; i++){
                if(uart_event == 1 && uart_typed == 't'){
                    minAngle = i;
                    flag = 1;
                    ran0 = 0;
                    break;
                }
                servo_move(i);
                f = ping_read()/2;
                time = f*6.25*(pow(10,-8)); //6.25*pow10,-8
                ping_dist = 34000*time;
//                timer_waitMillis(50);
                int soundDist = 0;
                int rawVal = 0;
                int begin = 0;
                int end = 0;
                int count = 0;
                int width = 0;
                int area = 0;
//                if(i == 90){
//                    uart_sendChar('|');
//                }
//                else{
//                    uart_sendChar('*');
//                }


                if(adc_read() >= 700){
                    begin = i;
                    soundDist = ping_dist;
                    rawVal = adc_read();
//                    if(j == 90){
//                        uart_sendChar('|');
//                    }
//                    else{
//                        uart_sendChar('-');
//                    }
                    for(j = i+1; j<=180; j++){
                        count++;
                        servo_move(j);
//                        timer_waitMillis(50);
//                        if(j == 90){
//                            uart_sendChar('|');
//                        }
//                        else{
//                            uart_sendChar('-');
//                        }

                        if(adc_read() <700 || j==180){
                            end = j;
                            width = end - begin;
                            i = i + count;

                            char obj[3];
                            char deg[3];
                            char dist[3];
                            char wid[3];

                            char tempStr[20] = "\0";

                            if(width > 2){
//                                object1[x] = k;
//                                angle1[x] = begin;
//                                distance1[x] = soundDist;
//                                width1[x] = width;
//                                x++;
//                                k++;
//                                sprintf(obj, "%d", k);
//                                for(x = 0; x<strlen(obj); x++){
//                                    uart_sendChar(obj[x]);
//                                }
//                                sprintf(tempStr, "Object %d: %d, %dcm ", k, rawVal, soundDist);
//
//                                lcd_puts(tempStr);
//                                k++;
//
//                                for(x = 0; x<11; x++){
//                                    uart_sendChar(' ');
//                                }
//
//                                sprintf(deg, "%d", begin);
//                                for(x = 0; x<strlen(deg); x++){
//                                    uart_sendChar(deg[x]);
//                                }
//
//                                for(x = 0; x<13; x++){
//                                    uart_sendChar(' ');
//                                }
//
//
//
//                                sprintf(dist, "%d cm", soundDist);
//                                for(x=0; x<strlen(dist); x++){
//                                    uart_sendChar(dist[x]);
//                                }
//
//                                for(x = 0; x<11; x++){
//                                    uart_sendChar(' ');
//                                }
//
//                                sprintf(wid, "%d", width);
//                                for(x = 0; x<strlen(wid); x++){
//                                    uart_sendChar(wid[x]);
//                                }
//
//                                uart_sendChar('\r');
//                                uart_sendChar('\n');



                                minActualWidth = soundDist*sin((width/2)*(PI/180));

                                minAngle = begin + (width/2);
                                float radius = (double) minActualWidth/2;
                                area = PI*pow(radius, 2);
                                char data[100];
                                sprintf(data, "%d, %d, %d, %d", minAngle, soundDist, minActualWidth, area);
                                uart_sendStr(data);

                                if(min == 0){
                                    min = minActualWidth;
                                    minAngle = begin + (width/2);
                                    minAngleDist = soundDist;
                                }
                                else if(min>minActualWidth){
                                    min = minActualWidth;
                                    minAngle = begin + (width/2);
                                    minAngleDist = soundDist;
                                }
                            }
                            break;
                        }

                    }

                }
                if(i==180){
                    break;
                }

//            }
//            servo_move(minAngle);
////            float converter = scan.sound_dist/scan.IR_raw_val;
////            int cm = scan.IR_raw_val*converter;
//            f = ping_read()/2;
//            time = f*6.25*(pow(10,-8)); //6.25*pow10,-8
//            ping_dist = 34000*time;
//            lcd_printf("%d", minAngleDist);
//            char skinObjFormat[] = "The skinniest object angle: ";
//            uart_sendChar('\r');
//            uart_sendChar('\n');
//            uart_sendStr(skinObjFormat);
//            char minAngleArr[3];
//            sprintf(minAngleArr, "%d", minAngle);
//
//            for(i = 0; i< strlen(minAngleArr); i++){
//                uart_sendChar(minAngleArr[i]);
//            }
//
//            uart_sendChar('\r');
//            uart_sendChar('\n');

//            while(i<10){
//                if(object1[i]>100 && object1[i]<0){
//                    break;
//                }
//
//                i++;
//            }
//
////            if(flag != 1){
////                int turnToAngle = minAngle - 90;
////                if(turnToAngle < 0){
////                    oi_setWheels(-50, 50);
////                    oi_turnClockwise(sensor_data, turnToAngle/1.5);
////                    oi_setWheels(0,0);
////                }
////                else if(turnToAngle > 0){
////                    oi_setWheels(50, -50);
////                    oi_turnCounterClockwise(sensor_data, turnToAngle/1.5);
////                    oi_setWheels(0,0);
////                }
////
////                servo_move(90);
////                timer_waitMillis(1);
////                oi_setWheels(100,100);
////                oi_moveForward(sensor_data, (minAngleDist - 12));
////                oi_setWheels(0,0);
////            }
////            ran0 = 35;
//
        }
            timer_waitMillis(5);
            uart_sendStr("end, end, end, ");
//
//        else if(command == 'm'){
//            int i, j, x;
//            int k = 1;
//
//            char partTwoF[] = "Degrees";
//            char partTwoF1[] = "Distance (cm)";
//            char partTwoF2[] = "IR Distance";
//
//            servo_move(0);
//            timer_waitMillis(1000);
//
//
//            for(i = 0; i<strlen(partTwoF); i++){
//                uart_sendChar(partTwoF[i]);
//            }
//
//            for(j = 0; j<8; j++){
//                uart_sendChar(' ');
//            }
//
//            for(i = 0; i<strlen(partTwoF1); i++){
//                uart_sendChar(partTwoF1[i]);
//            }
//
//            for(j = 0; j<8; j++){
//                uart_sendChar(' ');
//            }
//
//            for(i = 0; i<strlen(partTwoF2); i++){
//                uart_sendChar(partTwoF2[i]);
//            }
//            uart_sendChar('\r');
//            uart_sendChar('\n');
//
//            for(i = 0; i<=180; i++){
//                if(uart_event == 1 & uart_typed == 't'){
//                    break;
//                }
//                servo_move(i);
//                f = ping_read()/2;
//                time = f*6.25*(pow(10,-8)); //6.25*pow10,-8
//                ping_dist = 34000*time;
//                timer_waitMillis(200);
//                char c[3];
//                char c1[3];
//                char c2[3];
//
//                sprintf(c, "%d", i);
//                for(j = 0; j<strlen(c); j++){
//                    uart_sendChar(c[j]);
//                }
//
//                for(j = 0; j<14; j++){
//                    uart_sendChar(' ');
//                }
//
//                sprintf(c1, "%d", ping_dist);
//                for(j = 0; j<strlen(c1); j++){
//                    uart_sendChar(c1[j]);
//                }
//
//                for(j = 0; j<14; j++){
//                    uart_sendChar(' ');
//                }
//
//                sprintf(c2, "%d", adc_read());
//                for(j = 0; j<strlen(c2); j++){
//                    uart_sendChar(c2[j]);
//                }
//                uart_sendChar('\r');
//                uart_sendChar('\n');
//            }
        }
//
//        else if(command == 'b'){
//            int i, j, x;
//            int k = 1;
//            int angle[2];
//            int minWid[2];
//            int minDist[2];
//
//            char partTwoF[] = "Degrees";
//            char partTwoF1[] = "Distance (cm)";
//            char partTwoF2[] = "IR Distance";
//
//            char format[] = "Object#";
//            char format1[] = "Angle";
//            char format2[] = "Distance";
//            char format3[] = "Width";
//            int min = 0;
//            int minAngle = 0;
//            int minAngleDist = 0;
//            int minActualWidth = 0;
//            servo_move(0);
//
//            for(i = 0; i<strlen(format); i++){
//                uart_sendChar(format[i]);
//            }
//
//            for(i = 0; i<4; i++){
//                uart_sendChar(' ');
//            }
//
//            for(i = 0; i<strlen(format1); i++){
//                uart_sendChar(format1[i]);
//            }
//
//            for(i = 0; i<8; i++){
//                uart_sendChar(' ');
//            }
//
//            for(i = 0; i<strlen(format2); i++){
//                uart_sendChar(format2[i]);
//            }
//
//            for(i = 0; i<4; i++){
//                uart_sendChar(' ');
//            }
//
//            for(i = 0; i<strlen(format3); i++){
//                uart_sendChar(format3[i]);
//            }
//
//            uart_sendChar('\r');
//            uart_sendChar('\n');
//
//            //Object detection
////            for(k = 0; k<2; k++){
//                for(i = 0; i<=180; i++){
//                    servo_move(i);
//                    f = ping_read()/2;
//                    time = f*6.25*(pow(10,-8)); //6.25*pow10,-8
//                    ping_dist = 34000*time;
//                    timer_waitMillis(200);
//                    float soundDist = 0;
//                    int rawVal = 0;
//                    int begin = 0;
//                    int end = 0;
//                    int count = 0;
//                    int width = 0;
//
//
//                    if(adc_read() >= 150){
//                        begin = i;
//                        soundDist = ping_dist;
//                        rawVal = adc_read();
//                        for(j = i+1; j<=180; j++){
//                            count++;
//                            servo_move(j);
//                            timer_waitMillis(200);
//                            if(adc_read() <150){
//                                end = j;
//                                width = end - begin;
//                                i = i + count;
//
//                                float converter = soundDist/rawVal;
//                                int cm = rawVal*converter;
//                                char obj[3];
//                                char deg[3];
//                                char dist[3];
//                                char wid[3];
//
//                                char tempStr[20] = "\0";
//
//                                if(width > 4){
//                                    sprintf(obj, "%d", k);
//                                    for(x = 0; x<strlen(obj); x++){
//                                        uart_sendChar(obj[x]);
//                                    }
//                                    sprintf(tempStr, "Object %d: %d, %dcm ", k, rawVal, cm);
//
//                                    lcd_puts(tempStr);
//                                    k++;
//
//                                    for(x = 0; x<11; x++){
//                                        uart_sendChar(' ');
//                                    }
//
//                                    sprintf(deg, "%d", begin);
//                                    for(x = 0; x<strlen(deg); x++){
//                                        uart_sendChar(deg[x]);
//                                    }
//
//                                    for(x = 0; x<13; x++){
//                                        uart_sendChar(' ');
//                                    }
//
//
//
//                                    sprintf(dist, "%d cm", cm);
//                                    for(x=0; x<strlen(dist); x++){
//                                        uart_sendChar(dist[x]);
//                                    }
//
//                                    for(x = 0; x<11; x++){
//                                        uart_sendChar(' ');
//                                    }
//
//                                    sprintf(wid, "%d", width);
//                                    for(x = 0; x<strlen(wid); x++){
//                                        uart_sendChar(wid[x]);
//                                    }
//
//                                    uart_sendChar('\r');
//                                    uart_sendChar('\n');
//
//
//                                    minActualWidth = soundDist*sin((width/2)*(PI/180));
//
//                                    if(min == 0){
//                                        min = minActualWidth;
//                                        minAngle = begin + (width/2);
//                                        minAngleDist = soundDist;
//                                    }
//                                    else if(min>minActualWidth){
//                                        min = minActualWidth;
//                                        minAngle = begin + (width/2);
//                                        minAngleDist = soundDist;
//                                    }
//                                }
//
//                                break;
//                            }
//                            if(j == 180)
//                                break;
//                        }
//                        angle[0] = minAngle;
//                        minWid[0] = min;
//                        minDist[0] = minAngleDist;
//                    }
//                }
//                oi_setWheels(50, -50);
//                oi_turnCounterClockwise(sensor_data, 176);
//                oi_setWheels(0,0);
//
//                for(i = 0; i<=180; i++){
//                    servo_move(i);
//                    f = ping_read()/2;
//                    time = f*6.25*(pow(10,-8)); //6.25*pow10,-8
//                    ping_dist = 34000*time;
//                    timer_waitMillis(200);
//                    float soundDist = 0;
//                    int rawVal = 0;
//                    int begin = 0;
//                    int end = 0;
//                    int count = 0;
//                    int width = 0;
//
//
//                    if(adc_read() >= 150){
//                        begin = i;
//                        soundDist = ping_dist;
//                        rawVal = adc_read();
//                        for(j = i+1; j<=180; j++){
//                            count++;
//                            servo_move(j);
//                            timer_waitMillis(200);
//                            if(adc_read() <150){
//                                end = j;
//                                width = end - begin;
//                                i = i + count;
//
//                                float converter = soundDist/rawVal;
//                                int cm = rawVal*converter;
//                                char obj[3];
//                                char deg[3];
//                                char dist[3];
//                                char wid[3];
//
//                                char tempStr[20] = "\0";
//
//                                if(width > 4){
//                                    sprintf(obj, "%d", k);
//                                    for(x = 0; x<strlen(obj); x++){
//                                        uart_sendChar(obj[x]);
//                                    }
//                                    sprintf(tempStr, "Object %d: %d, %dcm ", k, rawVal, cm);
//
//                                    lcd_puts(tempStr);
//                                    k++;
//
//                                    for(x = 0; x<11; x++){
//                                        uart_sendChar(' ');
//                                    }
//
//                                    sprintf(deg, "%d", begin);
//                                    for(x = 0; x<strlen(deg); x++){
//                                        uart_sendChar(deg[x]);
//                                    }
//
//                                    for(x = 0; x<13; x++){
//                                        uart_sendChar(' ');
//                                    }
//
//
//
//                                    sprintf(dist, "%d cm", cm);
//                                    for(x=0; x<strlen(dist); x++){
//                                        uart_sendChar(dist[x]);
//                                    }
//
//                                    for(x = 0; x<11; x++){
//                                        uart_sendChar(' ');
//                                    }
//
//                                    sprintf(wid, "%d", width);
//                                    for(x = 0; x<strlen(wid); x++){
//                                        uart_sendChar(wid[x]);
//                                    }
//
//                                    uart_sendChar('\r');
//                                    uart_sendChar('\n');
//
//
//                                    minActualWidth = soundDist*sin((width/2)*(PI/180));
//
//                                    if(min == 0){
//                                        min = minActualWidth;
//                                        minAngle = begin + (width/2);
//                                        minAngleDist = soundDist;
//                                    }
//                                    else if(min>minActualWidth){
//                                        min = minActualWidth;
//                                        minAngle = begin + (width/2);
//                                        minAngleDist = soundDist;
//                                    }
//                                }
//
//                                break;
//                            }
//                        }
//                        angle[1] = minAngle;
//                        minWid[1] = min;
//                        minDist[1] = minAngleDist;
//                    }
//                }
//            }
//
////            if(minWid[0]<= minWid[1]){
////                oi_setWheels(50, -50);
////                oi_turnCounterClockwise(sensor_data, 176);
////                oi_setWheels(0,0);
////
////                minAngle = angle[0];
////                minAngleDist = minDist[0];
////
////
////            }
////            else{
////                minAngle = angle[1];
////                minAngleDist = minDist[1];
////            }
////            cyBOT_Scan(minAngle, &scan);
//////            float converter = scan.sound_dist/scan.IR_raw_val;
//////            int cm = scan.IR_raw_val*converter;
////            lcd_printf("%d", minAngleDist);
////            char skinObjFormat[] = "The skinniest object angle: ";
////            for(i = 0; i< strlen(skinObjFormat); i++){
////                uart_sendChar(skinObjFormat[i]);
////            }
////            char minAngleArr[3];
////            sprintf(minAngleArr, "%d", minAngle);
////
////            for(i = 0; i< strlen(minAngleArr); i++){
////                uart_sendChar(minAngleArr[i]);
////            }
////            int turnToAngle = minAngle - 90;
////            if(turnToAngle < 0){
////                oi_setWheels(-25, 25);
////                oi_turnClockwise(sensor_data, turnToAngle/1.765);
////                oi_setWheels(0,0);
////            }
////            else if(turnToAngle > 0){
////                oi_setWheels(25, -25);
////                oi_turnCounterClockwise(sensor_data, turnToAngle/1.765);
////                oi_setWheels(0,0);
////            }
////            cyBOT_Scan(90, &scan);
////            timer_waitMillis(1);
////            oi_setWheels(100,100);
////            oi_moveForward(sensor_data, (minAngleDist - 12));
////            oi_setWheels(0,0);
////
////        }

        else if(command == 'v'){
            unsigned char ImperialMarchNumNotes = 19;
            unsigned char ImperialMarchNotes[19] = {55, 55, 55, 51, 58, 55, 51, 58, 55, 0,  62, 62, 62, 63, 58, 54, 51, 58, 55};
            unsigned char ImperialMarchDurations[19] = {32, 32, 32, 20, 12, 32, 20, 12, 32, 32, 32, 32, 32, 20, 12, 32, 20, 12, 32};
            oi_loadSong(IMERPIAL_MARCH, ImperialMarchNumNotes, ImperialMarchNotes, ImperialMarchDurations);
            oi_play_song(IMERPIAL_MARCH);
            uart_sendStr("------VICTORY!----");
            timer_waitMillis(100);
            uart_sendStr("end");
        }

        else{
            loop = 0;
        }
    }

    oi_free(sensor_data);
}


