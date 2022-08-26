#include <mcp_can.h>
#include <SPI.h>

#define ID_CD30MP3  0x00000206

#define VOL_P       0x00089301
#define VOL_M       0x000893FF
#define NEXT_T      0x00019100
#define PREV_T      0x00019200

#define AUX_FM      0x00018200
#define NEXT_FM     0x00018100
#define NEXT_F      0x000883FF
#define PREV_F      0x00088301
#define PRES_FREQ   0x00018400

#define DELAY_CLICK_BUTTON  400
#define DELAY_PRESS_BUTTON  800
#define PIN_BUTTON_1        9
#define PIN_BUTTON_2        8
#define PIN_BUTTON_3        7

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
int rx_code = 0;

#define CAN0_INT A0         // Set INT to pin 2
MCP_CAN CAN0(10);           // Set CS to pin 10


void setup() {
    pinMode(PIN_BUTTON_1,OUTPUT);
    digitalWrite(PIN_BUTTON_1,LOW);

    pinMode(PIN_BUTTON_2,OUTPUT);
    digitalWrite(PIN_BUTTON_2,LOW);

    pinMode(PIN_BUTTON_3,OUTPUT);
    digitalWrite(PIN_BUTTON_3,LOW);
    //
    Serial.begin(9600);
    //MS-CAN
    CAN0.begin(MCP_ANY, CAN_95K2BPS, MCP_8MHZ);
    
    if(CAN0.begin(MCP_ANY, CAN_95K2BPS, MCP_8MHZ) == CAN_OK)  
      Serial.println("MCP2515 Initialized Successfully!");
    else
      Serial.println("Error Initializing MCP2515...");
    
    CAN0.setMode(MCP_LISTENONLY);
    pinMode(CAN0_INT, INPUT);
    Serial.println("MCP2515 Library Receive Example...");
}

void loop() {
    // If CAN0_INT pin is low, read receive buffer
    if(!digitalRead(CAN0_INT)) { 
        // Read data: len = data length, buf = data byte(s)
        CAN0.readMsgBuf(&rxId, &len, rxBuf);
        if((rxId == ID_CD30MP3) & (len == 0x03)) {
            rx_code = 0;
            rx_code = rx_code << 8 | rxBuf[0];  //0
            rx_code = rx_code << 8 | rxBuf[1];  //1
            rx_code = rx_code << 8 | rxBuf[2];  //2
            switch(rx_code) {
                case VOL_P:
                    Serial.println("Volume plus");
                    break;
                case VOL_M:    
                    Serial.println("Volume minus");
                    break;
                case NEXT_T:
                    Serial.println("Next track");
                    click_button(PIN_BUTTON_1);
                    break;
                case PREV_T:   
                    Serial.println("Previous track");
                    click_button(PIN_BUTTON_3);
                    break;
                case AUX_FM:  
                    Serial.println("AUX/FM"); 
                    break;
                case NEXT_FM: 
                    Serial.println("Next FM");
                    click_button(PIN_BUTTON_2);
                    break;
                case NEXT_F:
                    Serial.println("Next frequency"); 
                    break;                
                case PREV_F:
                    Serial.println("Previous frequency");    
                    break;
                case PRES_FREQ:
                    Serial.println("Pressure frequency");
                    break;
                default:
                    Serial.print("Undefined code: ");
                    Serial.println(rx_code,HEX);
                    break;
            }
        }
    }
}

void click_button(byte pin_name) {
    digitalWrite(pin_name,HIGH);
    delay(DELAY_CLICK_BUTTON);
    digitalWrite(pin_name,LOW);
}

void press_button(byte pin_name) {
    digitalWrite(pin_name,HIGH);
    delay(DELAY_PRESS_BUTTON);
    digitalWrite(pin_name,LOW);
}
      
