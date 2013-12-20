// ======================================================
// VRmagic API Hello World demo
// ======================================================
// (c) 2012 VRmagic GmbH 
// ------------------------------------------------------
// HOWTO: Do simple image processing on source image 

#include "vrmusbcam2.h"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <sstream>

#ifdef _MSC_VER
#include <conio.h>
#else
#include <termios.h>
#include<unistd.h>
#endif

#include <sys/time.h>

using namespace std;

long time_ms(void) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (long) tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

char GetKey() {
	unsigned char ch = 0;
#ifdef _MSC_VER
	if(_kbhit())
	ch=_getch();
	return ch;
#else
	struct termios initial_settings, new_settings;
	tcgetattr(0, &initial_settings);
	new_settings = initial_settings;
	new_settings.c_lflag &= ~ICANON;
	new_settings.c_lflag &= ~ECHO;
	new_settings.c_lflag &= ~ISIG;
	new_settings.c_cc[VTIME] = 0;
	int nread;
	new_settings.c_cc[VMIN] = 0;
	tcsetattr(0, TCSANOW, &new_settings);
	nread = read(0, &ch, 1);
	tcsetattr(0, TCSANOW, &initial_settings);
	if (nread == 1) {
		return ch;
	}
#endif

	return 0;
}

void LogExit() {
	cerr << "VRmUsbCam Error: " << VRmUsbCamGetLastError() << "\nApplication exit" << endl;
	exit(-1);
}

class BoundingBox {
public:
	BoundingBox() :
			top(-1), right(-1), left(-1), bottom(-1) {
	}
	int top;
	int right;
	int left;
	int bottom;
};

BoundingBox boxes[100];
static const int boxMatchingTolerance = 1;
template<class T> int getPointsAboveTreshold(VRmImage* p_image, VRmDWORD threashold, const int step) {
	if (!p_image)
		return -1;
	int boundingBoxIndex = 0;
	for (VRmDWORD ii = 0; ii < p_image->m_image_format.m_height; ii += step) {
		T* ptr = (T*) (p_image->mp_buffer + ii * p_image->m_pitch);
		for (VRmDWORD jj = 0; jj < p_image->m_image_format.m_width; jj += step) {
			int x = jj;
			int y = ii;
			VRmDWORD value = (*ptr++);
			if (value >= threashold) {
				if (boundingBoxIndex == 0) {
					boxes[boundingBoxIndex].top = y;
					boxes[boundingBoxIndex].bottom = y;
					boxes[boundingBoxIndex].left = x;
					boxes[boundingBoxIndex].right = x;
					boundingBoxIndex++;
				}
				else {
					int indexMatchedBB = -1;
					for (int i = 0; i < boundingBoxIndex; i++) {
						BoundingBox current = boxes[i];
						//expand matched Box
						if (((current.top - boxMatchingTolerance) >= y) && ((current.bottom + boxMatchingTolerance) <= y) && ((current.left - boxMatchingTolerance) <= x) && ((current.right + boxMatchingTolerance) >= x)) {
							current.top = max(y, current.top);
							current.bottom = min(y, current.bottom);
							current.left = min(x, current.left);
							current.right = max(x, current.right);
							indexMatchedBB = i;
						}
						if (indexMatchedBB > 0) {
							//TODO merge boxes
						}
					}
					//Add new box
					if (indexMatchedBB < 0) {
						boxes[boundingBoxIndex].top = y;
						boxes[boundingBoxIndex].bottom = y;
						boxes[boundingBoxIndex].left = x;
						boxes[boundingBoxIndex].right = x;
						boundingBoxIndex++;
					}
				}
			}
		}
	}
	return boundingBoxIndex;
}

