// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: t -*-
//
//  DIYDrones Custom Mediatek GPS driver for ArduPilot and ArduPilotMega.
//	Code by Michael Smith, Jordi Munoz and Jose Julio, DIYDrones.com
//
//	This library is free software; you can redistribute it and / or
//	modify it under the terms of the GNU Lesser General Public
//	License as published by the Free Software Foundation; either
//	version 2.1 of the License, or (at your option) any later version.
//
//	GPS configuration : Custom protocol per "DIYDrones Custom Binary Sentence Specification V1.1"
//
// Note - see AP_GPS_MTK16.h for firmware 1.6 and later.
//
#ifndef __GPS_MTK_H__
#define __GPS_MTK_H__

#include "GPS_MTK_Common.h"
#include <Stream.h>

class GPS_MTK {
public:
	GPS_MTK(Stream *s, bool parse);
	void init();
	bool poll();
	void getData(bool *fix, uint32_t *itime, uint32_t *idate, int32_t *ilatitude, int32_t *ilongitude, int32_t *ialtitude, uint32_t *iground_speed, int32_t *iground_course, int16_t *ihdop, uint8_t *inum_sats);
	uint8_t* getDataBuffer();
private:
	Stream *port;   ///< port the GPS is attached to
	bool parse;
	bool fix;
	uint32_t time;                      ///< GPS time (milliseconds from epoch)
	uint32_t date;                      ///< GPS date (FORMAT TBD)
	int32_t latitude;                   ///< latitude in degrees * 10,000,000
	int32_t longitude;                  ///< longitude in degrees * 10,000,000
	int32_t altitude;                   ///< altitude in cm
	uint32_t ground_speed;      ///< ground speed in cm/sec
	int32_t ground_course;      ///< ground course in 100ths of a degree
	int16_t hdop;                       ///< horizontal dilution of precision in cm
	uint8_t num_sats;           ///< Number of visible satelites

	// Packet checksum accumulators
	uint8_t ck_a;
	uint8_t ck_b;

	// State machine state
	uint8_t step;
	uint8_t payload_counter;

	struct diyd_mtk_msg {
		int32_t latitude; //4 - 4
		int32_t longitude; //4 - 8
		int32_t altitude; //4 - 12
		int32_t ground_speed; //4 - 16
		int32_t ground_course; //4 - 20
		uint8_t satellites; //1 - 21
		uint8_t fix_type; //1 - 22
		uint32_t utc_date; //4 - 26
		uint32_t utc_time; //4 - 30
		uint16_t hdop; //2 - 32
	};
	enum diyd_mtk_fix_type {
		FIX_NONE = 1, FIX_2D = 2, FIX_3D = 3, FIX_3D_SBAS = 7
	};

	enum diyd_mtk_protocol_bytes {
		PREAMBLE1 = 0xd0, PREAMBLE2 = 0xdd,
	};
	// Receive buffer
	union {
		diyd_mtk_msg msg;
		uint8_t bytes[sizeof(diyd_mtk_msg)];
	} buffer;
};

#endif  // __AP_GPS_MTK_H__
