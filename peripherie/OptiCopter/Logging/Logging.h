/*
 * Logging.h
 *
 *  Created on: Dec 2, 2013
 *      Author: helios
 */

#ifndef LOGGING_H_
#define LOGGING_H_
#include <Arduino.h>
#include "Serializer/Serializer.h"

namespace opticopter {

	class Logging {
	private:
		HalApm *hal;
		Serializer *serializer;
		static const uint32_t preamble = (uint32_t) 0xE5E7;
		static const uint32_t postamble = (uint32_t) 0x475E;
		uint32_t pos;
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
	public:
		struct __attribute__((__packed__)) Entry {
			uint32_t preeamble; //4
			uint32_t timestamp; //4 - 8
			int16_t accelX; //2 - 10
			int16_t accelY; //2 - 12
			int16_t accelZ; //2 - 14
			float gyroX; //4 - 18
			float gyroY; //4 - 22
			float gyroZ; //4 - 26
			float magX; //4 - 30
			float magY; //4 - 34
			float magZ; //4 - 38
			float roll; //4 - 42
			float pitch; //4 - 46
			float yaw; //4 - 50
			float rollLevel; //4 - 54
			float pitchLevel; //4 - 58
			float yawLevel; //4 - 62
			float rollPid; //4 - 66
			float pitchPid; //4 - 70
			float yawPid; //4 - 74
			int16_t output0; //2 - 76
			int16_t output1; //2 - 78
			int16_t output2; //2 - 80
			int16_t output3; //2 - 82
			uint32_t postamble; //4 - 86
		};
		Entry entry;
		uint8_t bytes[86];

		void entryToBytes() {
			setUL(entry.preeamble, 0);
			setUL(entry.timestamp, 4);
			setI(entry.accelX, 8);
			setI(entry.accelY, 10);
			setI(entry.accelZ, 12);
			setF(entry.gyroX, 14);
			setF(entry.gyroY, 18);
			setF(entry.gyroZ, 22);
			setF(entry.magX, 26);
			setF(entry.magY, 30);
			setF(entry.magZ, 34);
			setF(entry.roll, 38);
			setF(entry.pitch, 42);
			setF(entry.yaw, 46);
			setF(entry.rollLevel, 50);
			setF(entry.pitchLevel, 54);
			setF(entry.yawLevel, 58);
			setF(entry.rollPid, 62);
			setF(entry.pitchPid, 66);
			setF(entry.yawPid, 70);
			setI(entry.output0, 74);
			setI(entry.output1, 76);
			setI(entry.output2, 78);
			setI(entry.output3, 80);
			setUL(entry.postamble, 82);
		}
		void bytesToEntry() {
			entry.preeamble = getUL(0);
			entry.timestamp = getUL(4);
			entry.accelX = getI(8);
			entry.accelY = getI(10);
			entry.accelZ = getI(12);
			entry.gyroX = getF(14);
			entry.gyroY = getF(18);
			entry.gyroZ = getF(22);
			entry.magX = getF(26);
			entry.magY = getF(30);
			entry.magZ = getF(34);
			entry.roll = getF(38);
			entry.pitch = getF(42);
			entry.yaw = getF(46);
			entry.rollLevel = getF(50);
			entry.pitchLevel = getF(54);
			entry.yawLevel = getF(58);
			entry.rollPid = getF(62);
			entry.pitchPid = getF(66);
			entry.yawPid = getF(70);
			entry.output0 = getI(74);
			entry.output1 = getI(76);
			entry.output2 = getI(78);
			entry.output3 = getI(80);
			entry.postamble = getUL(82);
		}

		Entry* getEntry() {
			return &(entry);
		}

		uint8_t* getBytes() {
			return bytes;
		}

		void setPreAndPostAbmle() {
			entry.preeamble = preamble;
			entry.postamble = postamble;
		}

		bool checkPreAndPostAbmle() {
			return entry.preeamble == preamble && entry.postamble == postamble;
		}

		Logging(HalApm *hal, Serializer *serializer) :
				hal(hal), serializer(serializer) {
			pos = 0;
		}
		void init() {
			for (;;) {
				hal->readData(bytes, pos, 86);
				if (checkPreAndPostAbmle()) {
					pos += 86;
				} else {
					break;
				}
			}
		}
		void setPos(uint32_t iPos) {
			pos = iPos;
		}
		void save() {
			setPreAndPostAbmle();
			//serializer->beginn(serializer->ID_DATA);
			//serializer->write(bytes, 86);
			//serializer->end();
			entryToBytes();
			hal->writeData(bytes, pos, 86);
			pos += 86;
		}
		bool getNext() {
			hal->readData(bytes, pos, 86);
			bytesToEntry();
			pos += 86;
			if (checkPreAndPostAbmle()) {
				return true;
			} else {
				return false;
			}
		}
		virtual ~Logging() {
		}

		void setF(float f, uint8_t offset) {
			conv4.floating = f;
			for (uint8_t i = 0; i < 4; i++) {
				bytes[i + offset] = conv4.byte[i];
			}
		}

		void setI(int16_t s, int offset) {
			conv2.sint = s;
			for (uint8_t i = 0; i < 2; i++) {
				bytes[i + offset] = conv2.byte[i];
			}
		}

		void setUL(uint32_t ui, int offset) {
			conv4.uint = ui;
			for (uint8_t i = 0; i < 4; i++) {
				bytes[i + offset] = conv4.byte[i];
			}
		}

		float getF(uint8_t offset) {
			for (uint8_t i = 0; i < 4; i++) {
				conv4.byte[i] = bytes[i + offset];
			}
			return conv4.floating;
		}

		int16_t getI(int offset) {
			for (uint8_t i = 0; i < 2; i++) {
				conv2.byte[i] = bytes[i + offset];
			}
			return conv2.sint;
		}

		uint32_t getUL(int offset) {
			for (uint8_t i = 0; i < 4; i++) {
				conv4.byte[i] = bytes[i + offset];
			}
			return conv4.uint;
		}
	}
	;

} /* namespace opticopter */
#endif /* LOGGING_H_ */
