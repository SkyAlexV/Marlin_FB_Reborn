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

#include "../../inc/MarlinConfigPre.h"

#if HAS_UI_480x320 || HAS_UI_480x272

#include "ui_common.h"

#include "../marlinui.h"
#include "../menu/menu.h"
#include "../../libs/numtostr.h"

#include "../../sd/cardreader.h"
#include "../../module/temperature.h"
#include "../../module/printcounter.h"
#include "../../module/planner.h"
#include "../../module/motion.h"
#include "../language/language_en.h"
#include "../../lcd/thumbnails.h"

#if DISABLED(LCD_PROGRESS_BAR) && BOTH(FILAMENT_LCD_DISPLAY, SDSUPPORT)
  #include "../../feature/filwidth.h"
  #include "../../gcode/parser.h"
#endif

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #include "../../feature/bedlevel/bedlevel.h"
#endif

void MarlinUI::tft_idle() {
  #if ENABLED(TOUCH_SCREEN)
    if (TERN0(HAS_TOUCH_SLEEP, lcd_sleep_task())) return;
    if (draw_menu_navigation) {
      add_control(104, TFT_HEIGHT - 34, PAGE_UP, imgPageUp, encoderTopLine > 0);
      add_control(344, TFT_HEIGHT - 34, PAGE_DOWN, imgPageDown, encoderTopLine + LCD_HEIGHT < screen_items);
      add_control(224, TFT_HEIGHT - 34, BACK, imgBack);
      draw_menu_navigation = false;
    }
  #endif

  tft.queue.async();
  TERN_(TOUCH_SCREEN, touch.idle());
}

#if ENABLED(SHOW_BOOTSCREEN)

  void MarlinUI::show_bootscreen() {
    tft.queue.reset();

    tft.canvas(0, 0, TFT_WIDTH, TFT_HEIGHT);
    #if ENABLED(BOOT_MARLIN_LOGO_SMALL)
      #define BOOT_LOGO_W 195   // MarlinLogo195x59x16
      #define BOOT_LOGO_H  59
      #define SITE_URL_Y (TFT_HEIGHT - 55)    // must be enough for 2 lines
      tft.set_background(COLOR_BACKGROUND);
    #else
      #define BOOT_LOGO_W TFT_WIDTH   // MarlinLogo480x320x16
      #define BOOT_LOGO_H TFT_HEIGHT
      #define SITE_URL_Y (TFT_HEIGHT - 90)
    #endif
    tft.add_image((TFT_WIDTH - BOOT_LOGO_W) / 2, (TFT_HEIGHT - BOOT_LOGO_H) / 2, imgBootScreen);
    
    tft_string.set(Language_en::MSG_MARLIN);
    tft_string.add(" v");
    tft_string.add(SHORT_REBORN_VERSION);
    tft.add_text(tft_string.center(TFT_WIDTH), SITE_URL_Y, COLOR_WEBSITE_URL, tft_string);
    #ifdef WEBSITE_URL
      tft_string.set(WEBSITE_URL);
      tft.add_text(tft_string.center(TFT_WIDTH), SITE_URL_Y+25, COLOR_WEBSITE_URL, tft_string);
    #endif
    tft.queue.sync();
  }

  void MarlinUI::bootscreen_completion(const millis_t sofar) {
    if ((BOOTSCREEN_TIMEOUT) > sofar) safe_delay((BOOTSCREEN_TIMEOUT) - sofar);
    clear_lcd();
  }

#endif

void MarlinUI::draw_kill_screen() {
  tft.queue.reset();
  tft.fill(0, 0, TFT_WIDTH, TFT_HEIGHT, COLOR_KILL_SCREEN_BG);

  uint16_t line = 2;

  menu_line(line++, COLOR_KILL_SCREEN_BG);
  tft_string.set(status_message);
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);

  line++;
  menu_line(line++, COLOR_KILL_SCREEN_BG);
  tft_string.set(GET_TEXT(MSG_HALTED));
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);

  menu_line(line++, COLOR_KILL_SCREEN_BG);
  tft_string.set(GET_TEXT(MSG_PLEASE_RESET));
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);

  tft.queue.sync();
}

void draw_heater_status(uint16_t x, uint16_t y, const int8_t Heater) {
  MarlinImage image = imgHotEnd;
  // MarlinImage image = imgMove;
  uint16_t Color;
  celsius_t currentTemperature, targetTemperature;
  #if ENABLED(RS_STYLE_COLOR_UI)
    celsius_t coldExtrusionTemp;
  #endif

  if (Heater >= 0) { // HotEnd
    currentTemperature = thermalManager.wholeDegHotend(Heater);
    targetTemperature = thermalManager.degTargetHotend(Heater);
    #if ENABLED(RS_STYLE_COLOR_UI)
      coldExtrusionTemp = thermalManager.extrude_min_temp;
    #endif
  }
  #if HAS_HEATED_BED
    else if (Heater == H_BED) {
      currentTemperature = thermalManager.wholeDegBed();
      targetTemperature = thermalManager.degTargetBed();
    }
  #endif
  #if HAS_TEMP_CHAMBER
    else if (Heater == H_CHAMBER) {
      currentTemperature = thermalManager.wholeDegChamber();
      #if HAS_HEATED_CHAMBER
        targetTemperature = thermalManager.degTargetChamber();
      #else
        targetTemperature = ABSOLUTE_ZERO;
      #endif
    }
  #endif
  #if HAS_TEMP_COOLER
    else if (Heater == H_COOLER) {
      currentTemperature = thermalManager.wholeDegCooler();
      targetTemperature = TERN(HAS_COOLER, thermalManager.degTargetCooler(), ABSOLUTE_ZERO);
    }
  #endif
  else return;

#if ENABLED(RS_STYLE_COLOR_UI)
  TERN_(TOUCH_SCREEN, if (targetTemperature >= 0) touch.add_control(HEATER, x, y, ITEM_WIDTH1, 96, Heater));
  tft.canvas(x, y, ITEM_WIDTH1, 96);
  tft.set_background(COLOR_BACKGROUND);

  Color = currentTemperature < 0 ? COLOR_INACTIVE : COLOR_COLD;

  if (Heater >= 0) { // HotEnd
    if (currentTemperature >= coldExtrusionTemp) Color = COLOR_HOTEND;
    if (currentTemperature >= targetTemperature - TEMP_HYSTERESIS && currentTemperature <= targetTemperature + TEMP_HYSTERESIS) Color = COLOR_TARGET_HOTEND;
    
  }
  #if HAS_HEATED_BED
    else if (Heater == H_BED) {
      if (currentTemperature >= 40) Color = COLOR_HEATED_BED;
      if (currentTemperature >= targetTemperature - TEMP_HYSTERESIS && currentTemperature <= targetTemperature + TEMP_HYSTERESIS) Color = COLOR_TARGET_BED;
      image = targetTemperature > 0 ? imgBedHeated : imgBed;
    }
  #endif
  #if HAS_TEMP_CHAMBER
    else if (Heater == H_CHAMBER) {
      if (currentTemperature >= 50) Color = COLOR_CHAMBER;
      image = targetTemperature > 0 ? imgChamberHeated : imgChamber;
    }
  #endif
  #if HAS_TEMP_COOLER
    else if (Heater == H_COOLER) {
      if (currentTemperature <= 26) Color = COLOR_COLD;
      if (currentTemperature > 26) Color = COLOR_RED;
      image = targetTemperature > 26 ? imgCoolerHot : imgCooler;
    }
  #endif

  tft.add_image(ICON_INDENT1, 1, image, Color);

  Color = COLOR_TEMP_TEXT;
  tft_string.set((uint8_t *)i16tostr3rj(currentTemperature));
  if (targetTemperature > 0) {
    tft_string.add("/");
    tft_string.add((uint8_t *)i16tostr3rj(targetTemperature));
  }
  tft_string.add(LCD_STR_DEGREE);
  tft_string.trim();
  tft.add_text(tft_string.center(ITEM_WIDTH1) + 2, 68, Color, tft_string);

#else   // #if ENABLED(RS_STYLE_COLOR_UI)
  TERN_(TOUCH_SCREEN, if (targetTemperature >= 0) touch.add_control(HEATER, x, y, 80, 120, Heater));
  tft.canvas(x, y, 80, 120);
  tft.set_background(COLOR_BACKGROUND);

  Color = currentTemperature < 0 ? COLOR_INACTIVE : COLOR_COLD;

  if (Heater >= 0) { // HotEnd
    if (currentTemperature >= 50) Color = COLOR_HOTEND;
  }
  #if HAS_HEATED_BED
    else if (Heater == H_BED) {
      if (currentTemperature >= 50) Color = COLOR_HEATED_BED;
      image = targetTemperature > 0 ? imgBedHeated : imgBed;
    }
  #endif
  #if HAS_TEMP_CHAMBER
    else if (Heater == H_CHAMBER) {
      if (currentTemperature >= 50) Color = COLOR_CHAMBER;
      image = targetTemperature > 0 ? imgChamberHeated : imgChamber;
    }
  #endif
  #if HAS_TEMP_COOLER
    else if (Heater == H_COOLER) {
      if (currentTemperature <= 26) Color = COLOR_COLD;
      if (currentTemperature > 26) Color = COLOR_RED;
      image = targetTemperature > 26 ? imgCoolerHot : imgCooler;
    }
  #endif

  tft.add_image(8, 28, image, Color);

  tft_string.set((uint8_t *)i16tostr3rj(currentTemperature));
  tft_string.add(LCD_STR_DEGREE);
  tft_string.trim();
  tft.add_text(tft_string.center(80) + 2, 82, Color, tft_string);

  if (targetTemperature >= 0) {
    tft_string.set((uint8_t *)i16tostr3rj(targetTemperature));
    tft_string.add(LCD_STR_DEGREE);
    tft_string.trim();
    tft.add_text(tft_string.center(80) + 2, 8, Color, tft_string);
  }
#endif  // #if ENABLED(RS_STYLE_COLOR_UI)
}

