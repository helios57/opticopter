/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __AndroidAccessory_h__
#define __AndroidAccessory_h__

#include "Arduino.h"
#include "Max3421e.h"
#include "Max3421e_constants.h"
#include "Usb.h"

#define USB_ACCESSORY_VENDOR_ID         	0x18D1
#define USB_ACCESSORY_PRODUCT_ID        	0x2D00

#define USB_ACCESSORY_ADB_PRODUCT_ID    	0x2D01
#define USB_ACCESSORY_AUDIO_PRODUCT_ID 		0x2D04// - accessory + audio
#define USB_ACCESSORY_AUDIO_ADB_PRODUCT_ID 	0x2D05// - accessory + audio + adb

#define ACCESSORY_STRING_MANUFACTURER   0
#define ACCESSORY_STRING_MODEL          1
#define ACCESSORY_STRING_DESCRIPTION    2
#define ACCESSORY_STRING_VERSION        3
#define ACCESSORY_STRING_URI            4
#define ACCESSORY_STRING_SERIAL         5

#define ACCESSORY_GET_PROTOCOL          51
#define ACCESSORY_SEND_STRING           52
#define ACCESSORY_START                 53

class AndroidAccessory {
private:
	const char *manufacturer;
	const char *model;
	const char *description;
	const char *version;
	const char *uri;
	const char *serial;

	MAX3421E max;
	USB usb;
	bool connected;
	uint8_t in;
	uint8_t out;

	EP_RECORD epRecord[8];

	uint8_t descBuff[256];

	bool isAccessoryDevice(USB_DEVICE_DESCRIPTOR *desc) {
		return desc->idVendor == USB_ACCESSORY_VENDOR_ID //
		&& (desc->idProduct == USB_ACCESSORY_PRODUCT_ID || //
		        desc->idProduct == USB_ACCESSORY_ADB_PRODUCT_ID || //
		        desc->idProduct == USB_ACCESSORY_AUDIO_PRODUCT_ID || //
		        desc->idProduct == USB_ACCESSORY_AUDIO_ADB_PRODUCT_ID);
	}

	int getProtocol(byte addr);
	void sendString(byte addr, int index, const char *str);
	bool switchDevice(byte addr);
	bool findEndpoints(byte addr, EP_RECORD *inEp, EP_RECORD *outEp);
	bool configureAndroid(void);

public:
	AndroidAccessory(const char *manufacturer, const char *model, const char *description, const char *version, const char *uri, const char *serial);

	void powerOn(void);

	bool isConnected(void);
	int read(void *buff, int len, unsigned int nakLimit = USB_NAK_LIMIT);
	int write(void *buff, int len);
};

#endif /* __AndroidAccessory_h__ */
