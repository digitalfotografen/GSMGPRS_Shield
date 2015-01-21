#ifndef sim900gprs_h
#define sim900gprs_h
#include "GPRS.h"

#define SIM900_GPRS_VERSION F("v0.1_Serial")

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
     #define USE_HW_SERIAL
#endif

#define GSM_RX_PIN            7
#define GSM_TX_PIN            8
#define GSM_ON_PIN            9 // connect GSM Module turn ON to pin 77 
#define GSM_RESET_PIN         -1 // default undefined

enum ConnectionStatus_t { 
	IP_INITIAL, 
	IP_START, 
	IP_CONFIG, 
	IP_GPRSACT,
	IP_STATUS,
	CONNECTING_LISTENING,
	CONNECT_OK,
	CLOSING,
	CLOSED,
	PDP_DEACT,
	UNKNOWN_GSM_STATUS
};

// this hardware implements both GSM and GPRS functionality, thus inherits from GPRS
class SIM900GPRS : public GPRS {
protected:
  int _gprsOnPin = GSM_ON_PIN;
  int _gprsResetPin = GSM_RESET_PIN;
  ConnectionStatus_t parseConnectionStatus(char * str);
  ConnectionStatus_t getConnectionStatus();
  //void cyclePowerOnOff();
	
public:
#ifndef USE_HW_SERIAL
	SIM900GPRS(long baudrate=9600, 
	            int gprsBoardRXPin=7, 
	            int gprsBoardTXPin=8,
	            int gprsOnPin=GSM_ON_PIN,
	            int gprsResetPin=GSM_RESET_PIN);
#else
	SIM900GPRS(long baudrate=9600,
	            int gprsOnPin = GSM_ON_PIN,
	            int gprsResetPin = GSM_RESET_PIN);
#endif

	virtual NetworkStatus_t begin(char* pin=NULL, bool restart=true);
	
	bool isGPRSAvailable();
	NetworkStatus_t attachGPRS(const char * const domain, const char * const username=NULL, const char * const  password=NULL);
	void detachGPRS();

	char* getIMEI(char* imei, int length);
	char* getIP(char* ip, int length);
	
	bool turnOn();
	bool shutdown();
  void reset();
  void cyclePowerOnOff();
  	
  RegistrationStatus_t getRegistrationStatus();
  bool registerToNetwork();

	int getSignalStrength();
	
	void activateGPRS();
	void deactivateGPRS();
	
	// Functions below not yet implemented
	int getSIMStatus(void);

  int sendSMS(char* number, char* data);
  int readSMS(int messageIndex, char *message, int length);
  int deleteSMS(int index);

  int callUp(char* number);
  int answer(void);
	char getPhoneNumber(byte position, char *phone_number);
	char writePhoneNumber(byte position, char *phone_number);
	char deletePhoneNumber(byte position);
	
	friend class SIM900Client;
	friend class SIM900Server;
};

#endif