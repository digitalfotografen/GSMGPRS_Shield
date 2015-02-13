#ifndef _HTTPCLIENT_SIM900_H_
#define _HTTPCLIENT_SIM900_H_

#include "GSM.h"
#include "SIM900.h"
#define HTTP_TIMEOUT 30000
#include <MemoryFree.h>
enum HttpClient_retval {
  ERROR,
  NO_GPRS,
  NO_CONNECTION,
  CONNECTING,
  CONNECTED,
  CLOSING,
  CLOSED
};

class HttpClientSim900 {
  public:
    HttpClientSim900();
    HttpClient_retval begin(byte cid = 0, 
              const char *apn = NULL,
              const char *user = NULL,
              const char *pwd = NULL);
    boolean saveBearer(byte cid = 0);
    //byte init();
    int httpGet(const char *url, 
                boolean allowRedirect = false);
    int httpPost(const char *url, 
                const char *data = NULL,
                boolean allowRedirect = false);
    int readResponse(char *buff, int start = 0, int length = 100);
    int readRow(char *buff, int length = 100);
    int getResponseCode(){ return _responseCode; }
    int getResponseLength(){ return _responseLength; }
    boolean terminate();
    boolean close();

  protected:
    boolean httpRequest(const char *url, 
                boolean allowRedirect = false);
    int httpAction(int method = 0);
    byte _cid;
    int _responseCode;
    int _responseLength;
    int _pos;
};

#endif