void draw_fan_status(uint16_t x, uint16_t y, const bool blink) {
  uint8_t fanSpeed = thermalManager.fan_speed[0];
  MarlinImage image;

#if ENABLED(RS_STYLE_COLOR_UI)
  TERN_(TOUCH_SCREEN, touch.add_control(FAN, x, y, ITEM_WIDTH2, 96));
  tft.canvas(x, y, ITEM_WIDTH2, 96);
  tft.set_background(COLOR_BACKGROUND);

  if (fanSpeed >= 127)
      image = blink ? imgFanFast1 : imgFanFast0;
    else if (fanSpeed > 0)
      image = blink ? imgFanSlow1 : imgFanSlow0;
    else
      image = imgFanIdle;

  uint16_t  Color = COLOR_FAN;
  if (fanSpeed > 25)
    Color = COLOR_FAN_ACTIVE;
  tft.add_image(ICON_INDENT2, 1, image, Color);

  tft_string.set((uint8_t *)ui8tostr4pctrj(thermalManager.fan_speed[0]));
  tft_string.trim();
  tft.add_text(tft_string.center(ITEM_WIDTH2) + 2, 68, COLOR_TEMP_TEXT, tft_string);
#else   // #if ENABLED(RS_STYLE_COLOR_UI)
  TERN_(TOUCH_SCREEN, touch.add_control(FAN, x, y, 80, 120));
  tft.canvas(x, y, 80, 120);
  tft.set_background(COLOR_BACKGROUND);

  if (fanSpeed >= 127)
    image = blink ? imgFanFast1 : imgFanFast0;
  else if (fanSpeed > 0)
    image = blink ? imgFanSlow1 : imgFanSlow0;
  else
    image = imgFanIdle;

  tft.add_image(8, 20, image, COLOR_FAN);

  tft_string.set((uint8_t *)ui8tostr4pctrj(thermalManager.fan_speed[0]));
  tft_string.trim();
  tft.add_text(tft_string.center(80) + 6, 82, COLOR_FAN, tft_string);
#endif  // #if ENABLED(RS_STYLE_COLOR_UI)

}

#if ENABLED(RS_STYLE_COLOR_UI)
  void draw_feedrate_status(uint16_t x, uint16_t y) {
    MarlinImage image = imgFeedRate;

    TERN_(TOUCH_SCREEN, touch.add_control(FEEDRATE, x, y, ITEM_WIDTH2, 96));
    tft.canvas(x, y, ITEM_WIDTH2, 96);
    tft.set_background(COLOR_BACKGROUND);

    uint16_t  Color = feedrate_percentage == 100 ? COLOR_RATE_100 : COLOR_RATE_ALTERED;;
    tft.add_image(ICON_INDENT2, 1, image, Color);

    tft_string.set((uint8_t *)i16tostr3rj(feedrate_percentage));
    tft_string.add('%');
    tft_string.trim();
    tft.add_text(tft_string.center(ITEM_WIDTH2) + 2, 68, Color, tft_string);

  }

  void draw_flowrate_status(uint16_t x, uint16_t y) {
    MarlinImage image = imgFlowRate;

    TERN_(TOUCH_SCREEN, touch.add_control(FLOWRATE, x, y, ITEM_WIDTH2, 96));
    tft.canvas(x, y, ITEM_WIDTH2, 96);
    tft.set_background(COLOR_BACKGROUND);

    uint16_t  Color = planner.flow_percentage[active_extruder] == 100 ? COLOR_RATE_100 : COLOR_RATE_ALTERED;;
    tft.add_image(ICON_INDENT2, 1, image, Color);

    tft_string.set((uint8_t *)i16tostr3rj(planner.flow_percentage[active_extruder]));
    tft_string.add('%');
    tft_string.trim();
    tft.add_text(tft_string.center(ITEM_WIDTH2) + 2, 68, Color, tft_string);

  }
#endif  // RS_STYLE_COLOR_UI


