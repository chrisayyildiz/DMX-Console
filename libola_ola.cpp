
#include "libola_ola.h"

Libola_Ola::Libola_Ola()
{
    //m_Universe = 1;
    m_Buffer.Blackout();

    // Init logging
    ola::InitLogging(ola::OLA_LOG_WARN, ola::OLA_LOG_STDERR);
}

int Libola_Ola::write_dmx(unsigned int universe, unsigned char data[], unsigned int dataLength)
{
    ola::client::StreamingClient ola_client((ola::client::StreamingClient::Options()));

    if(!ola_client.Setup())
        return 0;

    m_Buffer.Blackout();

    uint8_t u8Array [512];
    for(int i = 0;i < 512; ++i)
    {
        u8Array[i] = (uint8_t) data[i];
    }

    m_Buffer.Set(u8Array, 512);

    if(!ola_client.SendDmx(universe, m_Buffer))
    {
        return 0;
    }
}
