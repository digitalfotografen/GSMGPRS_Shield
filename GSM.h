#ifndef gsm_h
#define gsm_h
#include <Arduino.h>
#include <SoftwareSerial.h>

#define GSM_LIB_VERSION F("v0.1_Serial1")

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
     #define USE_HW_SERIAL
#endif


#define RESPONSE_BUFFER_SIZE 40 // enough to get modem response
#define TIMEOUT 1000 //Default time out, 1 second

enum NetworkStatus_t { 
	ERROR, 
	IDLE, 
	CONNECTING, 
	GSM_READY,
	GPRS_FAILED_SINGLE_CONNECT,
	GPRS_NO_TRANSPARENT_MODE,
	GPRS_FAILED_START_GPRS,
	GPRS_FAILED_GPRS_CONNECT,
	GPRS_FAILED_NO_IP,
	GPRS_ACTIVE,
	GPRS_DEACTIVATED,
	GPRS_READY,
	CONNECTED,
	DEVICE_OFF
};

enum RegistrationStatus_t {
  NOT_REGISTRED_NOT_SEARCHING,
  REGISTRED_HOME,
  NOT_REGISTRED_SEARCHING,
  REGISTRATION_DENIED,
  UNKNOWN,
  ROAMING  
};


class GSM {

protected:
#ifndef USE_HW_SERIAL
  SoftwareSerial _softwareSerial;
  SoftwareSerial *_cell;
#else
  HardwareSerial *_cell;
#endif
	char _internalBuffer[RESPONSE_BUFFER_SIZE];
	char* _buffer = _internalBuffer;
	int _bufferIndex = 0;
	int _bufferSize;
	NetworkStatus_t _status;

	bool readAndCheckResponse(const char* expected, int readBeyond=-1, int timeout=TIMEOUT);
	char* readAndGetValue(const char* expected, int timeout=TIMEOUT);
	bool successfulResponse(int timeout=TIMEOUT);
	void readToClear();
	
public:
	GSM();

	NetworkStatus_t begin(char* pin=0, bool restart=true);
	NetworkStatus_t getNetworkStatus(void);
	void setBuffer(char * buffer, int size);
	void send(char * buffer);
	
	virtual bool shutdown()=0;
	virtual char* getIMEI(char* imei, int length)=0;
  virtual int getSIMStatus(void)=0;

  virtual int sendSMS(char* number, char* data)=0;
  virtual int readSMS(int messageIndex, char *message, int length)=0;
  virtual int deleteSMS(int index)=0;

  virtual int callUp(char* number)=0;
  virtual int answer(void)=0;
	virtual char getPhoneNumber(byte position, char *phone_number)=0;
	virtual char writePhoneNumber(byte position, char *phone_number)=0;
	virtual char deletePhoneNumber(byte position)=0;
	
	friend class GPRSClient;
	friend class SIM900Client;
};

#endif