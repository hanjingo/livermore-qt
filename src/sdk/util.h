#ifndef UTIL_H
#define UTIL_H

#include <QDataStream>
#include <QByteArray>
#include <QJsonObject>

#include "market_data.h"
#include "livermoresdk.h"

#define __cat(a, b) a##b
#define _cat(a, b) __mux_cat(a, b)

void md_to_json(market_data* md, QJsonObject& js);
void json_to_md(QJsonObject& js, market_data* md);

bool checkContext(context* ctx);

#endif
