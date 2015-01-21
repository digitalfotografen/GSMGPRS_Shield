#include "GSM.h"
#include <string.h>

#define GSM_RXPIN 7
#define GSM_TXPIN 8


#ifndef USE_HW_SERIAL
GSM::GSM():_softwareSerial(GSM_TXPIN, GSM_RXPIN) {
  _cell = &_softwareSerial;
#else
  GSM::GSM() {
  _cell = &Serial1;
#endif
  _bufferSize = RESPONSE_BUFFER_SIZE;
  _status = CONNECTING;
}

void GSM::setBuffer(char * buffer, int size) {
	_buffer = buffer;
	_bufferSize = size;
}

void GSM::send(char * str) {
	_cell->println(str);
}


void print_P(const char *str)
{
  uint8_t val;
  while (true) {
    val=pgm_read_byte(str);
    if (!val) break;
    Serial.write(val);
    str++;
  }
}

//#define DEBUG

/**
 * Parameters: 
 * expected - PSTR expected response str
 * readBeyond - number of bytes to continue to read after 'expected' is found or -1 to read all available data
 * response timeout in ms
 */
bool GSM::readAndCheckResponse(const char* expected, int readBeyond, int timeout) {
#ifdef DEBUG
	Serial.print(F("Check for '"));
	print_P(expected);
	Serial.print(F("' and read beyond "));
	Serial.println(readBeyond);
#endif
	bool found = false;
	long _endTime = millis()+timeout;		
	_bufferIndex = 0;
	
	// keep reading while not timeout AND 
	// buffer is not full AND 
	// while not found or readBeyound is not zero
	while((millis() < _endTime)  &&
	 			(_bufferIndex < _bufferSize) && 
				(!found || (readBeyond != 0))) {
		if((_cell->available()>0)) {
			_buffer[_bufferIndex++] = _cell->read();
			_buffer[_bufferIndex] = 0;
			if(found && (readBeyond != -1)) {
				readBeyond--;
			}
			if(!found && (NULL != strstr_P(_buffer, expected))) {
				// expected found
				found = true;
#ifdef DEBUG
				Serial.print(F("Found at "));
				Serial.print(_bufferIndex);
				Serial.print(F(": will "));
				if(-1 == readBeyond) {
					// read all available bytes
					Serial.println(F("read all."));
				} else {
					// read readBeyond number or bytes
					Serial.print(F("read "));
					Serial.print(readBeyond);
					Serial.println(F(" bytes."));
				}
#endif
			}
		}
	}

#ifdef DEBUG
	if(found) {
		Serial.print(F("Found '"));
		print_P(expected);
		Serial.print(F("' in '"));
		Serial.print(_buffer);
		Serial.println(F("'"));
	} else {
		Serial.print(F("Timeout after "));
		Serial.print(timeout);
		Serial.println(F("ms."));
		Serial.print(F("Did not find '"));
		print_P(expected);
		Serial.print(F("' in '"));
		Serial.print(_buffer);
		Serial.println(F("'"));
	}
#endif
	return found;
}

char* GSM::readAndGetValue(const char* expected, int timeout)
{
  long endTime = millis()+timeout;
  char* ptr = NULL;
  boolean eol = false;

  if (readAndCheckResponse(expected, 0, timeout)){
    ptr = _buffer + _bufferIndex;
    while ((millis() < endTime) && (_bufferIndex < _bufferSize) && (!eol)){
      if(_cell->available()){
         _buffer[_bufferIndex] = _cell->read();
         if(_buffer[_bufferIndex] == '/r'){
            eol = true;
            _buffer[_bufferIndex] = 0x00;
            
         } else {
            _buffer[++_bufferIndex] = 0x00;
         }
      }
    }
  } else {
    ptr = NULL; //not found
#ifdef DEBUG
    Serial.print("Value of ");
    print_P(expected);
    Serial.println(" not found!");
#endif
  }
  
/*
  if (endTime > millis()) {
    ptr = NULL; //timeout
#ifdef DEBUG
    Serial.print("Value of ");
    print_P(expected);
    Serial.println(" timout!");
#endif
  }
*/  
#ifdef DEBUG
  Serial.print(ptr);
  Serial.print("Value of ");
  print_P(expected);
  Serial.print(" is :");
  Serial.println(ptr);
#endif
  return ptr;
}

bool GSM::successfulResponse(int timeout) 
{
	return readAndCheckResponse(PSTR("OK\r\n"), -1, timeout);
}

void GSM::readToClear(){
  while(_cell->available()){
    Serial.print(_cell->read());
  }
}


NetworkStatus_t GSM::getNetworkStatus(void) {
	return _status;
}