#ifndef SERIAL_H
#define SERIAL_H

#include <cstdint>
#include <string>

#include "cave_talk.h"

namespace serial_comms
{

void Start(const std::string &port, const uint32_t baudrate);
void Stop(void);
void SetPort(const std::string &port);
void SetBaudrate(const uint32_t baudrate);
CaveTalk_Error_t Send(const void *const data, const size_t size);
CaveTalk_Error_t Receive(void *const data, const size_t size, size_t *const bytes_received);

} // namespace serial_comms

#endif // SERIAL_H