void MarlinUI::draw_status_screen() {
  const bool blink = get_blink();

  #if ENABLED(RS_STYLE_COLOR_UI)
    static bool       prev_is_printing = 0;
  #endif

  TERN_(TOUCH_SCREEN, touch.clear());

  // heaters and fan
  uint16_t i, x, y = 0, sw = (TFT_WIDTH - (ITEMS_COUNT1 * ITEM_WIDTH1 + ITEMS_COUNT2 * ITEM_WIDTH2)) / (ITEMS_COUNT1 + ITEMS_COUNT2 + 1);
  x = sw;

#if ENABLED(RS_STYLE_COLOR_UI)

  // Top state line: coords, time, etc
  tft.canvas(0, y, TFT_WIDTH - 1, 32);
  tft.set_background(COLOR_BACKGROUND);
  tft.add_rectangle(0, 0, TFT_WIDTH - 1, 32, COLOR_TOP_FRAME_BORDER);

  y = 3;
  // Z coord
  tft.add_text(8, y, COLOR_TOP_FRAME_TEXT , "Z:");

  bool not_homed = axis_should_home(X_AXIS) | axis_should_home(Y_AXIS) | axis_should_home(Z_AXIS);
  if (blink && not_homed)
    tft_string.set("?");
  else {
    const float z = LOGICAL_Z_POSITION(current_position.z);
    tft_string.set(ftostr42_52(z));
    tft_string.rtrim();
  }
  tft.add_text(110 - tft_string.width(), y, COLOR_TOP_FRAME_TEXT, tft_string);

  // Moving speed
  tft_string.set(ftostr5rj(SQRT(planner.get_current_block()->nominal_speed_sqr)));
  tft_string.trim();
  tft_string.add(" ");
  tft_string.add("mm/s");
  tft.add_text(290 - tft_string.width(), y, COLOR_TOP_FRAME_TEXT, tft_string);


  // Printing time
  char buffer[18];
  duration_t elapsed = print_job_timer.duration();
  duration_t heating = print_job_timer.durationHeat();
  duration_t remain;
  elapsed.toDigital(buffer);

  tft_string.set(buffer);
  if (printJobOngoing() || printingIsPaused())
  {

    #ifdef _MARLIN_CONFIG_MY
      // heating time for debug purpose
      tft_string.add(" / ");
      heating.toDigital(buffer);
      tft_string.add(buffer);
    #endif

    // remain time
    tft_string.add(" / ");
    if (elapsed.value > heating.value && (elapsed.value - heating.value) > 60)   // remain time only after 1 minute of printing (except heating time)
    {
      uint32_t  fsize = card.getFileSize();
      uint32_t  freaded = card.getIndex();
      float     bytes_per_sec = (float)freaded / (elapsed.value - heating.value);
      remain.value = (fsize - freaded) / bytes_per_sec;
      remain.toDigital(buffer);
      tft_string.add(buffer);
    }
    else
    {
      tft_string.add("--:--");
    }
  }
  uint16_t Color = COLOR_TOP_FRAME_TEXT;
  if (wait_for_heatup)
    Color = COLOR_RED;
  tft.add_text(470 - tft_string.width(), y, Color, tft_string);

  // Hotend, bed, fan
  y = 32;
  for (i = 0 ; i < ITEMS_COUNT1+ITEMS_COUNT2; i++) {
    switch (i) {
      #ifdef ITEM_E0
        case ITEM_E0:
          draw_heater_status(x, y, H_E0);
          x += sw + ITEM_WIDTH1;
          break;
      #endif
      #ifdef ITEM_E1
        case ITEM_E1: draw_heater_status(x, y, H_E1); break;
      #endif
      #ifdef ITEM_E2
        case ITEM_E2: draw_heater_status(x, y, H_E2); break;
      #endif
      #ifdef ITEM_BED
        case ITEM_BED:
          draw_heater_status(x, y, H_BED);
          x += sw + ITEM_WIDTH1;
          break;
      #endif
      #ifdef ITEM_CHAMBER
        case ITEM_CHAMBER: draw_heater_status(x, y, H_CHAMBER); break;
      #endif
      #ifdef ITEM_COOLER
        case ITEM_COOLER: draw_heater_status(x, y, H_COOLER); break;
      #endif
      #ifdef ITEM_FAN
        case ITEM_FAN:
          draw_fan_status(x, y, blink);
          x += sw + ITEM_WIDTH2;
          break;
      #endif
      #ifdef ITEM_FLOW
        case ITEM_FLOW:
          draw_flowrate_status(x, y);
          x += sw + ITEM_WIDTH2;
          break;
      #endif
      #ifdef ITEM_SPEED
        case ITEM_SPEED:
          draw_feedrate_status(x, y);
          x += sw + ITEM_WIDTH2;
          break;
      #endif
    }
  }

  // progress bar
  y = 130;
  const uint8_t progress = ui.get_progress_percent();
  tft.canvas(0, y, TFT_WIDTH - 1, 82);
  tft.set_background(COLOR_PROGRESS_BG);
  tft.add_bar(0, 0, TFT_WIDTH - 1, 82, COLOR_PROGRESS_BG);
  tft.add_rectangle(0, 0, TFT_WIDTH - 1, 82, COLOR_PROGRESS_FRAME);
  if (progress)
    tft.add_bar(2, 2, ((TFT_WIDTH - 5) * progress) / 100, 78, COLOR_PROGRESS_BAR);
  tft_string.set(pcttostrpctrj(progress));
  tft_string.trim();
  tft_string.add("  (");
  tft_string.add(ftostr32_52(e_move_accumulator / 1000));
  tft_string.add("m)");
  tft.add_text(240 - tft_string.width() / 2, 8, COLOR_PROGRESS_TEXT, tft_string);

  // file name
  if (printJobOngoing() || printingIsPaused())
  {
    tft_string.set(card.longFilename);
    x = tft_string.width();
    if (x > 460)
    x = 470;
    tft.add_text(240 - x / 2, 48, COLOR_PROGRESS_TEXT, tft_string);
  }

  #if ENABLED(TOUCH_SCREEN)
  y = 212;
  // if not in printing job
  if (!printJobOngoing() && !printingIsPaused())
  {
    if (prev_is_printing)
    {
      prev_is_printing = 0;
      tft.fill(0, y, TFT_WIDTH, TFT_HEIGHT-y, COLOR_BACKGROUND);
    }
    uint16_t dx = (TFT_WIDTH - 3 * 100) / 4;
    x = dx + (100-64)/2;
    y += 8;

    // SD
    TERN_(SDSUPPORT, add_control(x, y, menu_media, imgSD, !printingIsActive(), COLOR_CONTROL_ENABLED, card.isMounted() && printingIsActive() ? COLOR_BUSY : COLOR_CONTROL_DISABLED));
    x += dx + 100;

    // Menu
    add_control(x, y, menu_main, imgMenu, 1, COLOR_CONTROL_ENABLED);
    x += dx + 100;

    // Move
    add_control(x, y, MOVE_AXIS, imgMove, 1, COLOR_CONTROL_ENABLED);
  }
  else
  {
    if (!prev_is_printing)
    {
      prev_is_printing = 1;
      tft.fill(0, 212, TFT_WIDTH, TFT_HEIGHT-y, COLOR_BACKGROUND);
    }
    uint16_t dx = (TFT_WIDTH - 4 * 100) / 5;
    x = dx;
    y += 8;
    // menu tune
    add_control(x, y, menu_tune, imgSettings, 1, COLOR_CONTROL_ENABLED);

    // menu main
    x += dx + 100;
    add_control(x, y, menu_main, imgMenu, 1, COLOR_CONTROL_ENABLED);

    // resume
    x += dx + 100;
    if (printingIsPaused())
    {
      add_control(x, y, PRINT_RESUME, imgResume, 1, COLOR_CONTROL_ENABLED);
    }
    // pause
    else
    {
      add_control(x, y, PRINT_PAUSE, imgPause, 1, COLOR_CONTROL_ENABLED);
    }

    // stop
    x += dx + 100;
      add_control(x, y, PRINT_STOP, imgCancel, 1, COLOR_CONTROL_CANCEL);
  }
  #endif

  y = 292;
  // status message
  tft.canvas(0, y, TFT_WIDTH, 27);
  tft.set_background(COLOR_BACKGROUND);
  tft_string.set(status_message);
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_STATUS_MESSAGE, tft_string);
#else   // #if ENABLED(RS_STYLE_COLOR_UI)
  for (i = 0 ; i < ITEMS_COUNT; i++) {
    x = (TFT_WIDTH / ITEMS_COUNT - 80) / 2  + (TFT_WIDTH * i / ITEMS_COUNT);
    switch (i) {
      #ifdef ITEM_E0
        case ITEM_E0: draw_heater_status(x, y, H_E0); break;
      #endif
      #ifdef ITEM_E1
        case ITEM_E1: draw_heater_status(x, y, H_E1); break;
      #endif
      #ifdef ITEM_E2
        case ITEM_E2: draw_heater_status(x, y, H_E2); break;
      #endif
      #ifdef ITEM_BED
        case ITEM_BED: draw_heater_status(x, y, H_BED); break;
      #endif
      #ifdef ITEM_CHAMBER
        case ITEM_CHAMBER: draw_heater_status(x, y, H_CHAMBER); break;
      #endif
      #ifdef ITEM_COOLER
        case ITEM_COOLER: draw_heater_status(x, y, H_COOLER); break;
      #endif
      #ifdef ITEM_FAN
        case ITEM_FAN: draw_fan_status(x, y, blink); break;
      #endif
    }
  }

  y += TERN(HAS_UI_480x272, 118, 128);

  // coordinates
  tft.canvas(4, y, TFT_WIDTH - 8, FONT_LINE_HEIGHT);
  tft.set_background(COLOR_BACKGROUND);
  tft.add_rectangle(0, 0, TFT_WIDTH - 8, FONT_LINE_HEIGHT, COLOR_AXIS_HOMED);

  if (TERN0(LCD_SHOW_E_TOTAL, printingIsActive())) {
    #if ENABLED(LCD_SHOW_E_TOTAL)
      tft.add_text( 16, 3, COLOR_AXIS_HOMED , "E");
      const uint8_t escale = e_move_accumulator >= 100000.0f ? 10 : 1; // After 100m switch to cm
      tft_string.set(ftostr4sign(e_move_accumulator / escale));
      tft_string.add(escale == 10 ? 'c' : 'm');
      tft_string.add('m');
      tft.add_text(192 - tft_string.width(), 3, COLOR_AXIS_HOMED, tft_string);
    #endif
  }
  else {
    tft.add_text( 16, 3, COLOR_AXIS_HOMED , "X");
    const bool nhx = axis_should_home(X_AXIS);
    tft_string.set(blink && nhx ? "?" : ftostr4sign(LOGICAL_X_POSITION(current_position.x)));
    tft.add_text(102 - tft_string.width(), 3, nhx ? COLOR_AXIS_NOT_HOMED : COLOR_AXIS_HOMED, tft_string);

    tft.add_text(192, 3, COLOR_AXIS_HOMED , "Y");
    const bool nhy = axis_should_home(Y_AXIS);
    tft_string.set(blink && nhy ? "?" : ftostr4sign(LOGICAL_Y_POSITION(current_position.y)));
    tft.add_text(280 - tft_string.width(), 3, nhy ? COLOR_AXIS_NOT_HOMED : COLOR_AXIS_HOMED, tft_string);
  }
  tft.add_text(330, 3, COLOR_AXIS_HOMED , "Z");
  uint16_t offset = 32;
  const bool nhz = axis_should_home(Z_AXIS);
  if (blink && nhz)
    tft_string.set("?");
  else {
    const float z = LOGICAL_Z_POSITION(current_position.z);
    tft_string.set(ftostr52sp((int16_t)z));
    tft_string.rtrim();
    offset += tft_string.width();

    tft_string.set(ftostr52sp(z));
    offset -= tft_string.width();
  }
  tft.add_text(455 - tft_string.width() - offset, 3, nhz ? COLOR_AXIS_NOT_HOMED : COLOR_AXIS_HOMED, tft_string);
  TERN_(TOUCH_SCREEN, touch.add_control(MOVE_AXIS, 4, y, TFT_WIDTH - 8, FONT_LINE_HEIGHT));

  y += TERN(HAS_UI_480x272, 38, 48);
  // feed rate
  tft.canvas(96, y, 100, 32);
  tft.set_background(COLOR_BACKGROUND);
  uint16_t color = feedrate_percentage == 100 ? COLOR_RATE_100 : COLOR_RATE_ALTERED;
  tft.add_image(0, 0, imgFeedRate, color);
  tft_string.set(i16tostr3rj(feedrate_percentage));
  tft_string.add('%');
  tft.add_text(36, 1, color , tft_string);
  TERN_(TOUCH_SCREEN, touch.add_control(FEEDRATE, 96, 176, 100, 32));

  // flow rate
  tft.canvas(284, y, 100, 32);
  tft.set_background(COLOR_BACKGROUND);
  color = planner.flow_percentage[0] == 100 ? COLOR_RATE_100 : COLOR_RATE_ALTERED;
  tft.add_image(0, 0, imgFlowRate, color);
  tft_string.set(i16tostr3rj(planner.flow_percentage[active_extruder]));
  tft_string.add('%');
  tft.add_text(36, 1, color , tft_string);
  TERN_(TOUCH_SCREEN, touch.add_control(FLOWRATE, 284, 176, 100, 32, active_extruder));

  #if ENABLED(TOUCH_SCREEN)
    add_control(404, y, menu_main, imgSettings);
    TERN_(SDSUPPORT, add_control(12, y, menu_media, imgSD, !printingIsActive(), COLOR_CONTROL_ENABLED, card.isMounted() && printingIsActive() ? COLOR_BUSY : COLOR_CONTROL_DISABLED));
  #endif

  y += TERN(HAS_UI_480x272, 36, 44);
  // print duration
  char buffer[14];
  duration_t elapsed = print_job_timer.duration();
  elapsed.toDigital(buffer);

  tft.canvas((TFT_WIDTH - 128) / 2, y, 128, 29);
  tft.set_background(COLOR_BACKGROUND);
  tft_string.set(buffer);
  tft.add_text(tft_string.center(128), 0, COLOR_PRINT_TIME, tft_string);

  y += TERN(HAS_UI_480x272, 28, 36);
  // progress bar
  const uint8_t progress = ui.get_progress_percent();
  tft.canvas(4, y, TFT_WIDTH - 8, 9);
  tft.set_background(COLOR_PROGRESS_BG);
  tft.add_rectangle(0, 0, TFT_WIDTH - 8, 9, COLOR_PROGRESS_FRAME);
  if (progress)
    tft.add_bar(1, 1, ((TFT_WIDTH - 10) * progress) / 100, 7, COLOR_PROGRESS_BAR);

  y += 20;
  // status message
  tft.canvas(0, y, TFT_WIDTH, FONT_LINE_HEIGHT - 5);
  tft.set_background(COLOR_BACKGROUND);
  tft_string.set(status_message);
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_STATUS_MESSAGE, tft_string);
#endif  // #if ENABLED(RS_STYLE_COLOR_UI)
}

