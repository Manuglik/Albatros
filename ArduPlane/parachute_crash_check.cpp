#include "Plane.h"
#include <AP_Parachute/AP_Parachute.h>
#include <AP_Parachute/AP_Parachute.cpp>
#include <SRV_Channel/SRV_Channel.h>

// Code to detect a crash main Arduplane code
#define PARACHUTE_CHECK_ANGLE_DEVIATION_PITCH  5000    // critical pitch angle 50 degrees to detect
#define PARACHUTE_CHECK_ANGLE_DEVIATION_ROLL   5500    // critical roll angle 55 degrees to detect

// emergency_parachute_check -  disarms motors if a crash has been detected
// crashes are detected by the vehicle being more than critical degrees
// called at MAIN_LOOP_RATE
void Plane::emergency_parachute_check()
{
    #if PARACHUTE == ENABLED
    parachute.update();

// return immediately if disarmed
//    if (!arming.is_armed() {
//        return;
//    }

// ensure we are in flight
//    if (!is_flying()) {
//        return;
//    }

// check takeoff altiude
//    float baro_alt = barometer.get_altitude();
// return immediately if is below 2m
//    const float blimit = 2;    
//    if (baro_alt < auto_state.baro_takeoff_alt+blimit) {
//        return;
    }
   
// check for angle error over critical degrees
    if (labs(ahrs.pitch_sensor) <= PARACHUTE_CHECK_ANGLE_DEVIATION_PITCH)) 
    if (labs(ahrs.roll_sensor) <= PARACHUTE_CHECK_ANGLE_DEVIATION_ROLL)) {
        return;
    }

// emergency_parachute_release - disarm motors, release parachute and sending message to GCS
    void Plane::emergency_parachute_release()
{

// disarm motors
    arming.disarm();

// move servo
    if (_release_type == AP_PARACHUTE_TRIGGER_TYPE_SERVO) {
    SRV_Channels::set_output_pwm(SRV_Channel::k_parachute_release, _servo_on_pwm);
    }

// set relay
    else if (_release_type <= AP_PARACHUTE_TRIGGER_TYPE_RELAY_3) {
             _relay.on(_release_type);
    }
            _release_in_progress = true;
            _released = true;
}

//send message to GCS and write log event
    gcs().send_text(MAV_SEVERITY_INFO,"Emergency parachute: RELEASED");
    AP::logger().Write_Error(LogErrorSubsystem::PARACHUTES, LogErrorCode::PARACHUTE_EMERGENCY_RELEASED);

// if we get this far release parachute
    emergency_parachute_release();   
    
}
    
#endif