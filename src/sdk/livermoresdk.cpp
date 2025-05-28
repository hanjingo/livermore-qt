#include "livermoresdk.h"

#include <QDebug>
#include <QString>

#include "util.h"

#include "handler.h"

void livermore_sdk_init(context ctx)
{
	if (!checkContext(&ctx))
	{
		qCritical() << "INVALID CONTEXT WHILE CALL livermore_sdk_init";
		return;
	}
	
	qDebug() << "livermore_sdk_init";
	if (ctx.user_data == NULL)
		return;

	auto param = static_cast<param_init_sdk*>(ctx.user_data);
	QString version = QString(param->version);
	param->result = Handler::instance()->init();

	if (ctx.cb == NULL)
		return;

	ctx.cb((void*)param);
}

void livermore_sdk_dial_broker(context ctx)
{
	if (!checkContext(&ctx))
	{
		qCritical() << "INVALID CONTEXT WHILE CALL livermore_sdk_dial_broker";
		return;
	}

	qDebug() << "livermore_sdk_init";
	if (ctx.user_data == NULL)
		return;

	auto param = static_cast<param_dial_broker*>(ctx.user_data);
	QString ip = QString(param->ip);
	param->result = Handler::instance()->dialBroker(ip, param->port);
	if (ctx.cb == NULL)
		return;

	ctx.cb(ctx.user_data);
}

void livermore_sdk_close_broker(context ctx)
{
	if (!checkContext(&ctx))
	{
		qCritical() << "INVALID CONTEXT WHILE CALL livermore_sdk_close_broker";
		return;
	}

	qDebug() << "livermore_sdk_close_broker";
	if (ctx.user_data == NULL)
		return;

	auto param = static_cast<param_close_broker*>(ctx.user_data);
	QString ip = QString(param->ip);
	param->result = Handler::instance()->closeBroker(ip, param->port);
	if (ctx.cb == NULL)
		return;

	ctx.cb(ctx.user_data);
}

void livermore_sdk_sub_market_data(context ctx)
{
	if (!checkContext(&ctx))
	{
		qCritical() << "INVALID CONTEXT WHILE CALL livermore_sdk_sub_market_data";
		return;
	}

	qDebug() << "livermore_sdk_sub_market_data";
	if (ctx.user_data == NULL)
		return;
}

void livermore_sdk_quit(context ctx)
{
	if (!checkContext(&ctx))
	{
		qCritical() << "INVALID CONTEXT WHILE CALL livermore_sdk_quit";
		return;
	}

	qDebug() << "livermore_sdk_quit";
	if (ctx.user_data == NULL)
		return;

	auto param = static_cast<param_quit*>(ctx.user_data);
	param->result = Handler::instance()->quit();
	if (ctx.cb == NULL)
		return;

	ctx.cb(ctx.user_data);
}