// Low-level draw_edit_screen can be used to draw an edit screen from anyplace
void MenuEditItemBase::draw_edit_screen(PGM_P const pstr, const char * const value/*=nullptr*/) {
  ui.encoder_direction_normal();
  TERN_(TOUCH_SCREEN, touch.clear());

#if ENABLED(RS_STYLE_COLOR_UI)
  // Name of editable field
  uint16_t cy = 10;
  tft.canvas(0, cy, TFT_WIDTH, MENU_ITEM_HEIGHT);
  tft.set_background(COLOR_BACKGROUND);
  tft_string.set(pstr, itemIndex, itemString);
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);

  TERN_(AUTO_BED_LEVELING_UBL, if (ui.external_control) line++);  // ftostr52() will overwrite *value so *value has to be displayed first

  // current value
  cy += MENU_ITEM_HEIGHT;
  tft.canvas(0, cy, TFT_WIDTH, MENU_ITEM_HEIGHT);
  tft.set_background(COLOR_BACKGROUND);
  tft_string.set(value);
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

  #if ENABLED(AUTO_BED_LEVELING_UBL)
    if (ui.external_control) {
      menu_line(line - 1);

      tft_string.set(X_LBL);
      tft.add_text((TFT_WIDTH / 2 - ITEM_WIDTH), MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
      tft_string.set(ftostr52(LOGICAL_X_POSITION(current_position.x)));
      tft_string.trim();
      tft.add_text((TFT_WIDTH / 2 - 16) - tft_string.width(), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

      tft_string.set(Y_LBL);
      tft.add_text((TFT_WIDTH / 2 + 16), MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
      tft_string.set(ftostr52(LOGICAL_X_POSITION(current_position.y)));
      tft_string.trim();
      tft.add_text((TFT_WIDTH / 2 + ITEM_WIDTH) - tft_string.width(), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);
    }
  #endif

  // predefined values
  cy += MENU_ITEM_HEIGHT;

  cy += MENU_ITEM_HEIGHT;
  extern screenFunc_t _manual_move_func_ptr;
  if (ui.currentScreen != _manual_move_func_ptr && !ui.external_control) {

    #define SLIDER_LENGTH 336
    #define SLIDER_Y_POSITION 206

    tft.canvas((TFT_WIDTH - SLIDER_LENGTH) / 2, SLIDER_Y_POSITION, SLIDER_LENGTH, 16);
    tft.set_background(COLOR_BACKGROUND);

    int16_t position = (SLIDER_LENGTH - 2) * ui.encoderPosition / maxEditValue;
    tft.add_bar(0, 7, 1, 2, ui.encoderPosition == 0 ? COLOR_SLIDER_INACTIVE : COLOR_SLIDER);
    tft.add_bar(1, 6, position, 4, COLOR_SLIDER);
    tft.add_bar(position + 1, 6, SLIDER_LENGTH - 2 - position, 4, COLOR_SLIDER_INACTIVE);
    tft.add_bar(SLIDER_LENGTH - 1, 7, 1, 2, int32_t(ui.encoderPosition) == maxEditValue ? COLOR_SLIDER : COLOR_SLIDER_INACTIVE);

    #if ENABLED(TOUCH_SCREEN)
      tft.add_image((SLIDER_LENGTH - 8) * ui.encoderPosition / maxEditValue, 0, imgSlider, COLOR_SLIDER);
      touch.add_control(SLIDER, (TFT_WIDTH - SLIDER_LENGTH) / 2, SLIDER_Y_POSITION - 8, SLIDER_LENGTH, 32, maxEditValue);
    #endif
  }
#else // ENABLED(RS_STYLE_COLOR_UI)
  uint16_t line = 1;

  menu_line(line++);
  tft_string.set(pstr, itemIndex, itemString);
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);

  TERN_(AUTO_BED_LEVELING_UBL, if (ui.external_control) line++);  // ftostr52() will overwrite *value so *value has to be displayed first

  menu_line(line);
  tft_string.set(value);
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

  #if ENABLED(AUTO_BED_LEVELING_UBL)
    if (ui.external_control) {
      menu_line(line - 1);

      tft_string.set(X_LBL);
      tft.add_text((TFT_WIDTH / 2 - 120), MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
      tft_string.set(ftostr52(LOGICAL_X_POSITION(current_position.x)));
      tft_string.trim();
      tft.add_text((TFT_WIDTH / 2 - 16) - tft_string.width(), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

      tft_string.set(Y_LBL);
      tft.add_text((TFT_WIDTH / 2 + 16), MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
      tft_string.set(ftostr52(LOGICAL_X_POSITION(current_position.y)));
      tft_string.trim();
      tft.add_text((TFT_WIDTH / 2 + 120) - tft_string.width(), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);
    }
  #endif

  extern screenFunc_t _manual_move_func_ptr;
  if (ui.currentScreen != _manual_move_func_ptr && !ui.external_control) {

    #define SLIDER_LENGTH 336
    #define SLIDER_Y_POSITION 186

    tft.canvas((TFT_WIDTH - SLIDER_LENGTH) / 2, SLIDER_Y_POSITION, SLIDER_LENGTH, 16);
    tft.set_background(COLOR_BACKGROUND);

    int16_t position = (SLIDER_LENGTH - 2) * ui.encoderPosition / maxEditValue;
    tft.add_bar(0, 7, 1, 2, ui.encoderPosition == 0 ? COLOR_SLIDER_INACTIVE : COLOR_SLIDER);
    tft.add_bar(1, 6, position, 4, COLOR_SLIDER);
    tft.add_bar(position + 1, 6, SLIDER_LENGTH - 2 - position, 4, COLOR_SLIDER_INACTIVE);
    tft.add_bar(SLIDER_LENGTH - 1, 7, 1, 2, int32_t(ui.encoderPosition) == maxEditValue ? COLOR_SLIDER : COLOR_SLIDER_INACTIVE);

    #if ENABLED(TOUCH_SCREEN)
      tft.add_image((SLIDER_LENGTH - 8) * ui.encoderPosition / maxEditValue, 0, imgSlider, COLOR_SLIDER);
      touch.add_control(SLIDER, (TFT_WIDTH - SLIDER_LENGTH) / 2, SLIDER_Y_POSITION - 8, SLIDER_LENGTH, 32, maxEditValue);
    #endif
  }
#endif // ENABLED(RS_STYLE_COLOR_UI)

  tft.draw_edit_screen_buttons();
}

void TFT::draw_edit_screen_buttons() {
  #if ENABLED(TOUCH_SCREEN)
    add_control(64, TFT_HEIGHT - 64, DECREASE, imgDecrease);
    add_control(352, TFT_HEIGHT - 64, INCREASE, imgIncrease);
    add_control(208, TFT_HEIGHT - 64, CLICK, imgConfirm);
  #endif
}

#if ENABLED(RS_STYLE_COLOR_UI)
  void MenuItem_fileconfirm::draw_select_screen(const char * const string/*=nullptr*/) {

    #if ENABLED(THUMBNAILS_PREVIEW)
      char  str[256];

      tft.canvas(0, 2, TFT_WIDTH, 30);
      tft.set_background(COLOR_BACKGROUND);

      strcpy(str, GET_TEXT(MSG_START_PRINT));
      strcat(str, " ?");
      tft_string.set(str);
      tft_string.trim();
      tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);

      if (string)
      {
        tft.canvas(0, 32, TFT_WIDTH, 30);
        tft.set_background(COLOR_BACKGROUND);
        tft_string.set(string);
        tft_string.trim();
        tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);
      }

      bool is_thumb = thumbnails.Open(card.filename);
      // card.openFileRead(card.filename);
      // card.closefile();

      if (is_thumb)
      {
        thumbnails.DrawThumbnail(5, 75, 230, 220);
        thumbnails.Close();
      }
      else
      {
        thumbnails.DrawDefaultThumbnail(5, 75, 230, 220);
      }
    #else  // ENABLED(THUMBNAILS_PREVIEW)
      uint16_t line = 1;
      char  str[256];

      menu_line(line++);
      strcpy(str, GET_TEXT(MSG_START_PRINT));
      strcat(str, " ?");
      tft_string.set(str);
      tft_string.trim();
      tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);

      line++;
      if (string) {
        menu_line(line++);
        tft_string.set(string);
        tft_string.trim();
        tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);
      }
      else {
        line++;
      }
    #endif  // ENABLED(THUMBNAILS_PREVIEW)

    // if (suff) {
    //   menu_line(line);
    //   tft_string.set(suff);
    //   tft_string.trim();
    //   tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);
    // }
    #if ENABLED(TOUCH_SCREEN)
      add_control(406, TFT_HEIGHT - 64, CANCEL, imgCancel, true, HALF(COLOR_CONTROL_CANCEL));
      add_control(240, TFT_HEIGHT - 64, CONFIRM, imgConfirm, true, COLOR_CONTROL_CONFIRM);
    #endif
  }
