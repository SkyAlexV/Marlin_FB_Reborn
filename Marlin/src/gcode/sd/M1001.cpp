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

#include "../../inc/MarlinConfig.h"

#if ENABLED(SDSUPPORT)

#include "../gcode.h"
#include "../../module/planner.h"
#include "../../module/printcounter.h"
#include "../../module/temperature.h"
#include "../../sd/cardreader.h"
#include "../../module/settings.h"
#include "../../lcd/marlinui.h"
#include "../../lcd/menu/menu.h"
extern MarlinUI ui;


#ifdef SD_FINISHED_RELEASECOMMAND
  #include "../queue.h"
#endif

#if EITHER(LCD_SET_PROGRESS_MANUALLY, SD_REPRINT_LAST_SELECTED_FILE)
  #include "../../lcd/marlinui.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../feature/powerloss.h"
#endif

#if HAS_LEDS_OFF_FLAG
  #include "../../MarlinCore.h" // for wait_for_user_response()
  #include "../../feature/leds/printer_event_leds.h"
#endif

#if ENABLED(EXTENSIBLE_UI)
  #include "../../lcd/extui/ui_api.h"
#elif ENABLED(DWIN_CREALITY_LCD_ENHANCED)
  #include "../../lcd/e3v2/enhanced/dwin.h"
#endif

#if ENABLED(HOST_ACTION_COMMANDS)
  #include "../../feature/host_actions.h"
#endif

#ifndef PE_LEDS_COMPLETED_TIME
  #define PE_LEDS_COMPLETED_TIME (30*60)
#endif

/**
 * M1001: Execute actions for SD print completion
 */
void GcodeSuite::M1001() {
  planner.synchronize();

  // SD Printing is finished when the queue reaches M1001
  card.flag.sdprinting = card.flag.sdprintdone = false;

  // If there's another auto#.g file to run...
  if (TERN(NO_SD_AUTOSTART, false, card.autofile_check())) return;

  // Purge the recovery file...
  TERN_(POWER_LOSS_RECOVERY, recovery.purge());

  // Report total print time
  const bool long_print = print_job_timer.duration() > 60;
  if (long_print) process_subcommands_now(F("M31"));

  // Stop the print job timer
  process_subcommands_now(F("M77"));

  // Set the progress bar "done" state
  TERN_(LCD_SET_PROGRESS_MANUALLY, ui.set_progress_done());

  // Announce SD file completion
  {
    PORT_REDIRECT(SerialMask::All);
    SERIAL_ECHOLNPGM(STR_FILE_PRINTED);
  }

  // Update the status LED color
  #if HAS_LEDS_OFF_FLAG
    if (long_print) {
      printerEventLEDs.onPrintCompleted();
      TERN_(EXTENSIBLE_UI, ExtUI::onUserConfirmRequired(GET_TEXT_F(MSG_PRINT_DONE)));
      TERN_(HOST_PROMPT_SUPPORT, hostui.prompt_do(PROMPT_USER_CONTINUE, GET_TEXT_F(MSG_PRINT_DONE), FPSTR(CONTINUE_STR)));
      TERN_(HAS_RESUME_CONTINUE, wait_for_user_response(SEC_TO_MS(TERN(HAS_LCD_MENU, PE_LEDS_COMPLETED_TIME, 30))));
      printerEventLEDs.onResumeAfterWait();
    }
  #endif

  // Inject SD_FINISHED_RELEASECOMMAND, if any
  #ifdef SD_FINISHED_RELEASECOMMAND
    process_subcommands_now(F(SD_FINISHED_RELEASECOMMAND));
  #endif

  TERN_(EXTENSIBLE_UI, ExtUI::onPrintFinished());
  TERN_(DWIN_CREALITY_LCD_ENHANCED, DWIN_Print_Finished());

  // Re-select the last printed file in the UI
  TERN_(SD_REPRINT_LAST_SELECTED_FILE, ui.reselect_last_file());

  #if ENABLED(RS_ADDSETTINGS)
    if (autooff_settings.poweroff_at_printed && psu_settings.psu_enabled)
    {
      autooff_settings.sscreen_need_draw = true;
      autooff_settings.poweroff_at_printed = false;
      ui.goto_screen(ui.poweroff_wait_screen);
//      gcode.process_subcommands_now_P(PSTR("M81"));   // Power Off command
    }
  #endif  // RS_ADDSETTINGS

}

#endif // SDSUPPORT
