#include "Plane.h"
#include "parachute.cpp"
#include "AP_Parachute.cpp"
#include"AP_Parachute.h"
#include "is_flying.cpp"

#if PARACHUTE == ENABLED

// Коды детектирования аварии
#define PARACHUTE_CHECK_ANGLE_DEVIATION_PITCH  5000    // критический угол 50 градусов тангажа от нулевого значения
#define PARACHUTE_CHECK_ANGLE_DEVIATION_ROLL   5500    // критический угол 55 градусов крена от нулевого значения

// emergency_parachute_check - выключает мотор и активирует парашют при детектировании потери управления
// АП определяет себя как "потерявший управление" при превышении углов
void Plane::emergency_parachute_check()
{
        static int32_t baro_alt_start;

    // немедленно выходим если парашют выключен
    if (!parachute.enabled()) {
        return;
    }

    // вызов обновления для того, чтобы перевести серво парашюта а выключенное положение
    parachute.update();

    // Немедленно выходим если двигатель выключен
    if (!motors->armed()) {
       return;
    }

    // Удостоверяемся что находимся в полете
    // не готово
    if (_is_flying=1 {
        return;
    }

    // Удостоверяемся что находимся над минимальной высотой раскрытия 2 метра
    // Не готово
    if (current_loc.alt < (2) {
        return;
    }

    // Проверяем вышли ли мы за пределы критических углов
     if (ahrs.pitch <= PARACHUTE_CHECK_ANGLE_DEVIATION_PITCH) 
    if (ahrs.roll <= PARACHUTE_CHECK_ANGLE_DEVIATION_ROLL)
    {
        return;
    }



// emergency_parachute_release - выключение двигателя, выпуск парашюта и отправка уведомления оператору
void Plane::emergency_parachute_release()
{
    // выключаем двигатель
    arming.disarm();

    // release parachute
    parachute.release();
     gcs().send_text(MAV_SEVERITY_INFO,"Emergency parachute: RELEASED");
        AP::logger().Write_Error(LogErrorSubsystem::PARACHUTES, LogErrorCode::PARACHUTE_RELEASED);

}

    // exit immediately if parachute is not enabled
    if (!parachute.enabled()) {
        return;
    }

    // do not release if vehicle is landed
    // do not release if we are landed or below the minimum altitude above home
    if (ap.land_complete) {
        // warn user of reason for failure
        gcs().send_text(MAV_SEVERITY_INFO,"Parachute: Landed");
        AP::logger().Write_Error(LogErrorSubsystem::PARACHUTES, LogErrorCode::PARACHUTE_LANDED);
        return;
    }

    // do not release if we are landed or below the minimum altitude above home
    if ((parachute.alt_min() != 0 && (current_loc.alt < (int32_t)parachute.alt_min() * 100))) {
        // warn user of reason for failure
        gcs().send_text(MAV_SEVERITY_ALERT,"Parachute: Too low");
        AP::logger().Write_Error(LogErrorSubsystem::PARACHUTES, LogErrorCode::PARACHUTE_TOO_LOW);
        return;
    }

    // if we get this far release parachute
    parachute_release();
}

#endif // PARACHUTE == ENABLED