#endif // ENABLED(RS_STYLE_COLOR_UI)

// The Select Screen presents a prompt and two "buttons"
void MenuItem_confirm::draw_select_screen(PGM_P const yes, PGM_P const no, const bool yesno, PGM_P const pref, const char * const string/*=nullptr*/, PGM_P const suff/*=nullptr*/) {
  uint16_t line = 1;

  if (!string) line++;

  menu_line(line++);
  tft_string.set(pref);
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);

  if (string) {
    menu_line(line++);
    tft_string.set(string);
    tft_string.trim();
    tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);
  }

  if (suff) {
    menu_line(line);
    tft_string.set(suff);
    tft_string.trim();
    tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);
  }
  #if ENABLED(TOUCH_SCREEN)
    add_control(328, TFT_HEIGHT - 64, CANCEL, imgCancel, true, yesno ? HALF(COLOR_CONTROL_CANCEL) : COLOR_CONTROL_CANCEL);
    add_control(88, TFT_HEIGHT - 64, CONFIRM, imgConfirm, true, yesno ? COLOR_CONTROL_CONFIRM : HALF(COLOR_CONTROL_CONFIRM));
  #endif
}

#if ENABLED(ADVANCED_PAUSE_FEATURE)

  void MarlinUI::draw_hotend_status(const uint8_t row, const uint8_t extruder) {
    #if ENABLED(TOUCH_SCREEN)
      touch.clear();
      draw_menu_navigation = false;
      touch.add_control(RESUME_CONTINUE , 0, 0, TFT_WIDTH, TFT_HEIGHT);
    #endif

    menu_line(row);
    tft_string.set(GET_TEXT(MSG_FILAMENT_CHANGE_NOZZLE));
    tft_string.add('E');
    tft_string.add((char)('1' + extruder));
    tft_string.add(' ');
    tft_string.add(i16tostr3rj(thermalManager.wholeDegHotend(extruder)));
    tft_string.add(LCD_STR_DEGREE);
    tft_string.add(" / ");
    tft_string.add(i16tostr3rj(thermalManager.degTargetHotend(extruder)));
    tft_string.add(LCD_STR_DEGREE);
    tft_string.trim();
    tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);
  }

