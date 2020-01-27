#include "Plane.h"



/* 
   call parachute library update
*/
void Plane::parachute_check()
{
#if PARACHUTE == ENABLED
    parachute.update();
    if (parachute.released()) {
        if (parachute_enabled == true) {
            gcs().send_text(MAV_SEVERITY_CRITICAL,"Parachute: Released");
            set_mode(mode_manual, MODE_REASON_GCS_COMMAND);
            parachute_enabled = false;
        }
    }

    if (parachute.auto_enabled() &&
        parachute.auto_release_alt_reached() &&
        parachute.auto_release_alt() > relative_altitude) {
        parachute_release();
    }
    
   if (parachute.auto_enabled()) {
        static bool chute_auto_ready = false;
        bool alt_reached = parachute.update_alt(relative_altitude);
        if (alt_reached && chute_auto_ready == false) {
            gcs().send_text(MAV_SEVERITY_CRITICAL,"Parachute: AUTO READY");
            chute_auto_ready = true;
        }
        chute_auto_ready = alt_reached;
    }
#endif
}

#if PARACHUTE == ENABLED

/*
  parachute_release - trigger the release of the parachute
*/
void Plane::parachute_release()
{
    arming.disarm();
    set_mode(mode_stabilize, MODE_REASON_GCS_COMMAND);
    parachute_enabled = true;
    if (parachute.release_in_progress()) {
        return;
    }
    if (parachute.released()) {
        gcs().send_text(MAV_SEVERITY_CRITICAL,"Parachute: Released again");
    } else {
        gcs().send_text(MAV_SEVERITY_CRITICAL,"Parachute: Disarmed, Elevon override");
    }

    // release parachute
    parachute.release();

#if LANDING_GEAR_ENABLED == ENABLED
    // deploy landing gear
    g2.landing_gear.set_position(AP_LandingGear::LandingGear_Deploy);
#endif
}

/*
  parachute_manual_release - trigger the release of the parachute,
  after performing some checks for pilot error checks if the vehicle
  is landed
*/
bool Plane::parachute_manual_release()
{
    // exit immediately if parachute is not enabled
    if (!parachute.enabled() || parachute.released()) {
        return false;
    }

    // if we get this far release parachute
    parachute_release();

#if LANDING_GEAR_ENABLED == ENABLED
    // deploy landing gear
    g2.landing_gear.set_position(AP_LandingGear::LandingGear_Deploy);
#endif
    return true;    
}
#endif