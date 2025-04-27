/*
 Navicat Premium Data Transfer

 Source Server         : livermore-qt
 Source Server Type    : SQLite
 Source Server Version : 3035005
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005
 File Encoding         : 65001

 Date: 27/04/2025 14:31:21
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for tick
-- ----------------------------
CREATE TABLE IF NOT EXISTS "tick" (
  "instrument_id" TEXT,
  "instrument_name" TEXT,
  "exchange_id" TEXT,
  "trading_day" TEXT,
  "action_time" TEXT,
  "last_price" NUMBER,
  "pre_close_price" NUMBER,
  "open_price" NUMBER,
  "pre_settlement_price" NUMBER,
  "highest_price" NUMBER,
  "lowest_price" NUMBER,
  "close_price" NUMBER,
  "settlement_price" NUMBER,
  "upper_limit_price" NUMBER,
  "lower_limit_price" NUMBER,
  "average_price" NUMBER,
  "bid_price1" NUMBER,
  "ask_price1" NUMBER,
  "bid_price2" NUMBER,
  "ask_price2" NUMBER,
  "bid_price3" NUMBER,
  "ask_price3" NUMBER,
  "bid_price4" NUMBER,
  "ask_price4" NUMBER,
  "bid_price5" NUMBER,
  "ask_price5" NUMBER,
  "bid_volume1" NUMBER,
  "ask_volume1" NUMBER,
  "bid_volume2" NUMBER,
  "ask_volume2" NUMBER,
  "bid_volume3" NUMBER,
  "ask_volume3" NUMBER,
  "bid_volume4" NUMBER,
  "ask_volume4" NUMBER,
  "bid_volume5" NUMBER,
  "ask_volume5" NUMBER,
  "volume" NUMBER,
  "pre_open_interest" NUMBER,
  "open_interest" NUMBER,
  "turnover" NUMBER,
  "action_ms" NUMBER
);

PRAGMA foreign_keys = true;
