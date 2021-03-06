/*
 * ArducopterNg.h
 *
 *  Created on: Jun 7, 2013
 *      Author: helios
 */

#ifndef OPTICOPTER_H_
#define OPTICOPTER_H_

#include "../Libraries/Hal/HalApm.h"
#include "Serializer/Serializer.h"
#include "Util/DebugStream.h"
#include "DataModel.h"
#include "Persistence/Persistence.h"
#include "Logging/Logging.h"

namespace opticopter {

	class OptiCopter {
	private:
		HalApm *hal;
		Serializer *serializer;
		DebugStream *debug;
		DataModel *dm;
		Persistence *persistence;
		Logging *logging;
		bool sendData;
		unsigned long t_sendData;
		unsigned long t_10ms;
		unsigned long t_20ms;
		unsigned long t_50ms;
		unsigned long lastMicros;
		unsigned long lastAndroidMs;
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
		int16_t buffer_16t[3];
		uint16_t buffer_u16t[8];
		uint8_t buffer_u8t[8];
		float buffer_float[9];
		uint8_t commandBuffer[64];
		int16_t axyzgxyz[6];
		float androidInput[3];

		void sendMotion6();
		void sendGyro();
		void sendQuat();
		void sendBaro();
		void sendMag();
		void sendGPS();
		void resetEmptyCycles();
		void sendInput();
		void handleInSendData();
		void handleInReadParam();
		void handleInWriteData();
		void handleIn(uint8_t id);

	public:
		OptiCopter();
		~OptiCopter();
		int main();
		void setup();
		void loop();
		void sendLoggedData();
		void readAndroidInput();
	};

} /* namespace arducopterNg */
#endif /* ARDUCOPTERNG_H_ */
