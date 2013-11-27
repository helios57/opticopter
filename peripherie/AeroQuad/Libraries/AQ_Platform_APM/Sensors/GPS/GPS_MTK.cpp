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

#include <stdint.h>
#include "GPS_MTK.h"
GPS_MTK::GPS_MTK(Stream *port, bool parse) :
		port(port), parse(parse) {
	step = 0;
	payload_counter = 0;
	ck_b = ck_a = 0;
	fix = 0;
	time = 0;
	date = 0;
	latitude = 0;
	longitude = 0;
	altitude = 0;
	ground_speed = 0;
	ground_course = 0;
	hdop = 0;
	num_sats = 0;
}
// Public Methods //////////////////////////////////////////////////////////////
void GPS_MTK::init() {
	port->flush();
	// initialize serial port for binary protocol use
	port->print(MTK_SET_BINARY);

	// set 5Hz update rate
	port->print(MTK_OUTPUT_10HZ);

	// set SBAS on
	port->print(SBAS_ON);

	// set WAAS on
	port->print(WAAS_ON);

	// Set Nav Threshold to 0 m/s
	port->print(MTK_NAVTHRES_OFF);
}

void GPS_MTK::getData(bool *ifix, uint32_t *itime, uint32_t *idate, int32_t *ilatitude, int32_t *ilongitude, int32_t *ialtitude, uint32_t *iground_speed, int32_t *iground_course, int16_t *ihdop, uint8_t *inum_sats) {
	ifix = &fix;
	itime = &time;
	idate = &date;
	ilatitude = &latitude;
	ilongitude = &longitude;
	ialtitude = &altitude;
	iground_speed = &ground_speed;
	iground_course = &ground_course;
	ihdop = &hdop;
	inum_sats = &num_sats;
}
uint8_t* GPS_MTK::getDataBuffer() {
	return buffer.bytes;
}

// Process bytes available from the stream
//
// The stream is assumed to contain only our custom message.  If it
// contains other messages, and those messages contain the preamble bytes,
// it is possible for this code to become de-synchronised.  Without
// buffering the entire message and re-processing it from the top,
// this is unavoidable.
//
// The lack of a standard header length field makes it impossible to skip
// unrecognised messages.
//
bool GPS_MTK::poll() {
	uint8_t data;
	int16_t numc;
	bool parsed = false;

	numc = port->available();
	for (int16_t i = 0; i < numc; i++) {        // Process bytes received

		// read the next byte
		data = port->read();

		restart: switch (step) {
		case 0:
			if (PREAMBLE1 == data)
				step++;
			break;
		case 1:
			if (PREAMBLE2 == data) {
				step++;
				break;
			}
			step = 0;
			goto restart;
		case 2:
			if (sizeof(buffer) == data) {
				step++;
				ck_b = ck_a = data; // reset the checksum accumulators
				payload_counter = 0;
			} else {
				step = 0; // reset and wait for a message of the right class
				goto restart;
			}
			break;

			// Receive message data
		case 3:
			buffer.bytes[payload_counter++] = data;
			ck_b += (ck_a += data);
			if (payload_counter == sizeof(buffer))
				step++;
			break;

			// Checksum and message processing
		case 4:
			step++;
			if (ck_a != data) {
				step = 0;
			}
			break;
		case 5:
			step = 0;
			if (ck_b != data) {
				break;
			}

			if (parse) {
				fix = ((buffer.msg.fix_type == FIX_3D) || (buffer.msg.fix_type == FIX_3D_SBAS));
				latitude = buffer.msg.latitude * 10;
				longitude = buffer.msg.longitude * 10;
				altitude = buffer.msg.altitude;
				ground_speed = buffer.msg.ground_speed;
				ground_course = buffer.msg.ground_course;
				num_sats = buffer.msg.satellites;
				date = buffer.msg.utc_date;
				time = buffer.msg.utc_time;
				hdop = buffer.msg.hdop;
			}
			parsed = true;
		}
	}
	return parsed;
}
