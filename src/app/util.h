#ifndef UTIL_H
#define UTIL_H

#include <QString>

#include "livermoresdk.h"
#include "market_data.h"

QString errWhat(err);

std::string fmtMarketData(const market_data& md);

#endif
