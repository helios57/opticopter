
#include <AP_Common/AP_Common.h>
#include <AP_Progmem/AP_Progmem.h>
#include <AP_Param/AP_Param.h>
#include <AP_Math/AP_Math.h>
#include <AP_HAL/AP_HAL.h>

#include <AP_GPS/AP_GPS.h>
#include <AP_AHRS/AP_AHRS.h>
#include <AP_InertialSensor.h>
#include <AP_Compass.h>
#include <AP_Declination.h>
#include <AP_Airspeed.h>
#include <AP_ADC/AP_ADC.h>
#include <AP_ADC_AnalogSource.h>
#include <AP_Baro/AP_Baro.h>
#include <AP_Buffer/AP_Buffer.h>
#include <Filter.h>
#include <GCS_MAVLink/GCS_MAVLink.h>
#include <RC_Channel/RC_Channel.h>
#include <AP_Airspeed.h>
#include <AP_Vehicle/AP_Vehicle.h>
#include <DataFlash.h>
#include <AP_Notify/AP_Notify.h>

#include <AP_Mount.h>

#include <AP_HAL_AVR.h>
const AP_HAL::HAL& hal = AP_HAL_BOARD_DRIVER;

void setup () {
    hal.console->println_P(PSTR("Unit test for AP_Mount. This sketch"
                "has no functionality, it only tests build."));
}
void loop () {}

AP_HAL_MAIN();
