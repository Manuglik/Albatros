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
// АП определяет себя как "потерявший управление" при превышении критических углов
void Plane::emergency_parachute_check()
{
    
    // немедленно выходим если парашют выключен
    if (!parachute.enabled()) {
        return;
    }

    // вызов обновления для того, чтобы перевести серво парашюта а выключенное положение
    parachute.update();

    // Немедленно выходим если двигатель выключен
    if (AP_Arming=false {
       return;
    }
        // Удостоверяемся что находимся в полете
    // не готово
    if (_is_flying=1 {
        return;
    }

    // проверяем высоту включения двигателя
   float baro_alt = barometer.get_altitude();
    // Ниже 2 метров не включаем проверку потери управления
    const float blimit = 2;    
    if (baro_alt < auto_state.baro_takeoff_alt+blimit) {
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

    // выпускаем парашют и отправляем сообщение в наземную станцию
    parachute.release();
     gcs().send_text(MAV_SEVERITY_INFO,"Emergency parachute: RELEASED");
        AP::logger().Write_Error(LogErrorSubsystem::PARACHUTES, LogErrorCode::PARACHUTE_RELEASED);

}

    // немедленно выходим если парашют не включен
    if (!parachute.enabled()) {
        return;
    }

    }

    // если дошли досюда- выпускаем парашют
    parachute_release();
}

#endif // PARACHUTE == ENABLED
