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

//
// Backlash Menu
//

#include "../../inc/MarlinConfigPre.h"

#if BOTH(HAS_LCD_MENU, BACKLASH_GCODE)

#include "menu_item.h"

#include "../../feature/backlash.h"

void menu_backlash() {
  START_MENU();
  // BACK_ITEM(MSG_MAIN);

  EDIT_ITEM_FAST(percent, MSG_BACKLASH_CORRECTION, &backlash.correction, all_off, all_on);

  #if DISABLED(CORE_BACKLASH) || EITHER(MARKFORGED_XY, MARKFORGED_YX)
    #define _CAN_CALI AXIS_CAN_CALIBRATE
  #else
    #define _CAN_CALI(A) true
  #endif
  #define EDIT_BACKLASH_DISTANCE(N) EDIT_ITEM_FAST(float43, MSG_BACKLASH_##N, &backlash.distance_mm[_AXIS(N)], 0.0f, 9.9f);
  if (_CAN_CALI(A)) EDIT_BACKLASH_DISTANCE(A);
  #if HAS_Y_AXIS && _CAN_CALI(B)
    EDIT_BACKLASH_DISTANCE(B);
  #endif
  #if HAS_Z_AXIS && _CAN_CALI(C)
    EDIT_BACKLASH_DISTANCE(C);
  #endif
  #if HAS_I_AXIS && _CAN_CALI(I)
    EDIT_BACKLASH_DISTANCE(I);
  #endif
  #if HAS_J_AXIS && _CAN_CALI(J)
    EDIT_BACKLASH_DISTANCE(J);
  #endif
  #if HAS_K_AXIS && _CAN_CALI(K)
    EDIT_BACKLASH_DISTANCE(K);
  #endif

  #ifdef BACKLASH_SMOOTHING_MM
    EDIT_ITEM_FAST(float43, MSG_BACKLASH_SMOOTHING, &backlash.smoothing_mm, 0.0f, 9.9f);
  #endif

  END_MENU();
}

#endif // HAS_LCD_MENU && BACKLASH_GCODE
