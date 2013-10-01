/*
 * ArducopterNg.h
 *
 *  Created on: Jun 7, 2013
 *      Author: helios
 */

#ifndef ARDUCOPTERNG_H_
#define ARDUCOPTERNG_H_

#include "../Libraries/Hal/HalApm.h"
#include "Serializer/Serializer.h"
#include "Util/DebugStream.h"
#include "DataModel.h"
#include "Persistence/Persistence.h"

namespace arducopterNg {

	class ArducopterNg {
	private:
		HalApm *hal;
		Serializer *serializer;
		DebugStream *debug;
		DataModel *dm;
		Persistence *persistence;
		bool sendData;
		bool newData;
		unsigned long emptyCycles;
		unsigned long t_1000ms;
		unsigned long t_sendData;
		unsigned long t_5ms;
		unsigned long t_10ms;
		unsigned long t_20ms;
		union {
			uint64_t ulong;
			uint8_t byte[8];
		} conv8;
		union {
			uint32_t uint;
			int32_t sint;
			uint8_t byte[4];
			float floating;
		} conv4;
		union {
			uint16_t uint;
			int16_t sint;
			uint8_t byte[2];
		} conv2;
		int32_t buffer[4];
		int16_t buffer_16t[3];
		uint16_t buffer_u16t[8];
		uint8_t buffer_u8t[8];
		float buffer_float[3];
		uint8_t commandBuffer[64];

		void sendAccel();
		void sendGyro();
		void sendQuat();
		void sendBaro();
		void sendMag();
		void sendGPS();
		void resetEmptyCycles();
		void sendInput();

	public:
		ArducopterNg();
		~ArducopterNg();
		int main();
		void setup();
		void loop();
	};

} /* namespace arducopterNg */
#endif /* ARDUCOPTERNG_H_ */
