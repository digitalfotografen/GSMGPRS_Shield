#ifndef gprsclient_h
#define gprsclient_h
#include "Print.h"
#include "Client.h"
#include "IPAddress.h"

class GPRSClient : public Client {

public:
	GPRSClient();
  GPRSClient(uint8_t sock);

	uint8_t status();
	
	// the following methods comes from Client.h
  virtual int connect(IPAddress ip, uint16_t port);
  virtual int connect(const char *host, uint16_t port);
  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *buf, size_t size);
  virtual int available();
  virtual int read();
  virtual int read(uint8_t *buf, size_t size);
  virtual int peek();
  virtual void flush();
  virtual void stop();
  virtual uint8_t connected();
  virtual operator bool();
  virtual bool operator==(const GPRSClient&);
  virtual bool operator!=(const GPRSClient& rhs) { return !this->operator==(rhs); };

  using Print::write;
};

#endif