#include <EEPROM.h>

extern "C"
{
#include "uart_command_lib.h"
#include "corbomite.h"
}


int ch1TempPin = A0;
int chargePumpPin = 3;
int heaterPin = 7;
int heatLed = 13;
int setPoint = 0;

uint8_t ledSetting1 = 0;
uint8_t ledSetting2 = 0;
uint8_t ledSetting3 = 0;
uint8_t ledSetting4 = 0;
uint8_t ledSetting5 = 0;
uint8_t ledSetting6 = 0;
bool inited = false;

int32_t readTemperature = 0;
extern "C" {
void setSetPoint( int32_t temp)
{
    setPoint = temp;
    readTemperature = temp;
}

//3 5 6 9 10 11
void setDim1(int32_t d){ analogWrite(5, d); ledSetting1=d; }
void setDim2(int32_t d){ analogWrite(9, d); ledSetting2=d; }
void setDim3(int32_t d){ analogWrite(11, d); ledSetting3=d; }
void setDim4(int32_t d){ analogWrite(10, d); ledSetting4=d; }
void setDim5(int32_t d){ analogWrite(6, d); ledSetting5=d; }
void setDim6(int32_t d){ analogWrite(3, d); ledSetting6=d; }

void save(){
	EEPROM.write(1, ledSetting1);
	EEPROM.write(2, ledSetting2);
	EEPROM.write(3, ledSetting3);
	EEPROM.write(4, ledSetting4);
	EEPROM.write(5, ledSetting5);
	EEPROM.write(6, ledSetting6);
}

void load(){
	setDim1(EEPROM.read(1));
	setDim2(EEPROM.read(2));
	setDim3(EEPROM.read(3));
	setDim4(EEPROM.read(4));
	setDim5(EEPROM.read(5));
	setDim6(EEPROM.read(6));
}

//ANA_OUT("setPoint", "C", "0", "300", 0, 300, setSetPoint ,setPointTemperature);
ANA_OUT("Dim650nm", "%", "0", "100", 0, 255, setDim1 ,dim1Widget);
ANA_OUT("Dim625nm", "%", "0", "100", 0, 255, setDim2 ,dim2Widget);
ANA_OUT("Dim465nm", "%", "0", "100", 0, 255, setDim3 ,dim3Widget);
ANA_OUT("Dim445nm", "%", "0", "100", 0, 255, setDim4 ,dim4Widget);
ANA_OUT("Dim425nm", "%", "0", "100", 0, 255, setDim5 ,dim5Widget);
ANA_OUT("Dim360nm", "%", "0", "100", 0, 255, setDim6 ,dim6Widget);
EVENT_OUT("Save", save, saveWidget);
EVENT_OUT("Load", load, loadWidget);

//ANA_IN("temperature", "C", "0", "450", 0, 300, temperature);
const CorbomiteEntry last PROGMEM = {LASTTYPE, "", 0};
const EventData initEvent PROGMEM = {registeredEntries};

const CorbomiteEntry initcmd PROGMEM = 
	{EVENT_OUT, internalId, (CorbomiteData*)&initEvent};

const CorbomiteEntry * const entries[] PROGMEM = {
	//&setPointTemperature,
	&dim1Widget,
	&dim2Widget,
	&dim3Widget,
	&dim4Widget,
	&dim5Widget,
	&dim6Widget,
	&saveWidget,
	&loadWidget,
//        &temperature,
	&initcmd, &last
};

} //extern "C"

void resetDallas(int pin){

	pinMode(pin, OUTPUT);
	digitalWrite(pin,0);
	delayMicroseconds(500);
	pinMode(pin, INPUT);
}

void initDallas(int pin)
{
	resetDallas(pin);
	delayMicroseconds(500); //ignore precens pulses for now
}

void writeBit(int pin, uint8_t bit)
{
	noInterrupts();
	pinMode(pin, OUTPUT);
	if(bit == 0){
		delayMicroseconds(65);
		pinMode(pin, INPUT);
	        delayMicroseconds(5);
		
	} else {
		delayMicroseconds(10);
		pinMode(pin, INPUT);
	        delayMicroseconds(55);
	}
	interrupts();
}

void writeByte(int pin, uint8_t data)
{
	for( int i = 0 ; i < 8 ; i++){
		writeBit(pin, data & 0x01);
		data = data >> 1;
	}
}


uint8_t readBit(int pin)
{
	uint8_t b;
	noInterrupts();
	pinMode(pin, OUTPUT);
	delayMicroseconds(3);
	pinMode(pin, INPUT);
	delayMicroseconds(10);
	b = digitalRead(pin);
	interrupts();
	return b == HIGH ? 0xFF : 0;
	delayMicroseconds(53);
}

uint8_t readByte(int pin)
{
	uint8_t data;
	for( int i = 0 ; i < 8 ; i++){
		data = (data>>1) |(readBit(pin)&+0x80);
	}
	return data;
}

void setup()
{
  pinMode(ch1TempPin, INPUT);
  Serial.begin(115200);
  //setDim1(0);	
  //setDim2(0);	
  //setDim3(0);	
  //setDim4(0);	
  //setDim5(0);	
  //setDim6(0);	
  //analogReference(INTERNAL);
  //analogWrite(chargePumpPin, 127);
  //pinMode(heatLed, OUTPUT);
  //pinMode(ch1TempPin, INPUT);
  //pinMode(heaterPin, OUTPUT);

}


void printValue(char *name, float value)
{
  Serial.print(name);
  if (value > 0)
   Serial.print("+");
  Serial.print(value);
}


int readDallas(int pin){
	static uint8_t state = 0;
	static uint32_t t0 = 0;
	static uint8_t r = 0;
	static uint16_t t = 0;
	static uint16_t tr = 0;
	switch(state){
		case 0:
			initDallas(pin);
			state = 1;
			break;
		case 1:
			writeByte(pin, 0xCC);
			state = 2;
			break;
		case 2:
			writeByte(pin, 0x44);
			state = 3;
			t0 = millis();
			break;
		case 3:
			if (millis() > t0+800){
				initDallas(pin);
				state = 4;
			}
			break;
		case 4:
			writeByte(pin, 0xCC);
			state = 5;
			break;
		case 5:
			writeByte(pin, 0xBE);
			state = 6;
			r = 0;
			break;
		case 6:
			if(r == 0){
				t = readByte(pin);
			} else if(r == 1) {
				tr = t | readByte(pin)<<8;
			} else {
				readByte(pin);
			}
			if(r==9){
				state = 0;
				break;	
			}
			r++;
			break;
			 
		default:
			state = 0;
	}
	return tr;
}


void loop()
{
  Serial.print(float(readDallas(7))/16.0);
  Serial.println("DERP");
  commandLine();
  if(not inited){
	  load();
	  inited = true;
  }
}

void platformSerialWrite(const char *buf, uint16_t len)
{
    Serial.write((uint8_t *)buf, len);
}

void serialEvent()
{
    while(Serial.available()){
        addCharToBuffer(Serial.read());
    }
}
 
