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
#pragma once

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(NO_LCD_MENUS)
  #error "Seriously? High resolution TFT screen without menu?"
#endif

#include "tft.h"
#include "tft_image.h"

#if ENABLED(TOUCH_SCREEN)
  #include "touch.h"
  extern bool draw_menu_navigation;
#endif

#if HAS_UI_320x240
  #include "ui_320x240.h"
#elif HAS_UI_480x320 || HAS_UI_480x272
  #include "ui_480x320.h"
#elif HAS_UI_1024x600
  #include "ui_1024x600.h"
#else
  #error "Unsupported display resolution!"
#endif

void draw_heater_status(uint16_t x, uint16_t y, const int8_t Heater);
void draw_fan_status(uint16_t x, uint16_t y, const bool blink);

void menu_line(const uint8_t row, uint16_t color=COLOR_BACKGROUND);
void menu_item(const uint8_t row, bool sel = false);

#if HAS_TOUCH_SLEEP
  bool lcd_sleep_task();
#endif

#define ABSOLUTE_ZERO     -273.15

#if HAS_TEMP_CHAMBER && HAS_MULTI_HOTEND
  #define ITEM_E0         0
  #define ITEM_E1         1
  #define ITEM_BED        2
  #define ITEM_CHAMBER    3
  #define ITEM_FAN        4
  #define ITEMS_COUNT     5
#elif HAS_TEMP_CHAMBER
  #define ITEM_E0         0
  #define ITEM_BED        1
  #define ITEM_CHAMBER    2
  #define ITEM_FAN        3
  #define ITEMS_COUNT     4
#elif HAS_TEMP_COOLER
  #define ITEM_COOLER     0
  #define ITEM_FAN        1
  #define ITEMS_COUNT     2
#elif HAS_MULTI_HOTEND
  #define ITEM_E0         0
  #define ITEM_E1         1
  #define ITEM_BED        2
  #define ITEM_FAN        3
  #define ITEMS_COUNT     4
#else
  #if ENABLED(RS_STYLE_COLOR_UI)
    #define ITEM_E0         0
    #if HOTENDS > 1
      #define ITEM_E1         1
      #define ITEM_BED        2
      #define ITEM_FAN        3
      #define ITEM_SPEED      4
      #define ITEM_FLOW       5
      #define ITEMS_COUNT1    3   // wide items count
      #define ITEMS_COUNT2    3   // narrow items count
      #define ITEM_WIDTH1     90
    #else
      #define ITEM_BED        1
      #define ITEM_FAN        2
      #define ITEM_SPEED      3
      #define ITEM_FLOW       4
      #define ITEMS_COUNT1    2   // wide items count
      #define ITEMS_COUNT2    3   // narrow items count
      #define ITEM_WIDTH1     110
    #endif
    #define ICON_INDENT1     (ITEM_WIDTH1 - ICON_WIDTH)/2
    #define ITEM_WIDTH2      66
    #define ICON_INDENT2     (ITEM_WIDTH2 - ICON_WIDTH)/2
    #define ICON_WIDTH      64
  #else
  #define ITEM_E0         0
  #define ITEM_BED        1
  #define ITEM_FAN        2
  #define ITEMS_COUNT     3
  #endif  // RS_STYLE_COLOR_UI
#endif
