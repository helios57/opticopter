/*
 * Logging.h
 *
 *  Created on: Dec 2, 2013
 *      Author: helios
 */

#ifndef LOGGING_H_
#define LOGGING_H_
#include <Arduino.h>

namespace opticopter {

	class Logging {
	private:
		static const int preamble = 0xDDE5E7;
		static const int postamble = 0xEF475E;
	public:
		struct Entry {
			int preeamble;
			long timestamp;
			short accelX;
			short accelY;
			short accelZ;
			float gyroX;
			float gyroY;
			float gyroZ;
			float magX;
			float magY;
			float magZ;
			float roll;
			float pitch;
			float yaw;
			float rollLevel;
			float pitchLevel;
			float yawLevel;
			float rollPid;
			float pitchPid;
			float yawPid;
			short output0;
			short output1;
			short output2;
			short output3;
			int postamble;
		};
		union EntryConverter {
			Entry entry;
			uint8_t bytes[sizeof(Entry)];
		};
		static EntryConverter entryConverter;

		Entry* getEntry() {
			return &entryConverter.entry;
		}

		uint8_t* getBytes() {
			return entryConverter.bytes;
		}

		void setPreAndPostAbmle() {
			entryConverter.entry.preeamble = preamble;
			entryConverter.entry.postamble = postamble;
		}
		bool checkPreAndPostAbmle() {
			return entryConverter.entry.preeamble == preamble && entryConverter.entry.postamble == postamble;
		}
		Logging() {
		}
		virtual ~Logging() {
		}
	};

} /* namespace opticopter */
#endif /* LOGGING_H_ */
