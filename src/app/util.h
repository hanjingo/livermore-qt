#ifndef UTIL_H
#define UTIL_H

#include <QString>

extern "C" {
#include "livermoresdk.h"
#include "market_data.h"
}

std::string fmtMarketData(const market_data& md);

#endif