#endif // ADVANCED_PAUSE_FEATURE

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #define GRID_OFFSET_X   8
  #define GRID_OFFSET_Y   8
  #define GRID_WIDTH      192
  #define GRID_HEIGHT     192
  #define CONTROL_OFFSET  16

  void MarlinUI::ubl_plot(const uint8_t x_plot, const uint8_t y_plot) {

    tft.canvas(GRID_OFFSET_X, GRID_OFFSET_Y, GRID_WIDTH, GRID_HEIGHT);
    tft.set_background(COLOR_BACKGROUND);
    tft.add_rectangle(0, 0, GRID_WIDTH, GRID_HEIGHT, COLOR_WHITE);

    for (uint16_t x = 0; x < (GRID_MAX_POINTS_X); x++)
      for (uint16_t y = 0; y < (GRID_MAX_POINTS_Y); y++)
        if (position_is_reachable({ ubl.mesh_index_to_xpos(x), ubl.mesh_index_to_ypos(y) }))
          tft.add_bar(1 + (x * 2 + 1) * (GRID_WIDTH - 4) / (GRID_MAX_POINTS_X) / 2, GRID_HEIGHT - 3 - ((y * 2 + 1) * (GRID_HEIGHT - 4) / (GRID_MAX_POINTS_Y) / 2), 2, 2, COLOR_UBL);

    tft.add_rectangle((x_plot * 2 + 1) * (GRID_WIDTH - 4) / (GRID_MAX_POINTS_X) / 2 - 1, GRID_HEIGHT - 5 - ((y_plot * 2 + 1) * (GRID_HEIGHT - 4) / (GRID_MAX_POINTS_Y) / 2), 6, 6, COLOR_UBL);

    const xy_pos_t pos = { ubl.mesh_index_to_xpos(x_plot), ubl.mesh_index_to_ypos(y_plot) },
                   lpos = pos.asLogical();

    tft.canvas(320, GRID_OFFSET_Y + (GRID_HEIGHT - MENU_ITEM_HEIGHT) / 2 - MENU_ITEM_HEIGHT, 120, MENU_ITEM_HEIGHT);
    tft.set_background(COLOR_BACKGROUND);
    tft_string.set(X_LBL);
    tft.add_text(0, MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
    tft_string.set(ftostr52(lpos.x));
    tft_string.trim();
    tft.add_text(120 - tft_string.width(), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

    tft.canvas(320, GRID_OFFSET_Y + (GRID_HEIGHT - MENU_ITEM_HEIGHT) / 2, 120, MENU_ITEM_HEIGHT);
    tft.set_background(COLOR_BACKGROUND);
    tft_string.set(Y_LBL);
    tft.add_text(0, MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
    tft_string.set(ftostr52(lpos.y));
    tft_string.trim();
    tft.add_text(120 - tft_string.width(), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

    tft.canvas(320, GRID_OFFSET_Y + (GRID_HEIGHT - MENU_ITEM_HEIGHT) / 2 + MENU_ITEM_HEIGHT, 120, MENU_ITEM_HEIGHT);
    tft.set_background(COLOR_BACKGROUND);
    tft_string.set(Z_LBL);
    tft.add_text(0, MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
    tft_string.set(isnan(ubl.z_values[x_plot][y_plot]) ? "-----" : ftostr43sign(ubl.z_values[x_plot][y_plot]));
    tft_string.trim();
    tft.add_text(120 - tft_string.width(), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

    constexpr uint8_t w = (TFT_WIDTH) / 10;
    tft.canvas(GRID_OFFSET_X + (GRID_WIDTH - w) / 2, GRID_OFFSET_Y + GRID_HEIGHT + CONTROL_OFFSET - 5, w, MENU_ITEM_HEIGHT);
    tft.set_background(COLOR_BACKGROUND);
    tft_string.set(ui8tostr3rj(x_plot));
    tft_string.trim();
    tft.add_text(tft_string.center(w), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

    tft.canvas(GRID_OFFSET_X + GRID_WIDTH + CONTROL_OFFSET + 16 - 24, GRID_OFFSET_Y + (GRID_HEIGHT - MENU_ITEM_HEIGHT) / 2, w, MENU_ITEM_HEIGHT);
    tft.set_background(COLOR_BACKGROUND);
    tft_string.set(ui8tostr3rj(y_plot));
    tft_string.trim();
    tft.add_text(tft_string.center(w), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

    #if ENABLED(TOUCH_SCREEN)
      touch.clear();
      draw_menu_navigation = false;
      add_control(GRID_OFFSET_X + GRID_WIDTH + CONTROL_OFFSET,      GRID_OFFSET_Y + CONTROL_OFFSET,                    UBL,  (ENCODER_STEPS_PER_MENU_ITEM) * (GRID_MAX_POINTS_X), imgUp);
      add_control(GRID_OFFSET_X + GRID_WIDTH + CONTROL_OFFSET,      GRID_OFFSET_Y + GRID_HEIGHT - CONTROL_OFFSET - 32, UBL, -(ENCODER_STEPS_PER_MENU_ITEM) * (GRID_MAX_POINTS_X), imgDown);
      add_control(GRID_OFFSET_X + CONTROL_OFFSET,                   GRID_OFFSET_Y + GRID_HEIGHT + CONTROL_OFFSET,      UBL, -(ENCODER_STEPS_PER_MENU_ITEM), imgLeft);
      add_control(GRID_OFFSET_X + GRID_WIDTH - CONTROL_OFFSET - 32, GRID_OFFSET_Y + GRID_HEIGHT + CONTROL_OFFSET,      UBL,   ENCODER_STEPS_PER_MENU_ITEM, imgRight);
      add_control(320, GRID_OFFSET_Y + GRID_HEIGHT + CONTROL_OFFSET, CLICK, imgLeveling);
      add_control(224, TFT_HEIGHT - 34, BACK, imgBack);
    #endif
  }
#endif // AUTO_BED_LEVELING_UBL

#if ENABLED(BABYSTEP_ZPROBE_OFFSET)
  #include "../../feature/babystep.h"
#endif

#if HAS_BED_PROBE
  #include "../../module/probe.h"
#endif

#define Z_SELECTION_Z 1
#define Z_SELECTION_Z_PROBE -1

struct MotionAxisState {
  xy_int_t xValuePos, yValuePos, zValuePos, eValuePos, stepValuePos, zTypePos, eNamePos;
  float currentStepSize = 10.0;
  int z_selection = Z_SELECTION_Z;
  uint8_t e_selection = 0;
  bool blocked = false;
  char message[32];
};

MotionAxisState motionAxisState;

#define E_BTN_COLOR COLOR_YELLOW
#define X_BTN_COLOR COLOR_CORAL_RED
#define Y_BTN_COLOR COLOR_VIVID_GREEN
#define Z_BTN_COLOR COLOR_LIGHT_BLUE

#define BTN_WIDTH 64
#define BTN_HEIGHT 52
#define X_MARGIN 20
#define Y_MARGIN 15

static void quick_feedback() {
  #if HAS_CHIRP
    ui.chirp(); // Buzz and wait. Is the delay needed for buttons to settle?
    #if BOTH(HAS_LCD_MENU, USE_BEEPER)
      for (int8_t i = 5; i--;) { buzzer.tick(); delay(2); }
    #elif HAS_LCD_MENU
      delay(10);
    #endif
  #endif
}

#define CUR_STEP_VALUE_WIDTH 104
static void drawCurStepValue() {
  tft_string.set((uint8_t *)ftostr52sp(motionAxisState.currentStepSize));
  tft_string.add("mm");
  tft.canvas(motionAxisState.stepValuePos.x, motionAxisState.stepValuePos.y, CUR_STEP_VALUE_WIDTH, BTN_HEIGHT);
  tft.set_background(COLOR_BACKGROUND);
  tft.add_text(tft_string.center(CUR_STEP_VALUE_WIDTH), 0, COLOR_AXIS_HOMED, tft_string);
}

static void drawCurZSelection() {
  tft_string.set("Z");
  tft.canvas(motionAxisState.zTypePos.x, motionAxisState.zTypePos.y, tft_string.width(), 34);
  tft.set_background(COLOR_BACKGROUND);
  tft.add_text(0, 0, Z_BTN_COLOR, tft_string);
  tft.queue.sync();
  tft_string.set("Offset");
  tft.canvas(motionAxisState.zTypePos.x, motionAxisState.zTypePos.y + 34, tft_string.width(), 34);
  tft.set_background(COLOR_BACKGROUND);
  if (motionAxisState.z_selection == Z_SELECTION_Z_PROBE) {
    tft.add_text(0, 0, Z_BTN_COLOR, tft_string);
  }
}

static void drawCurESelection() {
  tft.canvas(motionAxisState.eNamePos.x, motionAxisState.eNamePos.y, BTN_WIDTH, BTN_HEIGHT);
  tft.set_background(COLOR_BACKGROUND);
  tft_string.set("E");
  tft.add_text(0, 0, E_BTN_COLOR , tft_string);
  tft.add_text(tft_string.width(), 0, E_BTN_COLOR, ui8tostr3rj(motionAxisState.e_selection));
}

static void drawMessage(const char *msg) {
  tft.canvas(X_MARGIN, TFT_HEIGHT - Y_MARGIN - 34, TFT_HEIGHT / 2, 34);
  tft.set_background(COLOR_BACKGROUND);
  tft.add_text(0, 0, COLOR_YELLOW, msg);
}

static void drawAxisValue(const AxisEnum axis) {
  const float value = (
    TERN_(HAS_BED_PROBE, axis == Z_AXIS && motionAxisState.z_selection == Z_SELECTION_Z_PROBE ? probe.offset.z :)
    ui.manual_move.axis_value(axis)
  );
  xy_int_t pos;
  uint16_t color;
  switch (axis) {
    case X_AXIS: pos = motionAxisState.xValuePos; color = X_BTN_COLOR; break;
    case Y_AXIS: pos = motionAxisState.yValuePos; color = Y_BTN_COLOR; break;
    case Z_AXIS: pos = motionAxisState.zValuePos; color = Z_BTN_COLOR; break;
    case E_AXIS: pos = motionAxisState.eValuePos; color = E_BTN_COLOR; break;
    default: return;
  }
  tft.canvas(pos.x, pos.y, BTN_WIDTH + X_MARGIN, BTN_HEIGHT);
  tft.set_background(COLOR_BACKGROUND);
  tft_string.set(ftostr52sp(value));
  tft.add_text(0, 0, color, tft_string);
}

static void moveAxis(const AxisEnum axis, const int8_t direction) {
  quick_feedback();

  #if ENABLED(PREVENT_COLD_EXTRUSION)
    if (axis == E_AXIS && thermalManager.tooColdToExtrude(motionAxisState.e_selection)) {
      drawMessage("Too cold");
      return;
    }
  #endif

  const float diff = motionAxisState.currentStepSize * direction;

  if (axis == Z_AXIS && motionAxisState.z_selection == Z_SELECTION_Z_PROBE) {
    #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
      const int16_t babystep_increment = direction * BABYSTEP_SIZE_Z;
      const bool do_probe = DISABLED(BABYSTEP_HOTEND_Z_OFFSET) || active_extruder == 0;
      const float bsDiff = planner.mm_per_step[Z_AXIS] * babystep_increment,
                  new_probe_offset = probe.offset.z + bsDiff,
                  new_offs = TERN(BABYSTEP_HOTEND_Z_OFFSET
                    , do_probe ? new_probe_offset : hotend_offset[active_extruder].z - bsDiff
                    , new_probe_offset
                  );
      if (WITHIN(new_offs, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX)) {
        babystep.add_steps(Z_AXIS, babystep_increment);
        if (do_probe)
          probe.offset.z = new_offs;
        else
          TERN(BABYSTEP_HOTEND_Z_OFFSET, hotend_offset[active_extruder].z = new_offs, NOOP);
        drawMessage(""); // clear the error
        drawAxisValue(axis);
      }
      else {
        drawMessage(GET_TEXT(MSG_LCD_SOFT_ENDSTOPS));
      }
    #elif HAS_BED_PROBE
      // only change probe.offset.z
      probe.offset.z += diff;
      if (direction < 0 && current_position[axis] < Z_PROBE_OFFSET_RANGE_MIN) {
        current_position[axis] = Z_PROBE_OFFSET_RANGE_MIN;
        drawMessage(GET_TEXT(MSG_LCD_SOFT_ENDSTOPS));
      }
      else if (direction > 0 && current_position[axis] > Z_PROBE_OFFSET_RANGE_MAX) {
        current_position[axis] = Z_PROBE_OFFSET_RANGE_MAX;
        drawMessage(GET_TEXT(MSG_LCD_SOFT_ENDSTOPS));
      }
      else {
        drawMessage(""); // clear the error
      }
      drawAxisValue(axis);
    #endif
    return;
  }

  if (!ui.manual_move.processing) {
    // Get motion limit from software endstops, if any
    float min, max;
    soft_endstop.get_manual_axis_limits(axis, min, max);

    // Delta limits XY based on the current offset from center
    // This assumes the center is 0,0
    #if ENABLED(DELTA)
      if (axis != Z_AXIS && axis != E_AXIS) {
        max = SQRT(sq((float)(DELTA_PRINTABLE_RADIUS)) - sq(current_position[Y_AXIS - axis])); // (Y_AXIS - axis) == the other axis
        min = -max;
      }
    #endif

    // Get the new position
    const bool limited = ui.manual_move.apply_diff(axis, diff, min, max);
    #if IS_KINEMATIC
      UNUSED(limited);
    #else
      PGM_P const msg = limited ? GET_TEXT(MSG_LCD_SOFT_ENDSTOPS) : NUL_STR;
      drawMessage(msg);
    #endif

    ui.manual_move.soon(axis OPTARG(MULTI_E_MANUAL, motionAxisState.e_selection));
  }

  drawAxisValue(axis);
}

static void e_plus()  { moveAxis(E_AXIS, 1);  }
static void e_minus() { moveAxis(E_AXIS, -1); }
static void x_minus() { moveAxis(X_AXIS, -1); }
static void x_plus()  { moveAxis(X_AXIS, 1);  }
static void y_plus()  { moveAxis(Y_AXIS, 1);  }
static void y_minus() { moveAxis(Y_AXIS, -1); }
static void z_plus()  { moveAxis(Z_AXIS, 1);  }
static void z_minus() { moveAxis(Z_AXIS, -1); }

#if ENABLED(TOUCH_SCREEN)
  static void e_select() {
    motionAxisState.e_selection++;
    if (motionAxisState.e_selection >= EXTRUDERS) {
      motionAxisState.e_selection = 0;
    }

    quick_feedback();
    drawCurESelection();
    drawAxisValue(E_AXIS);
  }

  static void do_home() {
    quick_feedback();
    drawMessage(GET_TEXT(MSG_LEVEL_BED_HOMING));
    queue.inject_P(G28_STR);
    // Disable touch until home is done
    TERN_(HAS_TFT_XPT2046, touch.disable());
    drawAxisValue(E_AXIS);
    drawAxisValue(X_AXIS);
    drawAxisValue(Y_AXIS);
    drawAxisValue(Z_AXIS);
  }

  static void step_size() {
    motionAxisState.currentStepSize = motionAxisState.currentStepSize / 10.0;
    if (motionAxisState.currentStepSize < 0.0015) motionAxisState.currentStepSize = 10.0;
    quick_feedback();
    drawCurStepValue();
  }
#endif

#if HAS_BED_PROBE
  static void z_select() {
    motionAxisState.z_selection *= -1;
    quick_feedback();
    drawCurZSelection();
    drawAxisValue(Z_AXIS);
  }
#endif

static void disable_steppers() {
  quick_feedback();
  queue.inject(F("M84"));
}

static void drawBtn(int x, int y, const char *label, intptr_t data, MarlinImage img, uint16_t bgColor, bool enabled = true) {
  uint16_t width = Images[imgBtn52Rounded].width;
  uint16_t height = Images[imgBtn52Rounded].height;

  if (!enabled) bgColor = COLOR_CONTROL_DISABLED;

  tft.canvas(x, y, width, height);
  tft.set_background(COLOR_BACKGROUND);
  tft.add_image(0, 0, imgBtn52Rounded, bgColor, COLOR_BACKGROUND, COLOR_DARKGREY);

  // TODO: Make an add_text() taking a font arg
  if (label) {
    tft_string.set(label);
    tft_string.trim();
    tft.add_text(tft_string.center(width), height / 2 - tft_string.font_height() / 2, bgColor, tft_string);
  }
  else {
    tft.add_image(0, 0, img, bgColor, COLOR_BACKGROUND, COLOR_DARKGREY);
  }

  TERN_(HAS_TFT_XPT2046, if (enabled) touch.add_control(BUTTON, x, y, width, height, data));
}

void MarlinUI::move_axis_screen() {
  // Reset
  defer_status_screen(true);
  motionAxisState.blocked = false;
  TERN_(HAS_TFT_XPT2046, touch.enable());

  ui.clear_lcd();

  TERN_(TOUCH_SCREEN, touch.clear());

  const bool busy = printingIsActive();

  // Babysteps during printing? Select babystep for Z probe offset
  if (busy && ENABLED(BABYSTEP_ZPROBE_OFFSET))
    motionAxisState.z_selection = Z_SELECTION_Z_PROBE;

  // ROW 1 -> E- Y- CurY Z+
  int x = X_MARGIN, y = Y_MARGIN, spacing = 0;

  drawBtn(x, y, "E+", (intptr_t)e_plus, imgUp, E_BTN_COLOR, !busy);

  spacing = (TFT_WIDTH - X_MARGIN * 2 - 3 * BTN_WIDTH) / 2;
  x += BTN_WIDTH + spacing;
  drawBtn(x, y, "Y+", (intptr_t)y_plus, imgUp, Y_BTN_COLOR, !busy);

  // Cur Y
  x += BTN_WIDTH;
  motionAxisState.yValuePos.x = x + 2;
  motionAxisState.yValuePos.y = y;
  drawAxisValue(Y_AXIS);

  x += spacing;
  drawBtn(x, y, "Z-", (intptr_t)z_minus, imgUp, Z_BTN_COLOR, !busy || ENABLED(BABYSTEP_ZPROBE_OFFSET)); //only enabled when not busy or have baby step

  // ROW 2 -> "Ex"  X-  HOME X+  "Z"
  y += BTN_HEIGHT + (TFT_HEIGHT - Y_MARGIN * 2 - 4 * BTN_HEIGHT) / 3;
  x = X_MARGIN;
  spacing = (TFT_WIDTH - X_MARGIN * 2 - 5 * BTN_WIDTH) / 4;

  motionAxisState.eNamePos.x = x;
  motionAxisState.eNamePos.y = y;
  drawCurESelection();
  TERN_(HAS_TFT_XPT2046, if (!busy) touch.add_control(BUTTON, x, y, BTN_WIDTH, BTN_HEIGHT, (intptr_t)e_select));

  x += BTN_WIDTH + spacing;
  drawBtn(x, y, "X-", (intptr_t)x_minus, imgLeft, X_BTN_COLOR, !busy);

  x += BTN_WIDTH + spacing; //imgHome is 64x64
  TERN_(HAS_TFT_XPT2046, add_control(TFT_WIDTH / 2 - Images[imgHome].width / 2, y - (Images[imgHome].width - BTN_HEIGHT) / 2, BUTTON, (intptr_t)do_home, imgHome, !busy));

  x += BTN_WIDTH + spacing;
  uint16_t xplus_x = x;
  drawBtn(x, y, "X+", (intptr_t)x_plus, imgRight, X_BTN_COLOR, !busy);

  x += BTN_WIDTH + spacing;
  motionAxisState.zTypePos.x = x;
  motionAxisState.zTypePos.y = y;
  drawCurZSelection();
  #if BOTH(HAS_BED_PROBE, TOUCH_SCREEN)
    if (!busy) touch.add_control(BUTTON, x, y, BTN_WIDTH, 34 * 2, (intptr_t)z_select);
  #endif

  // ROW 3 -> E- CurX Y-  Z-
  y += BTN_HEIGHT + (TFT_HEIGHT - Y_MARGIN * 2 - 4 * BTN_HEIGHT) / 3;
  x = X_MARGIN;
  spacing = (TFT_WIDTH - X_MARGIN * 2 - 3 * BTN_WIDTH) / 2;

  drawBtn(x, y, "E-", (intptr_t)e_minus, imgDown, E_BTN_COLOR, !busy);

  // Cur E
  motionAxisState.eValuePos.x = x;
  motionAxisState.eValuePos.y = y + BTN_HEIGHT + 2;
  drawAxisValue(E_AXIS);

  // Cur X
  motionAxisState.xValuePos.x = BTN_WIDTH + (TFT_WIDTH - X_MARGIN * 2 - 5 * BTN_WIDTH) / 4; //X- pos
  motionAxisState.xValuePos.y = y - 10;
  drawAxisValue(X_AXIS);

  x += BTN_WIDTH + spacing;
  drawBtn(x, y, "Y-", (intptr_t)y_minus, imgDown, Y_BTN_COLOR, !busy);

  x += BTN_WIDTH + spacing;
  drawBtn(x, y, "Z+", (intptr_t)z_plus, imgDown, Z_BTN_COLOR, !busy || ENABLED(BABYSTEP_ZPROBE_OFFSET)); //only enabled when not busy or have baby step

  // Cur Z
  motionAxisState.zValuePos.x = x;
  motionAxisState.zValuePos.y = y + BTN_HEIGHT + 2;
  drawAxisValue(Z_AXIS);

  // ROW 4 -> step_size  disable steppers back
  y = TFT_HEIGHT - Y_MARGIN - 32; //
  x = TFT_WIDTH / 2 - CUR_STEP_VALUE_WIDTH / 2;
  motionAxisState.stepValuePos.x = x;
  motionAxisState.stepValuePos.y = y;
  if (!busy) {
    drawCurStepValue();
    TERN_(HAS_TFT_XPT2046, touch.add_control(BUTTON, motionAxisState.stepValuePos.x, motionAxisState.stepValuePos.y, CUR_STEP_VALUE_WIDTH, BTN_HEIGHT, (intptr_t)step_size));
  }

  // aligned with x+
  drawBtn(xplus_x, TFT_HEIGHT - Y_MARGIN - BTN_HEIGHT, "off", (intptr_t)disable_steppers, imgCancel, COLOR_WHITE, !busy);

  TERN_(HAS_TFT_XPT2046, add_control(TFT_WIDTH - X_MARGIN - BTN_WIDTH, y, BACK, imgBack));
}

#endif // HAS_UI_480x320