int main(int argc, char** argv) {
	// at first, be sure to call VRmUsbCamCleanup() at exit, 
	// even in case of an error
	atexit(VRmUsbCamCleanup);

	// uncomment this to enable logging features of VRmUsbCam
	// VRmUsbCamEnableLogging();

	VRmDWORD size = 0;
	// check for connected devices
	if (!VRmUsbCamGetDeviceKeyListSize(&size))
		LogExit();

	//open first usable device
	VRmUsbCamDevice device = 0;
	VRmDeviceKey* p_device_key = 0;
	stringstream device_string;
	for (VRmDWORD ii = 0; ii < size && !device; ++ii) {
		if (!VRmUsbCamGetDeviceKeyListEntry(ii, &p_device_key))
			LogExit();
		if (!p_device_key->m_busy) {
			if (!VRmUsbCamOpenDevice(p_device_key, &device))
				LogExit();
			VRmSTRING l_serial_string;
			if (!VRmUsbCamGetSerialString(p_device_key, &l_serial_string))
				LogExit();
			device_string << p_device_key->mp_product_str << "<#" << l_serial_string << ">";
		}
		if (!VRmUsbCamFreeDeviceKey(&p_device_key))
			LogExit();
	}
	// display error when no camera has been found
	if (!device) {
		cerr << "No suitable VRmagic device found!" << endl;
		exit(-1);
	}
	// NOTE:from now on, the "device" handle can be used to access the camera board.
	// use VRmUsbCamCloseDevice to end the usage

	//output of current source format 
	const char* format_string;
	if (!VRmUsbCamGetSourceFormatDescription(device, 1, &format_string))
		LogExit();
	cout << "Found VRmagic camera " << device_string.str() << " with format " << format_string << "!" << endl;

	// prepare framegrabber ... 
	if (!VRmUsbCamResetFrameCounter(device))
		LogExit();
	VRmImage* p_src_image = 0;
	VRmDWORD frames_dropped = 0;
	unsigned char ch = 0;

	// ... and start it
	if (!VRmUsbCamStart(device))
		LogExit();

	int pointsAboveThreshold;
	VRmDWORD threashold = 150;
	int maxPoints = 10000;
	VRmPointI points[maxPoints];
	long processedPictures = 0;
	// MAIN LOOP
	do {
		// Lock image in ringbuffer
		if (!VRmUsbCamLockNextImageEx(device, 1, &p_src_image, &frames_dropped)) {
			// in case of an error, check for trigger timeouts and trigger stalls.
			// both can be recovered, so continue. otherwise exit the app
			if (VRmUsbCamLastErrorWasTriggerTimeout())
				cerr << "trigger timeout" << endl;
			else if (VRmUsbCamLastErrorWasTriggerStall())
				cerr << "trigger stall" << endl;
			else
				LogExit();
		}

		if (p_src_image) {
			// if the processing is slower than the grabbing, frames will be dropped
			if (frames_dropped)
				cout << endl << frames_dropped << " frames dropped!" << endl;

			VRmDWORD pixeldepth_src;
			if (!VRmUsbCamGetPixelDepthFromColorFormat(p_src_image->m_image_format.m_color_format, &pixeldepth_src))
				LogExit();
			if (pixeldepth_src == 1)
				pointsAboveThreshold = getPointsAboveTreshold<VRmBYTE>(p_src_image, threashold, points, maxPoints, 1);
			else if (pixeldepth_src == 2)
				pointsAboveThreshold = getPointsAboveTreshold<VRmWORD>(p_src_image, threashold, points, maxPoints, 1);
			else {
				cout << "Pixeldepth >2 not supported!" << endl;
				exit(-1);
			}

			cout << "\rMillis,Count, Points above Threshold, Points(x,y): " << time_ms() << "," << processedPictures << "," << pointsAboveThreshold;
			for (int i = 0; i < min(pointsAboveThreshold, maxPoints); i++) {
				cout << ",(" << points[i].m_x << "," << points[i].m_y << ")";
			}

			cout << flush;

			// Release the image to the ringbuffer
			if (!VRmUsbCamUnlockNextImage(device, &p_src_image))
				LogExit();
		}

		ch = GetKey();
	} while (!ch);

	// Stop the framegrabber
	if (!VRmUsbCamStop(device))
		LogExit();

	// Close device before leaving
	if (!VRmUsbCamCloseDevice(device))
		LogExit();

	cout << endl << "Exit! " << endl;

	return 0;
}
