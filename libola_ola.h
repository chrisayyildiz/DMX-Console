#ifndef LIBOLA_OLA_H
#define LIBOLA_OLA_H

#include <ola/DmxBuffer.h>
#include <ola/Logging.h>
#include <ola/client/StreamingClient.h>

class Libola_Ola
{
public:
    Libola_Ola();
    int write_dmx(unsigned int universe, unsigned char data[], unsigned int dataLength = 512);

private:
    //unsigned int m_Universe;
    ola::DmxBuffer m_Buffer;
};

#endif // LIBOLA_OLA_H
