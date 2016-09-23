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
int32_t readTemperature = 0;
extern "C" {
void setSetPoint( int32_t temp)
{
    setPoint = temp;
    readTemperature = temp;
}

//3 5 6 9 10 11
void setDim1(int32_t d){ analogWrite(5, d); }
void setDim2(int32_t d){ analogWrite(9, d); }
void setDim3(int32_t d){ analogWrite(11, d); }
void setDim4(int32_t d){ analogWrite(10, d); }
void setDim5(int32_t d){ analogWrite(6, d); }
void setDim6(int32_t d){ analogWrite(3, d); }

//ANA_OUT("setPoint", "C", "0", "300", 0, 300, setSetPoint ,setPointTemperature);
ANA_OUT("Dim650nm", "%", "0", "100", 0, 255, setDim1 ,dim1Widget);
ANA_OUT("Dim625nm", "%", "0", "100", 0, 255, setDim2 ,dim2Widget);
ANA_OUT("Dim465nm", "%", "0", "100", 0, 255, setDim3 ,dim3Widget);
ANA_OUT("Dim445nm", "%", "0", "100", 0, 255, setDim4 ,dim4Widget);
ANA_OUT("Dim425nm", "%", "0", "100", 0, 255, setDim5 ,dim5Widget);
ANA_OUT("Dim360nm", "%", "0", "100", 0, 255, setDim6 ,dim6Widget);
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
//        &temperature,
	&initcmd, &last
};




}
void setup()
{
  pinMode(ch1TempPin, INPUT);
  Serial.begin(115200);
  setDim1(0);	
  setDim2(0);	
  setDim3(0);	
  setDim4(0);	
  setDim5(0);	
  setDim6(0);	
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

void loop()
{
  //transmitAnalogIn(&temperature, (int) (readTemperature));
  commandLine();
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
 