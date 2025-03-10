/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../gcode.h"

#include "../../module/temperature.h"
#include "../../module/planner.h"       // for planner.finish_and_disable
#include "../../module/printcounter.h"  // for print_job_timer.stop
#include "../../lcd/marlinui.h"         // for LCD_MESSAGE_F

#include "../../inc/MarlinConfig.h"

#include "../../module/settings.h"

#if ENABLED(PSU_CONTROL)
  #include "../queue.h"
  #include "../../feature/power.h"
#endif

#if HAS_SUICIDE
  #include "../../MarlinCore.h"
#endif

#if ENABLED(PSU_CONTROL)

  /**
   * M80   : Turn on the Power Supply
   * M80 S : Report the current state and exit
   */
  void GcodeSuite::M80() {

    // S: Report the current power supply state and exit
    if (parser.seen('S')) {
      SERIAL_ECHOF(powerManager.psu_on ? F("PS:1\n") : F("PS:0\n"));
      return;
    }

    #if ENABLED(RS_ADDSETTINGS)
      if (psu_settings.psu_enabled)
    #endif
        powerManager.power_on();

    /**
     * If you have a switch on suicide pin, this is useful
     * if you want to start another print with suicide feature after
     * a print without suicide...
     */
    #if HAS_SUICIDE
      OUT_WRITE(SUICIDE_PIN, !SUICIDE_PIN_STATE);
    #endif

    TERN_(HAS_LCD_MENU, ui.reset_status());
  }

#endif // PSU_CONTROL

/**
 * M81: Turn off Power, including Power Supply, if there is one.
 *
 *      This code should ALWAYS be available for FULL SHUTDOWN!
 */
void GcodeSuite::M81() {
  planner.finish_and_disable();
  thermalManager.cooldown();

  print_job_timer.stop();

  #if HAS_FAN
    #if ENABLED(PROBING_FANS_OFF)
      thermalManager.fans_paused = false;
      ZERO(thermalManager.saved_fan_speed);
    #endif
  #endif

  safe_delay(1000); // Wait 1 second before switching off

  #if HAS_SUICIDE
    suicide();
  #elif ENABLED(PSU_CONTROL)
    #if ENABLED(RS_ADDSETTINGS)
      if (psu_settings.psu_enabled)
    #endif
        powerManager.power_off_soon();
  #endif

  LCD_MESSAGE_F(MACHINE_NAME " " STR_OFF ".");
}
