#ifndef SERIALSERVICES__H
#define SERIALSERVICES__H

class SerialServices
{
    public:

    SerialServices();
    ~SerialServices();

    void CheckSerialClient(char *buffer);
    void SendResponse(char* response);

    bool isSerialRequest;
};


#endif

