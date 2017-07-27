#pragma once

#include <string>
#include "bitstream.h"

void deflate(char *data, size_t size, Bitstream *bstream);
void create_gzip(std::string src, std::string dist);
