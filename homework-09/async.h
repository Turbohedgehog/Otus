#pragma once

#include <cstdint>

namespace homework_09 {

uint32_t connect(uint32_t block_size = 3);
void receive(const char* buffer, uint32_t buffer_size, uint32_t context_id);
void disconnect(uint32_t context_id);

}  // namespace homework_09
