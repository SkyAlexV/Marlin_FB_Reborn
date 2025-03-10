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

/**
 * stepper/indirection.h
 *
 * Stepper motor driver indirection to allow some stepper functions to
 * be done via SPI/I2c instead of direct pin manipulation.
 *
 * Copyright (c) 2015 Dominik Wenger
 */

#include "../../inc/MarlinConfig.h"
#include "../planner.h"

#if HAS_L64XX
  #include "L64xx.h"
#endif

#if HAS_TMC26X
  #include "TMC26X.h"
#endif

#if HAS_TRINAMIC_CONFIG
  #include "trinamic.h"
#endif

void restore_stepper_drivers();  // Called by powerManager.power_on()
void reset_stepper_drivers();    // Called by settings.load / settings.reset

// X Stepper
#ifndef X_ENABLE_INIT
  #define X_ENABLE_INIT() SET_OUTPUT(X_ENABLE_PIN)
  #define X_ENABLE_WRITE(STATE) WRITE(X_ENABLE_PIN,STATE)
  #define X_ENABLE_READ() bool(READ(X_ENABLE_PIN))
#endif
#ifndef X_DIR_INIT
  #define X_DIR_INIT() SET_OUTPUT(X_DIR_PIN)
  #define X_DIR_WRITE(STATE) WRITE(X_DIR_PIN,STATE)
  #define X_DIR_READ() bool(READ(X_DIR_PIN))
#endif
#define X_STEP_INIT() SET_OUTPUT(X_STEP_PIN)
#ifndef X_STEP_WRITE
  #define X_STEP_WRITE(STATE) WRITE(X_STEP_PIN,STATE)
#endif
#define X_STEP_READ() bool(READ(X_STEP_PIN))

// Y Stepper
#if HAS_Y_AXIS
  #ifndef Y_ENABLE_INIT
    #define Y_ENABLE_INIT() SET_OUTPUT(Y_ENABLE_PIN)
    #define Y_ENABLE_WRITE(STATE) WRITE(Y_ENABLE_PIN,STATE)
    #define Y_ENABLE_READ() bool(READ(Y_ENABLE_PIN))
  #endif
  #ifndef Y_DIR_INIT
    #define Y_DIR_INIT() SET_OUTPUT(Y_DIR_PIN)
    #define Y_DIR_WRITE(STATE) WRITE(Y_DIR_PIN,STATE)
    #define Y_DIR_READ() bool(READ(Y_DIR_PIN))
  #endif
  #define Y_STEP_INIT() SET_OUTPUT(Y_STEP_PIN)
  #ifndef Y_STEP_WRITE
    #define Y_STEP_WRITE(STATE) WRITE(Y_STEP_PIN,STATE)
  #endif
  #define Y_STEP_READ() bool(READ(Y_STEP_PIN))
#endif

// Z Stepper
#if HAS_Z_AXIS
  #ifndef Z_ENABLE_INIT
    #define Z_ENABLE_INIT() SET_OUTPUT(Z_ENABLE_PIN)
    #define Z_ENABLE_WRITE(STATE) WRITE(Z_ENABLE_PIN,STATE)
    #define Z_ENABLE_READ() bool(READ(Z_ENABLE_PIN))
  #endif
  #ifndef Z_DIR_INIT
    #define Z_DIR_INIT() SET_OUTPUT(Z_DIR_PIN)
    #define Z_DIR_WRITE(STATE) WRITE(Z_DIR_PIN,STATE)
    #define Z_DIR_READ() bool(READ(Z_DIR_PIN))
  #endif
  #define Z_STEP_INIT() SET_OUTPUT(Z_STEP_PIN)
  #ifndef Z_STEP_WRITE
    #define Z_STEP_WRITE(STATE) WRITE(Z_STEP_PIN,STATE)
  #endif
  #define Z_STEP_READ() bool(READ(Z_STEP_PIN))
#endif

// X2 Stepper
#if HAS_X2_ENABLE
  #ifndef X2_ENABLE_INIT
    #define X2_ENABLE_INIT() SET_OUTPUT(X2_ENABLE_PIN)
    #define X2_ENABLE_WRITE(STATE) WRITE(X2_ENABLE_PIN,STATE)
    #define X2_ENABLE_READ() bool(READ(X2_ENABLE_PIN))
  #endif
  #ifndef X2_DIR_INIT
    #define X2_DIR_INIT() SET_OUTPUT(X2_DIR_PIN)
    #define X2_DIR_WRITE(STATE) WRITE(X2_DIR_PIN,STATE)
    #define X2_DIR_READ() bool(READ(X2_DIR_PIN))
  #endif
  #define X2_STEP_INIT() SET_OUTPUT(X2_STEP_PIN)
  #ifndef X2_STEP_WRITE
    #define X2_STEP_WRITE(STATE) WRITE(X2_STEP_PIN,STATE)
  #endif
  #define X2_STEP_READ() bool(READ(X2_STEP_PIN))
#endif

// Y2 Stepper
#if HAS_Y2_ENABLE
  #ifndef Y2_ENABLE_INIT
    #define Y2_ENABLE_INIT() SET_OUTPUT(Y2_ENABLE_PIN)
    #define Y2_ENABLE_WRITE(STATE) WRITE(Y2_ENABLE_PIN,STATE)
    #define Y2_ENABLE_READ() bool(READ(Y2_ENABLE_PIN))
  #endif
  #ifndef Y2_DIR_INIT
    #define Y2_DIR_INIT() SET_OUTPUT(Y2_DIR_PIN)
    #define Y2_DIR_WRITE(STATE) WRITE(Y2_DIR_PIN,STATE)
    #define Y2_DIR_READ() bool(READ(Y2_DIR_PIN))
  #endif
  #define Y2_STEP_INIT() SET_OUTPUT(Y2_STEP_PIN)
  #ifndef Y2_STEP_WRITE
    #define Y2_STEP_WRITE(STATE) WRITE(Y2_STEP_PIN,STATE)
  #endif
  #define Y2_STEP_READ() bool(READ(Y2_STEP_PIN))
#else
  #define Y2_DIR_WRITE(STATE) NOOP
#endif

// Z2 Stepper
#if HAS_Z2_ENABLE
  #ifndef Z2_ENABLE_INIT
    #define Z2_ENABLE_INIT() SET_OUTPUT(Z2_ENABLE_PIN)
    #define Z2_ENABLE_WRITE(STATE) WRITE(Z2_ENABLE_PIN,STATE)
    #define Z2_ENABLE_READ() bool(READ(Z2_ENABLE_PIN))
  #endif
  #ifndef Z2_DIR_INIT
    #define Z2_DIR_INIT() SET_OUTPUT(Z2_DIR_PIN)
    #define Z2_DIR_WRITE(STATE) WRITE(Z2_DIR_PIN,STATE)
    #define Z2_DIR_READ() bool(READ(Z2_DIR_PIN))
  #endif
  #define Z2_STEP_INIT() SET_OUTPUT(Z2_STEP_PIN)
  #ifndef Z2_STEP_WRITE
    #define Z2_STEP_WRITE(STATE) WRITE(Z2_STEP_PIN,STATE)
  #endif
  #define Z2_STEP_READ() bool(READ(Z2_STEP_PIN))
#else
  #define Z2_DIR_WRITE(STATE) NOOP
#endif

// Z3 Stepper
#if HAS_Z3_ENABLE
  #ifndef Z3_ENABLE_INIT
    #define Z3_ENABLE_INIT() SET_OUTPUT(Z3_ENABLE_PIN)
    #define Z3_ENABLE_WRITE(STATE) WRITE(Z3_ENABLE_PIN,STATE)
    #define Z3_ENABLE_READ() bool(READ(Z3_ENABLE_PIN))
  #endif
  #ifndef Z3_DIR_INIT
    #define Z3_DIR_INIT() SET_OUTPUT(Z3_DIR_PIN)
    #define Z3_DIR_WRITE(STATE) WRITE(Z3_DIR_PIN,STATE)
    #define Z3_DIR_READ() bool(READ(Z3_DIR_PIN))
  #endif
  #define Z3_STEP_INIT() SET_OUTPUT(Z3_STEP_PIN)
  #ifndef Z3_STEP_WRITE
    #define Z3_STEP_WRITE(STATE) WRITE(Z3_STEP_PIN,STATE)
  #endif
  #define Z3_STEP_READ() bool(READ(Z3_STEP_PIN))
#else
  #define Z3_DIR_WRITE(STATE) NOOP
#endif

// Z4 Stepper
#if HAS_Z4_ENABLE
  #ifndef Z4_ENABLE_INIT
    #define Z4_ENABLE_INIT() SET_OUTPUT(Z4_ENABLE_PIN)
    #define Z4_ENABLE_WRITE(STATE) WRITE(Z4_ENABLE_PIN,STATE)
    #define Z4_ENABLE_READ() bool(READ(Z4_ENABLE_PIN))
  #endif
  #ifndef Z4_DIR_INIT
    #define Z4_DIR_INIT() SET_OUTPUT(Z4_DIR_PIN)
    #define Z4_DIR_WRITE(STATE) WRITE(Z4_DIR_PIN,STATE)
    #define Z4_DIR_READ() bool(READ(Z4_DIR_PIN))
  #endif
  #define Z4_STEP_INIT() SET_OUTPUT(Z4_STEP_PIN)
  #ifndef Z4_STEP_WRITE
    #define Z4_STEP_WRITE(STATE) WRITE(Z4_STEP_PIN,STATE)
  #endif
  #define Z4_STEP_READ() bool(READ(Z4_STEP_PIN))
#else
  #define Z4_DIR_WRITE(STATE) NOOP
#endif

// I Stepper
#if HAS_I_AXIS
  #ifndef I_ENABLE_INIT
    #define I_ENABLE_INIT() SET_OUTPUT(I_ENABLE_PIN)
    #define I_ENABLE_WRITE(STATE) WRITE(I_ENABLE_PIN,STATE)
    #define I_ENABLE_READ() bool(READ(I_ENABLE_PIN))
  #endif
  #ifndef I_DIR_INIT
    #define I_DIR_INIT() SET_OUTPUT(I_DIR_PIN)
    #define I_DIR_WRITE(STATE) WRITE(I_DIR_PIN,STATE)
    #define I_DIR_READ() bool(READ(I_DIR_PIN))
  #endif
  #define I_STEP_INIT() SET_OUTPUT(I_STEP_PIN)
  #ifndef I_STEP_WRITE
    #define I_STEP_WRITE(STATE) WRITE(I_STEP_PIN,STATE)
  #endif
  #define I_STEP_READ() bool(READ(I_STEP_PIN))
#endif

// J Stepper
#if HAS_J_AXIS
  #ifndef J_ENABLE_INIT
    #define J_ENABLE_INIT() SET_OUTPUT(J_ENABLE_PIN)
    #define J_ENABLE_WRITE(STATE) WRITE(J_ENABLE_PIN,STATE)
    #define J_ENABLE_READ() bool(READ(J_ENABLE_PIN))
  #endif
  #ifndef J_DIR_INIT
    #define J_DIR_INIT() SET_OUTPUT(J_DIR_PIN)
    #define J_DIR_WRITE(STATE) WRITE(J_DIR_PIN,STATE)
    #define J_DIR_READ() bool(READ(J_DIR_PIN))
  #endif
  #define J_STEP_INIT() SET_OUTPUT(J_STEP_PIN)
  #ifndef J_STEP_WRITE
    #define J_STEP_WRITE(STATE) WRITE(J_STEP_PIN,STATE)
  #endif
  #define J_STEP_READ() bool(READ(J_STEP_PIN))
#endif

// K Stepper
#if HAS_K_AXIS
  #ifndef K_ENABLE_INIT
    #define K_ENABLE_INIT() SET_OUTPUT(K_ENABLE_PIN)
    #define K_ENABLE_WRITE(STATE) WRITE(K_ENABLE_PIN,STATE)
    #define K_ENABLE_READ() bool(READ(K_ENABLE_PIN))
  #endif
  #ifndef K_DIR_INIT
    #define K_DIR_INIT() SET_OUTPUT(K_DIR_PIN)
    #define K_DIR_WRITE(STATE) WRITE(K_DIR_PIN,STATE)
    #define K_DIR_READ() bool(READ(K_DIR_PIN))
  #endif
  #define K_STEP_INIT() SET_OUTPUT(K_STEP_PIN)
  #ifndef K_STEP_WRITE
    #define K_STEP_WRITE(STATE) WRITE(K_STEP_PIN,STATE)
  #endif
  #define K_STEP_READ() bool(READ(K_STEP_PIN))
#endif

// E0 Stepper
#ifndef E0_ENABLE_INIT
  #define E0_ENABLE_INIT() SET_OUTPUT(E0_ENABLE_PIN)
  #define E0_ENABLE_WRITE(STATE) WRITE(E0_ENABLE_PIN,STATE)
  #define E0_ENABLE_READ() bool(READ(E0_ENABLE_PIN))
#endif
#ifndef E0_DIR_INIT
  #define E0_DIR_INIT() SET_OUTPUT(E0_DIR_PIN)
  #define E0_DIR_WRITE(STATE) WRITE(E0_DIR_PIN,STATE)
  #define E0_DIR_READ() bool(READ(E0_DIR_PIN))
#endif
#define E0_STEP_INIT() SET_OUTPUT(E0_STEP_PIN)
#ifndef E0_STEP_WRITE
  #define E0_STEP_WRITE(STATE) WRITE(E0_STEP_PIN,STATE)
#endif
#define E0_STEP_READ() bool(READ(E0_STEP_PIN))

// E1 Stepper
#ifndef E1_ENABLE_INIT
  #define E1_ENABLE_INIT() SET_OUTPUT(E1_ENABLE_PIN)
  #define E1_ENABLE_WRITE(STATE) WRITE(E1_ENABLE_PIN,STATE)
  #define E1_ENABLE_READ() bool(READ(E1_ENABLE_PIN))
#endif
#ifndef E1_DIR_INIT
  #define E1_DIR_INIT() SET_OUTPUT(E1_DIR_PIN)
  #define E1_DIR_WRITE(STATE) WRITE(E1_DIR_PIN,STATE)
  #define E1_DIR_READ() bool(READ(E1_DIR_PIN))
#endif
#define E1_STEP_INIT() SET_OUTPUT(E1_STEP_PIN)
#ifndef E1_STEP_WRITE
  #define E1_STEP_WRITE(STATE) WRITE(E1_STEP_PIN,STATE)
#endif
#define E1_STEP_READ() bool(READ(E1_STEP_PIN))

// E2 Stepper
#ifndef E2_ENABLE_INIT
  #define E2_ENABLE_INIT() SET_OUTPUT(E2_ENABLE_PIN)
  #define E2_ENABLE_WRITE(STATE) WRITE(E2_ENABLE_PIN,STATE)
  #define E2_ENABLE_READ() bool(READ(E2_ENABLE_PIN))
#endif
#ifndef E2_DIR_INIT
  #define E2_DIR_INIT() SET_OUTPUT(E2_DIR_PIN)
  #define E2_DIR_WRITE(STATE) WRITE(E2_DIR_PIN,STATE)
  #define E2_DIR_READ() bool(READ(E2_DIR_PIN))
#endif
#define E2_STEP_INIT() SET_OUTPUT(E2_STEP_PIN)
#ifndef E2_STEP_WRITE
  #define E2_STEP_WRITE(STATE) WRITE(E2_STEP_PIN,STATE)
#endif
#define E2_STEP_READ() bool(READ(E2_STEP_PIN))

// E3 Stepper
#ifndef E3_ENABLE_INIT
  #define E3_ENABLE_INIT() SET_OUTPUT(E3_ENABLE_PIN)
  #define E3_ENABLE_WRITE(STATE) WRITE(E3_ENABLE_PIN,STATE)
  #define E3_ENABLE_READ() bool(READ(E3_ENABLE_PIN))
#endif
#ifndef E3_DIR_INIT
  #define E3_DIR_INIT() SET_OUTPUT(E3_DIR_PIN)
  #define E3_DIR_WRITE(STATE) WRITE(E3_DIR_PIN,STATE)
  #define E3_DIR_READ() bool(READ(E3_DIR_PIN))
#endif
#define E3_STEP_INIT() SET_OUTPUT(E3_STEP_PIN)
#ifndef E3_STEP_WRITE
  #define E3_STEP_WRITE(STATE) WRITE(E3_STEP_PIN,STATE)
#endif
#define E3_STEP_READ() bool(READ(E3_STEP_PIN))

// E4 Stepper
#ifndef E4_ENABLE_INIT
  #define E4_ENABLE_INIT() SET_OUTPUT(E4_ENABLE_PIN)
  #define E4_ENABLE_WRITE(STATE) WRITE(E4_ENABLE_PIN,STATE)
  #define E4_ENABLE_READ() bool(READ(E4_ENABLE_PIN))
#endif
#ifndef E4_DIR_INIT
  #define E4_DIR_INIT() SET_OUTPUT(E4_DIR_PIN)
  #define E4_DIR_WRITE(STATE) WRITE(E4_DIR_PIN,STATE)
  #define E4_DIR_READ() bool(READ(E4_DIR_PIN))
#endif
#define E4_STEP_INIT() SET_OUTPUT(E4_STEP_PIN)
#ifndef E4_STEP_WRITE
  #define E4_STEP_WRITE(STATE) WRITE(E4_STEP_PIN,STATE)
#endif
#define E4_STEP_READ() bool(READ(E4_STEP_PIN))

// E5 Stepper
#ifndef E5_ENABLE_INIT
  #define E5_ENABLE_INIT() SET_OUTPUT(E5_ENABLE_PIN)
  #define E5_ENABLE_WRITE(STATE) WRITE(E5_ENABLE_PIN,STATE)
  #define E5_ENABLE_READ() bool(READ(E5_ENABLE_PIN))
#endif
#ifndef E5_DIR_INIT
  #define E5_DIR_INIT() SET_OUTPUT(E5_DIR_PIN)
  #define E5_DIR_WRITE(STATE) WRITE(E5_DIR_PIN,STATE)
  #define E5_DIR_READ() bool(READ(E5_DIR_PIN))
#endif
#define E5_STEP_INIT() SET_OUTPUT(E5_STEP_PIN)
#ifndef E5_STEP_WRITE
  #define E5_STEP_WRITE(STATE) WRITE(E5_STEP_PIN,STATE)
#endif
#define E5_STEP_READ() bool(READ(E5_STEP_PIN))

// E6 Stepper
#ifndef E6_ENABLE_INIT
  #define E6_ENABLE_INIT() SET_OUTPUT(E6_ENABLE_PIN)
  #define E6_ENABLE_WRITE(STATE) WRITE(E6_ENABLE_PIN,STATE)
  #define E6_ENABLE_READ() bool(READ(E6_ENABLE_PIN))
#endif
#ifndef E6_DIR_INIT
  #define E6_DIR_INIT() SET_OUTPUT(E6_DIR_PIN)
  #define E6_DIR_WRITE(STATE) WRITE(E6_DIR_PIN,STATE)
  #define E6_DIR_READ() bool(READ(E6_DIR_PIN))
#endif
#define E6_STEP_INIT() SET_OUTPUT(E6_STEP_PIN)
#ifndef E6_STEP_WRITE
  #define E6_STEP_WRITE(STATE) WRITE(E6_STEP_PIN,STATE)
#endif
#define E6_STEP_READ() bool(READ(E6_STEP_PIN))

// E7 Stepper
#ifndef E7_ENABLE_INIT
  #define E7_ENABLE_INIT() SET_OUTPUT(E7_ENABLE_PIN)
  #define E7_ENABLE_WRITE(STATE) WRITE(E7_ENABLE_PIN,STATE)
  #define E7_ENABLE_READ() bool(READ(E7_ENABLE_PIN))
#endif
#ifndef E7_DIR_INIT
  #define E7_DIR_INIT() SET_OUTPUT(E7_DIR_PIN)
  #define E7_DIR_WRITE(STATE) WRITE(E7_DIR_PIN,STATE)
  #define E7_DIR_READ() bool(READ(E7_DIR_PIN))
#endif
#define E7_STEP_INIT() SET_OUTPUT(E7_STEP_PIN)
#ifndef E7_STEP_WRITE
  #define E7_STEP_WRITE(STATE) WRITE(E7_STEP_PIN,STATE)
#endif
#define E7_STEP_READ() bool(READ(E7_STEP_PIN))

/**
 * Extruder indirection for the single E axis
 */
#if ENABLED(SWITCHING_EXTRUDER) // One stepper driver per two extruders, reversed on odd index
  #if EXTRUDERS > 7
    #define E_STEP_WRITE(E,V) do{ if (E < 2) { E0_STEP_WRITE(V); } else if (E < 4) { E1_STEP_WRITE(V); } else if (E < 6) { E2_STEP_WRITE(V); } else { E3_STEP_WRITE(V); } }while(0)
    #define   NORM_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E0_DIR_WRITE( INVERT_E0_DIR); break; \
        case 2: E1_DIR_WRITE(!INVERT_E1_DIR); break; case 3: E1_DIR_WRITE( INVERT_E1_DIR); break; \
        case 4: E2_DIR_WRITE(!INVERT_E2_DIR); break; case 5: E2_DIR_WRITE( INVERT_E2_DIR); break; \
        case 6: E3_DIR_WRITE( INVERT_E3_DIR); break; case 7: E3_DIR_WRITE( INVERT_E3_DIR); break; \
      } }while(0)
    #define    REV_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E0_DIR_WRITE(!INVERT_E0_DIR); break; \
        case 2: E1_DIR_WRITE( INVERT_E1_DIR); break; case 3: E1_DIR_WRITE(!INVERT_E1_DIR); break; \
        case 4: E2_DIR_WRITE( INVERT_E2_DIR); break; case 5: E2_DIR_WRITE(!INVERT_E2_DIR); break; \
        case 6: E3_DIR_WRITE(!INVERT_E3_DIR); break; case 7: E3_DIR_WRITE(!INVERT_E3_DIR); break; \
      } }while(0)
  #elif EXTRUDERS > 6
    #define E_STEP_WRITE(E,V) do{ if (E < 2) { E0_STEP_WRITE(V); } else if (E < 4) { E1_STEP_WRITE(V); } else if (E < 6) { E2_STEP_WRITE(V); } else { E3_STEP_WRITE(V); } }while(0)
    #define   NORM_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E0_DIR_WRITE( INVERT_E0_DIR); break; \
        case 2: E1_DIR_WRITE(!INVERT_E1_DIR); break; case 3: E1_DIR_WRITE( INVERT_E1_DIR); break; \
        case 4: E2_DIR_WRITE(!INVERT_E2_DIR); break; case 5: E2_DIR_WRITE( INVERT_E2_DIR); break; \
        case 6: E3_DIR_WRITE( INVERT_E3_DIR); break; \
      } }while(0)
    #define    REV_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E0_DIR_WRITE(!INVERT_E0_DIR); break; \
        case 2: E1_DIR_WRITE( INVERT_E1_DIR); break; case 3: E1_DIR_WRITE(!INVERT_E1_DIR); break; \
        case 4: E2_DIR_WRITE( INVERT_E2_DIR); break; case 5: E2_DIR_WRITE(!INVERT_E2_DIR); break; \
        case 6: E3_DIR_WRITE(!INVERT_E3_DIR); } }while(0)
  #elif EXTRUDERS > 5
    #define E_STEP_WRITE(E,V) do{ if (E < 2) { E0_STEP_WRITE(V); } else if (E < 4) { E1_STEP_WRITE(V); } else { E2_STEP_WRITE(V); } }while(0)
    #define   NORM_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E0_DIR_WRITE( INVERT_E0_DIR); break; \
        case 2: E1_DIR_WRITE(!INVERT_E1_DIR); break; case 3: E1_DIR_WRITE( INVERT_E1_DIR); break; \
        case 4: E2_DIR_WRITE(!INVERT_E2_DIR); break; case 5: E2_DIR_WRITE( INVERT_E2_DIR); break; \
      } }while(0)
    #define    REV_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E0_DIR_WRITE(!INVERT_E0_DIR); break; \
        case 2: E1_DIR_WRITE( INVERT_E1_DIR); break; case 3: E1_DIR_WRITE(!INVERT_E1_DIR); break; \
        case 4: E2_DIR_WRITE( INVERT_E2_DIR); break; case 5: E2_DIR_WRITE(!INVERT_E2_DIR); break; \
      } }while(0)
  #elif EXTRUDERS > 4
    #define E_STEP_WRITE(E,V) do{ if (E < 2) { E0_STEP_WRITE(V); } else if (E < 4) { E1_STEP_WRITE(V); } else { E2_STEP_WRITE(V); } }while(0)
    #define   NORM_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E0_DIR_WRITE( INVERT_E0_DIR); break; \
        case 2: E1_DIR_WRITE(!INVERT_E1_DIR); break; case 3: E1_DIR_WRITE( INVERT_E1_DIR); break; \
        case 4: E2_DIR_WRITE(!INVERT_E2_DIR); break; \
      } }while(0)
    #define    REV_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E0_DIR_WRITE(!INVERT_E0_DIR); break; \
        case 2: E1_DIR_WRITE( INVERT_E1_DIR); break; case 3: E1_DIR_WRITE(!INVERT_E1_DIR); break; \
        case 4: E2_DIR_WRITE( INVERT_E2_DIR); break; \
      } }while(0)
  #elif EXTRUDERS > 3
    #define E_STEP_WRITE(E,V) do{ if (E < 2) { E0_STEP_WRITE(V); } else { E1_STEP_WRITE(V); } }while(0)
    #define   NORM_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E0_DIR_WRITE( INVERT_E0_DIR); break; \
        case 2: E1_DIR_WRITE(!INVERT_E1_DIR); break; case 3: E1_DIR_WRITE( INVERT_E1_DIR); break; \
      } }while(0)
    #define    REV_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E0_DIR_WRITE(!INVERT_E0_DIR); break; \
        case 2: E1_DIR_WRITE( INVERT_E1_DIR); break; case 3: E1_DIR_WRITE(!INVERT_E1_DIR); break; \
      } }while(0)
  #elif EXTRUDERS > 2
    #define E_STEP_WRITE(E,V) do{ if (E < 2) { E0_STEP_WRITE(V); } else { E1_STEP_WRITE(V); } }while(0)
    #define   NORM_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E0_DIR_WRITE( INVERT_E0_DIR); break; \
        case 2: E1_DIR_WRITE(!INVERT_E1_DIR); break; \
      } }while(0)
    #define    REV_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E0_DIR_WRITE(!INVERT_E0_DIR); break; \
        case 2: E1_DIR_WRITE( INVERT_E1_DIR); break; \
      } }while(0)
  #else
    #define E_STEP_WRITE(E,V) E0_STEP_WRITE(V)
    #define   NORM_E_DIR(E)   do{ E0_DIR_WRITE(E ?  INVERT_E0_DIR : !INVERT_E0_DIR); }while(0)
    #define    REV_E_DIR(E)   do{ E0_DIR_WRITE(E ? !INVERT_E0_DIR :  INVERT_E0_DIR); }while(0)
  #endif

#elif HAS_PRUSA_MMU2  // One multiplexed stepper driver

  #define E_STEP_WRITE(E,V) E0_STEP_WRITE(V)
  #define   NORM_E_DIR(E)   E0_DIR_WRITE(!INVERT_E0_DIR)
  #define    REV_E_DIR(E)   E0_DIR_WRITE( INVERT_E0_DIR)

#elif HAS_PRUSA_MMU1  // One multiplexed stepper driver, reversed on odd index

  #define E_STEP_WRITE(E,V) E0_STEP_WRITE(V)
  #define   NORM_E_DIR(E)   do{ E0_DIR_WRITE(TEST(E, 0) ? !INVERT_E0_DIR:  INVERT_E0_DIR); }while(0)
  #define    REV_E_DIR(E)   do{ E0_DIR_WRITE(TEST(E, 0) ?  INVERT_E0_DIR: !INVERT_E0_DIR); }while(0)

#elif E_STEPPERS > 1

  #if E_STEPPERS > 7

    #define _E_STEP_WRITE(E,V) do{ switch (E) { \
        case 0: E0_STEP_WRITE(V); break; case 1: E1_STEP_WRITE(V); break; case 2: E2_STEP_WRITE(V); break; case 3: E3_STEP_WRITE(V); break; \
        case 4: E4_STEP_WRITE(V); break; case 5: E5_STEP_WRITE(V); break; case 6: E6_STEP_WRITE(V); break; case 7: E7_STEP_WRITE(V); break; \
      } }while(0)
    #define   _NORM_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E1_DIR_WRITE(!INVERT_E1_DIR); break; \
        case 2: E2_DIR_WRITE(!INVERT_E2_DIR); break; case 3: E3_DIR_WRITE(!INVERT_E3_DIR); break; \
        case 4: E4_DIR_WRITE(!INVERT_E4_DIR); break; case 5: E5_DIR_WRITE(!INVERT_E5_DIR); break; \
        case 6: E6_DIR_WRITE(!INVERT_E6_DIR); break; case 7: E7_DIR_WRITE(!INVERT_E7_DIR); break; \
      } }while(0)
    #define    _REV_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E1_DIR_WRITE( INVERT_E1_DIR); break; \
        case 2: E2_DIR_WRITE( INVERT_E2_DIR); break; case 3: E3_DIR_WRITE( INVERT_E3_DIR); break; \
        case 4: E4_DIR_WRITE( INVERT_E4_DIR); break; case 5: E5_DIR_WRITE( INVERT_E5_DIR); break; \
        case 6: E6_DIR_WRITE( INVERT_E6_DIR); break; case 7: E7_DIR_WRITE( INVERT_E7_DIR); break; \
      } }while(0)

  #elif E_STEPPERS > 6

    #define _E_STEP_WRITE(E,V) do{ switch (E) { \
        case 0: E0_STEP_WRITE(V); break; case 1: E1_STEP_WRITE(V); break; case 2: E2_STEP_WRITE(V); break; case 3: E3_STEP_WRITE(V); break; \
        case 4: E4_STEP_WRITE(V); break; case 5: E5_STEP_WRITE(V); break; case 6: E6_STEP_WRITE(V); break; \
      } }while(0)
    #define   _NORM_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E1_DIR_WRITE(!INVERT_E1_DIR); break; \
        case 2: E2_DIR_WRITE(!INVERT_E2_DIR); break; case 3: E3_DIR_WRITE(!INVERT_E3_DIR); break; \
        case 4: E4_DIR_WRITE(!INVERT_E4_DIR); break; case 5: E5_DIR_WRITE(!INVERT_E5_DIR); break; \
        case 6: E6_DIR_WRITE(!INVERT_E6_DIR); break; \
      } }while(0)
    #define    _REV_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E1_DIR_WRITE( INVERT_E1_DIR); break; \
        case 2: E2_DIR_WRITE( INVERT_E2_DIR); break; case 3: E3_DIR_WRITE( INVERT_E3_DIR); break; \
        case 4: E4_DIR_WRITE( INVERT_E4_DIR); break; case 5: E5_DIR_WRITE( INVERT_E5_DIR); break; \
        case 6: E6_DIR_WRITE( INVERT_E6_DIR); break; \
      } }while(0)

  #elif E_STEPPERS > 5

    #define _E_STEP_WRITE(E,V) do{ switch (E) { \
        case 0: E0_STEP_WRITE(V); break; case 1: E1_STEP_WRITE(V); break; case 2: E2_STEP_WRITE(V); break; case 3: E3_STEP_WRITE(V); break; \
        case 4: E4_STEP_WRITE(V); break; case 5: E5_STEP_WRITE(V); break; \
      } }while(0)
    #define   _NORM_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E1_DIR_WRITE(!INVERT_E1_DIR); break; \
        case 2: E2_DIR_WRITE(!INVERT_E2_DIR); break; case 3: E3_DIR_WRITE(!INVERT_E3_DIR); break; \
        case 4: E4_DIR_WRITE(!INVERT_E4_DIR); break; case 5: E5_DIR_WRITE(!INVERT_E5_DIR); break; \
      } }while(0)
    #define    _REV_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E1_DIR_WRITE( INVERT_E1_DIR); break; \
        case 2: E2_DIR_WRITE( INVERT_E2_DIR); break; case 3: E3_DIR_WRITE( INVERT_E3_DIR); break; \
        case 4: E4_DIR_WRITE( INVERT_E4_DIR); break; case 5: E5_DIR_WRITE( INVERT_E5_DIR); break; \
      } }while(0)

  #elif E_STEPPERS > 4

    #define _E_STEP_WRITE(E,V) do{ switch (E) { \
        case 0: E0_STEP_WRITE(V); break; case 1: E1_STEP_WRITE(V); break; case 2: E2_STEP_WRITE(V); break; case 3: E3_STEP_WRITE(V); break; \
        case 4: E4_STEP_WRITE(V); break; \
      } }while(0)
    #define   _NORM_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E1_DIR_WRITE(!INVERT_E1_DIR); break; \
        case 2: E2_DIR_WRITE(!INVERT_E2_DIR); break; case 3: E3_DIR_WRITE(!INVERT_E3_DIR); break; \
        case 4: E4_DIR_WRITE(!INVERT_E4_DIR); break; \
      } }while(0)
    #define    _REV_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E1_DIR_WRITE( INVERT_E1_DIR); break; \
        case 2: E2_DIR_WRITE( INVERT_E2_DIR); break; case 3: E3_DIR_WRITE( INVERT_E3_DIR); break; \
        case 4: E4_DIR_WRITE( INVERT_E4_DIR); break; \
      } }while(0)

  #elif E_STEPPERS > 3

    #define _E_STEP_WRITE(E,V) do{ switch (E) { \
        case 0: E0_STEP_WRITE(V); break; case 1: E1_STEP_WRITE(V); break; case 2: E2_STEP_WRITE(V); break; case 3: E3_STEP_WRITE(V); break; \
      } }while(0)
    #define   _NORM_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E1_DIR_WRITE(!INVERT_E1_DIR); break; \
        case 2: E2_DIR_WRITE(!INVERT_E2_DIR); break; case 3: E3_DIR_WRITE(!INVERT_E3_DIR); break; \
      } }while(0)
    #define    _REV_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E1_DIR_WRITE( INVERT_E1_DIR); break; \
        case 2: E2_DIR_WRITE( INVERT_E2_DIR); break; case 3: E3_DIR_WRITE( INVERT_E3_DIR); break; \
      } }while(0)

  #elif E_STEPPERS > 2

    #define _E_STEP_WRITE(E,V) do{ switch (E) { case 0: E0_STEP_WRITE(V); break; case 1: E1_STEP_WRITE(V); break; case 2: E2_STEP_WRITE(V); } }while(0)
    #define   _NORM_E_DIR(E)   do{ switch (E) { case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E1_DIR_WRITE(!INVERT_E1_DIR); break; case 2: E2_DIR_WRITE(!INVERT_E2_DIR); } }while(0)
    #define    _REV_E_DIR(E)   do{ switch (E) { case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E1_DIR_WRITE( INVERT_E1_DIR); break; case 2: E2_DIR_WRITE( INVERT_E2_DIR); } }while(0)

  #else

    #define _E_STEP_WRITE(E,V) do{ if (E == 0) { E0_STEP_WRITE(V); } else { E1_STEP_WRITE(V); } }while(0)
    #define   _NORM_E_DIR(E)   do{ if (E == 0) { E0_DIR_WRITE(!INVERT_E0_DIR); } else { E1_DIR_WRITE(!INVERT_E1_DIR); } }while(0)
    #define    _REV_E_DIR(E)   do{ if (E == 0) { E0_DIR_WRITE( INVERT_E0_DIR); } else { E1_DIR_WRITE( INVERT_E1_DIR); } }while(0)
  #endif

  #if HAS_DUPLICATION_MODE

    #if ENABLED(MULTI_NOZZLE_DUPLICATION)
      #define _DUPE(N,T,V)  do{ if (TEST(duplication_e_mask, N)) E##N##_##T##_WRITE(V); }while(0)
    #else
      #define _DUPE(N,T,V)  E##N##_##T##_WRITE(V)
    #endif

    #define NDIR(N) _DUPE(N,DIR,!INVERT_E##N##_DIR)
    #define RDIR(N) _DUPE(N,DIR, INVERT_E##N##_DIR)

    #define E_STEP_WRITE(E,V) do{ if (extruder_duplication_enabled) { DUPE(STEP,V); } else _E_STEP_WRITE(E,V); }while(0)

    #if E_STEPPERS > 2
      #if E_STEPPERS > 7
        #define DUPE(T,V)     do{ _DUPE(0,T,V); _DUPE(1,T,V); _DUPE(2,T,V); _DUPE(3,T,V); _DUPE(4,T,V); _DUPE(5,T,V); _DUPE(6,T,V); _DUPE(7,T,V); }while(0)
        #define NORM_E_DIR(E) do{ if (extruder_duplication_enabled) { NDIR(0); NDIR(1); NDIR(2); NDIR(3); NDIR(4); NDIR(5); NDIR(6); NDIR(7); } else _NORM_E_DIR(E); }while(0)
        #define REV_E_DIR(E)  do{ if (extruder_duplication_enabled) { RDIR(0); RDIR(1); RDIR(2); RDIR(3); RDIR(4); RDIR(5); RDIR(6); RDIR(7); } else  _REV_E_DIR(E); }while(0)
      #elif E_STEPPERS > 6
        #define DUPE(T,V)     do{ _DUPE(0,T,V); _DUPE(1,T,V); _DUPE(2,T,V); _DUPE(3,T,V); _DUPE(4,T,V); _DUPE(5,T,V); _DUPE(6,T,V); }while(0)
        #define NORM_E_DIR(E) do{ if (extruder_duplication_enabled) { NDIR(0); NDIR(1); NDIR(2); NDIR(3); NDIR(4); NDIR(5); NDIR(6); } else _NORM_E_DIR(E); }while(0)
        #define REV_E_DIR(E)  do{ if (extruder_duplication_enabled) { RDIR(0); RDIR(1); RDIR(2); RDIR(3); RDIR(4); RDIR(5); RDIR(6); } else  _REV_E_DIR(E); }while(0)
      #elif E_STEPPERS > 5
        #define DUPE(T,V)     do{ _DUPE(0,T,V); _DUPE(1,T,V); _DUPE(2,T,V); _DUPE(3,T,V); _DUPE(4,T,V); _DUPE(5,T,V); }while(0)
        #define NORM_E_DIR(E) do{ if (extruder_duplication_enabled) { NDIR(0); NDIR(1); NDIR(2); NDIR(3); NDIR(4); NDIR(5); } else _NORM_E_DIR(E); }while(0)
        #define REV_E_DIR(E)  do{ if (extruder_duplication_enabled) { RDIR(0); RDIR(1); RDIR(2); RDIR(3); RDIR(4); RDIR(5); } else  _REV_E_DIR(E); }while(0)
      #elif E_STEPPERS > 4
        #define DUPE(T,V)     do{ _DUPE(0,T,V); _DUPE(1,T,V); _DUPE(2,T,V); _DUPE(3,T,V); _DUPE(4,T,V); }while(0)
        #define NORM_E_DIR(E) do{ if (extruder_duplication_enabled) { NDIR(0); NDIR(1); NDIR(2); NDIR(3); NDIR(4); } else _NORM_E_DIR(E); }while(0)
        #define REV_E_DIR(E)  do{ if (extruder_duplication_enabled) { RDIR(0); RDIR(1); RDIR(2); RDIR(3); RDIR(4); } else  _REV_E_DIR(E); }while(0)
      #elif E_STEPPERS > 3
        #define DUPE(T,V)     do{ _DUPE(0,T,V); _DUPE(1,T,V); _DUPE(2,T,V); _DUPE(3,T,V); }while(0)
        #define NORM_E_DIR(E) do{ if (extruder_duplication_enabled) { NDIR(0); NDIR(1); NDIR(2); NDIR(3); } else _NORM_E_DIR(E); }while(0)
        #define REV_E_DIR(E)  do{ if (extruder_duplication_enabled) { RDIR(0); RDIR(1); RDIR(2); RDIR(3); } else  _REV_E_DIR(E); }while(0)
      #else
        #define DUPE(T,V)     do{ _DUPE(0,T,V); _DUPE(1,T,V); _DUPE(2,T,V); }while(0)
        #define NORM_E_DIR(E) do{ if (extruder_duplication_enabled) { NDIR(0); NDIR(1); NDIR(2); } else _NORM_E_DIR(E); }while(0)
        #define REV_E_DIR(E)  do{ if (extruder_duplication_enabled) { RDIR(0); RDIR(1); RDIR(2); } else  _REV_E_DIR(E); }while(0)
      #endif
    #else
      #define DUPE(T,V)     do{ _DUPE(0,T,V); _DUPE(1,T,V); }while(0)
      #define NORM_E_DIR(E) do{ if (extruder_duplication_enabled) { NDIR(0); NDIR(1); } else _NORM_E_DIR(E); }while(0)
      #define REV_E_DIR(E)  do{ if (extruder_duplication_enabled) { RDIR(0); RDIR(1); } else  _REV_E_DIR(E); }while(0)
    #endif

  #else

    #define E_STEP_WRITE(E,V) _E_STEP_WRITE(E,V)
    #define   NORM_E_DIR(E)   _NORM_E_DIR(E)
    #define    REV_E_DIR(E)   _REV_E_DIR(E)

  #endif

#elif ENABLED(E_DUAL_STEPPER_DRIVERS)
  #define E_STEP_WRITE(E,V) do{ E0_STEP_WRITE(V); E1_STEP_WRITE(V); }while(0)
  #define   NORM_E_DIR(E)   do{ E0_DIR_WRITE(!INVERT_E0_DIR); E1_DIR_WRITE(!INVERT_E0_DIR ^ ENABLED(INVERT_E1_VS_E0_DIR)); }while(0)
  #define    REV_E_DIR(E)   do{ E0_DIR_WRITE( INVERT_E0_DIR); E1_DIR_WRITE( INVERT_E0_DIR ^ ENABLED(INVERT_E1_VS_E0_DIR)); }while(0)

#elif E_STEPPERS
  #define E_STEP_WRITE(E,V) E0_STEP_WRITE(V)
  #if ENABLED(RS_ADDSETTINGS)
    #define   NORM_E_DIR(E)   E0_DIR_WRITE(!planner.invert_axis.invert_axis[E_AXIS])
    #define    REV_E_DIR(E)   E0_DIR_WRITE( planner.invert_axis.invert_axis[E_AXIS])
  #else
  #define   NORM_E_DIR(E)   E0_DIR_WRITE(!INVERT_E0_DIR)
  #define    REV_E_DIR(E)   E0_DIR_WRITE( INVERT_E0_DIR)
  #endif  // RS_ADDSETTINGS

#else
  #define E_STEP_WRITE(E,V) NOOP
  #define   NORM_E_DIR(E)   NOOP
  #define    REV_E_DIR(E)   NOOP

#endif

//
// Individual stepper enable / disable macros
//

#ifndef ENABLE_STEPPER_X
  #define  ENABLE_STEPPER_X() TERN(HAS_X_ENABLE, X_ENABLE_WRITE( X_ENABLE_ON), NOOP)
#endif
#ifndef DISABLE_STEPPER_X
  #define DISABLE_STEPPER_X() TERN(HAS_X_ENABLE, X_ENABLE_WRITE(!X_ENABLE_ON), NOOP)
#endif

#ifndef ENABLE_STEPPER_X2
  #define  ENABLE_STEPPER_X2() TERN(HAS_X2_ENABLE, X2_ENABLE_WRITE( X_ENABLE_ON), NOOP)
#endif
#ifndef DISABLE_STEPPER_X2
  #define DISABLE_STEPPER_X2() TERN(HAS_X2_ENABLE, X2_ENABLE_WRITE(!X_ENABLE_ON), NOOP)
#endif

#ifndef ENABLE_STEPPER_Y
  #define  ENABLE_STEPPER_Y() TERN(HAS_Y_ENABLE, Y_ENABLE_WRITE( Y_ENABLE_ON), NOOP)
#endif
#ifndef DISABLE_STEPPER_Y
  #define DISABLE_STEPPER_Y() TERN(HAS_Y_ENABLE, Y_ENABLE_WRITE(!Y_ENABLE_ON), NOOP)
#endif

#ifndef ENABLE_STEPPER_Y2
  #define  ENABLE_STEPPER_Y2() TERN(HAS_Y2_ENABLE, Y2_ENABLE_WRITE( Y_ENABLE_ON), NOOP)
#endif
#ifndef DISABLE_STEPPER_Y2
  #define DISABLE_STEPPER_Y2() TERN(HAS_Y2_ENABLE, Y2_ENABLE_WRITE(!Y_ENABLE_ON), NOOP)
#endif

#ifndef ENABLE_STEPPER_Z
  #define  ENABLE_STEPPER_Z() TERN(HAS_Z_ENABLE, Z_ENABLE_WRITE( Z_ENABLE_ON), NOOP)
#endif
#ifndef DISABLE_STEPPER_Z
  #define DISABLE_STEPPER_Z() TERN(HAS_Z_ENABLE, Z_ENABLE_WRITE(!Z_ENABLE_ON), NOOP)
#endif

#ifndef ENABLE_STEPPER_Z2
  #define  ENABLE_STEPPER_Z2() TERN(HAS_Z2_ENABLE, Z2_ENABLE_WRITE( Z_ENABLE_ON), NOOP)
#endif
#ifndef DISABLE_STEPPER_Z2
  #define DISABLE_STEPPER_Z2() TERN(HAS_Z2_ENABLE, Z2_ENABLE_WRITE(!Z_ENABLE_ON), NOOP)
#endif

#ifndef ENABLE_STEPPER_Z3
  #define  ENABLE_STEPPER_Z3() TERN(HAS_Z3_ENABLE, Z3_ENABLE_WRITE( Z_ENABLE_ON), NOOP)
#endif
#ifndef DISABLE_STEPPER_Z3
  #define DISABLE_STEPPER_Z3() TERN(HAS_Z3_ENABLE, Z3_ENABLE_WRITE(!Z_ENABLE_ON), NOOP)
#endif

#ifndef ENABLE_STEPPER_Z4
  #define  ENABLE_STEPPER_Z4() TERN(HAS_Z4_ENABLE, Z4_ENABLE_WRITE( Z_ENABLE_ON), NOOP)
#endif
#ifndef DISABLE_STEPPER_Z4
  #define DISABLE_STEPPER_Z4() TERN(HAS_Z4_ENABLE, Z4_ENABLE_WRITE(!Z_ENABLE_ON), NOOP)
#endif

#ifndef ENABLE_STEPPER_I
  #define  ENABLE_STEPPER_I() TERN(HAS_I_ENABLE, I_ENABLE_WRITE( I_ENABLE_ON), NOOP)
#endif
#ifndef DISABLE_STEPPER_I
  #define DISABLE_STEPPER_I() TERN(HAS_I_ENABLE, I_ENABLE_WRITE(!I_ENABLE_ON), NOOP)
#endif

#ifndef ENABLE_STEPPER_J
  #define  ENABLE_STEPPER_J() TERN(HAS_J_ENABLE, J_ENABLE_WRITE( J_ENABLE_ON), NOOP)
#endif
#ifndef DISABLE_STEPPER_J
  #define DISABLE_STEPPER_J() TERN(HAS_J_ENABLE, J_ENABLE_WRITE(!J_ENABLE_ON), NOOP)
#endif

#ifndef ENABLE_STEPPER_K
  #define  ENABLE_STEPPER_K() TERN(HAS_K_ENABLE, K_ENABLE_WRITE( K_ENABLE_ON), NOOP)
#endif
#ifndef DISABLE_STEPPER_K
  #define DISABLE_STEPPER_K() TERN(HAS_K_ENABLE, K_ENABLE_WRITE(!K_ENABLE_ON), NOOP)
#endif

#ifndef ENABLE_STEPPER_E0
  #define  ENABLE_STEPPER_E0() TERN(HAS_E0_ENABLE, E0_ENABLE_WRITE( E_ENABLE_ON), NOOP)
#endif
#ifndef DISABLE_STEPPER_E0
  #define DISABLE_STEPPER_E0() TERN(HAS_E0_ENABLE, E0_ENABLE_WRITE(!E_ENABLE_ON), NOOP)
#endif

#ifndef ENABLE_STEPPER_E1
  #if (E_STEPPERS > 1 || ENABLED(E_DUAL_STEPPER_DRIVERS)) && HAS_E1_ENABLE
    #define  ENABLE_STEPPER_E1() E1_ENABLE_WRITE( E_ENABLE_ON)
  #else
    #define  ENABLE_STEPPER_E1() NOOP
  #endif
#endif
#ifndef DISABLE_STEPPER_E1
  #if (E_STEPPERS > 1 || ENABLED(E_DUAL_STEPPER_DRIVERS)) && HAS_E1_ENABLE
    #define DISABLE_STEPPER_E1() E1_ENABLE_WRITE(!E_ENABLE_ON)
  #else
    #define DISABLE_STEPPER_E1() NOOP
  #endif
#endif

#ifndef ENABLE_STEPPER_E2
  #if E_STEPPERS > 2 && HAS_E2_ENABLE
    #define  ENABLE_STEPPER_E2() E2_ENABLE_WRITE( E_ENABLE_ON)
  #else
    #define  ENABLE_STEPPER_E2() NOOP
  #endif
#endif
#ifndef DISABLE_STEPPER_E2
  #if E_STEPPERS > 2 && HAS_E2_ENABLE
    #define DISABLE_STEPPER_E2() E2_ENABLE_WRITE(!E_ENABLE_ON)
  #else
    #define DISABLE_STEPPER_E2() NOOP
  #endif
#endif

#ifndef ENABLE_STEPPER_E3
  #if E_STEPPERS > 3 && HAS_E3_ENABLE
    #define  ENABLE_STEPPER_E3() E3_ENABLE_WRITE( E_ENABLE_ON)
  #else
    #define  ENABLE_STEPPER_E3() NOOP
  #endif
#endif
#ifndef DISABLE_STEPPER_E3
  #if E_STEPPERS > 3 && HAS_E3_ENABLE
    #define DISABLE_STEPPER_E3() E3_ENABLE_WRITE(!E_ENABLE_ON)
  #else
    #define DISABLE_STEPPER_E3() NOOP
  #endif
#endif

#ifndef ENABLE_STEPPER_E4
  #if E_STEPPERS > 4 && HAS_E4_ENABLE
    #define  ENABLE_STEPPER_E4() E4_ENABLE_WRITE( E_ENABLE_ON)
  #else
    #define  ENABLE_STEPPER_E4() NOOP
  #endif
#endif
#ifndef DISABLE_STEPPER_E4
  #if E_STEPPERS > 4 && HAS_E4_ENABLE
    #define DISABLE_STEPPER_E4() E4_ENABLE_WRITE(!E_ENABLE_ON)
  #else
    #define DISABLE_STEPPER_E4() NOOP
  #endif
#endif

#ifndef ENABLE_STEPPER_E5
  #if E_STEPPERS > 5 && HAS_E5_ENABLE
    #define  ENABLE_STEPPER_E5() E5_ENABLE_WRITE( E_ENABLE_ON)
  #else
    #define  ENABLE_STEPPER_E5() NOOP
  #endif
#endif
#ifndef DISABLE_STEPPER_E5
  #if E_STEPPERS > 5 && HAS_E5_ENABLE
    #define DISABLE_STEPPER_E5() E5_ENABLE_WRITE(!E_ENABLE_ON)
  #else
    #define DISABLE_STEPPER_E5() NOOP
  #endif
#endif

#ifndef ENABLE_STEPPER_E6
  #if E_STEPPERS > 6 && HAS_E6_ENABLE
    #define  ENABLE_STEPPER_E6() E6_ENABLE_WRITE( E_ENABLE_ON)
  #else
    #define  ENABLE_STEPPER_E6() NOOP
  #endif
#endif
#ifndef DISABLE_STEPPER_E6
  #if E_STEPPERS > 6 && HAS_E6_ENABLE
    #define DISABLE_STEPPER_E6() E6_ENABLE_WRITE(!E_ENABLE_ON)
  #else
    #define DISABLE_STEPPER_E6() NOOP
  #endif
#endif

#ifndef ENABLE_STEPPER_E7
  #if E_STEPPERS > 7 && HAS_E7_ENABLE
    #define  ENABLE_STEPPER_E7() E7_ENABLE_WRITE( E_ENABLE_ON)
  #else
    #define  ENABLE_STEPPER_E7() NOOP
  #endif
#endif
#ifndef DISABLE_STEPPER_E7
  #if E_STEPPERS > 7 && HAS_E7_ENABLE
    #define DISABLE_STEPPER_E7() E7_ENABLE_WRITE(!E_ENABLE_ON)
  #else
    #define DISABLE_STEPPER_E7() NOOP
  #endif
#endif

//
// Axis steppers enable / disable macros
//
#if ENABLED(SOFTWARE_DRIVER_ENABLE)
  // Avoid expensive calls to enable / disable steppers
  extern xyz_bool_t axis_sw_enabled;
  #define SHOULD_ENABLE(N)  !axis_sw_enabled.N
  #define SHOULD_DISABLE(N)  axis_sw_enabled.N
  #define AFTER_CHANGE(N,TF) axis_sw_enabled.N = TF
#else
  #define SHOULD_ENABLE(N)   true
  #define SHOULD_DISABLE(N)  true
  #define AFTER_CHANGE(N,TF) NOOP
#endif

#define  ENABLE_AXIS_X() if (SHOULD_ENABLE(x))  {  ENABLE_STEPPER_X();  ENABLE_STEPPER_X2(); AFTER_CHANGE(x, true); }
#define DISABLE_AXIS_X() if (SHOULD_DISABLE(x)) { DISABLE_STEPPER_X(); DISABLE_STEPPER_X2(); AFTER_CHANGE(x, false); set_axis_untrusted(X_AXIS); }

#if HAS_Y_AXIS
  #define  ENABLE_AXIS_Y() if (SHOULD_ENABLE(y))  {  ENABLE_STEPPER_Y();  ENABLE_STEPPER_Y2(); AFTER_CHANGE(y, true); }
  #define DISABLE_AXIS_Y() if (SHOULD_DISABLE(y)) { DISABLE_STEPPER_Y(); DISABLE_STEPPER_Y2(); AFTER_CHANGE(y, false); set_axis_untrusted(Y_AXIS); }
#else
  #define  ENABLE_AXIS_Y() NOOP
  #define DISABLE_AXIS_Y() NOOP
#endif

#if HAS_Z_AXIS
  #define  ENABLE_AXIS_Z() if (SHOULD_ENABLE(z))  {  ENABLE_STEPPER_Z();  ENABLE_STEPPER_Z2();  ENABLE_STEPPER_Z3();  ENABLE_STEPPER_Z4(); AFTER_CHANGE(z, true); }
  #define DISABLE_AXIS_Z() if (SHOULD_DISABLE(z)) { DISABLE_STEPPER_Z(); DISABLE_STEPPER_Z2(); DISABLE_STEPPER_Z3(); DISABLE_STEPPER_Z4(); AFTER_CHANGE(z, false); set_axis_untrusted(Z_AXIS); Z_RESET(); }
#else
  #define  ENABLE_AXIS_Z() NOOP
  #define DISABLE_AXIS_Z() NOOP
#endif

#ifdef Z_IDLE_HEIGHT
  #define Z_RESET() do{ current_position.z = Z_IDLE_HEIGHT; sync_plan_position(); }while(0)
#else
  #define Z_RESET()
#endif

#if HAS_I_AXIS
  #define  ENABLE_AXIS_I() if (SHOULD_ENABLE(i))  {  ENABLE_STEPPER_I(); AFTER_CHANGE(i, true); }
  #define DISABLE_AXIS_I() if (SHOULD_DISABLE(i)) { DISABLE_STEPPER_I(); AFTER_CHANGE(i, false); set_axis_untrusted(I_AXIS); }
#else
  #define  ENABLE_AXIS_I() NOOP
  #define DISABLE_AXIS_I() NOOP
#endif
#if HAS_J_AXIS
  #define  ENABLE_AXIS_J() if (SHOULD_ENABLE(j))  {  ENABLE_STEPPER_J(); AFTER_CHANGE(j, true); }
  #define DISABLE_AXIS_J() if (SHOULD_DISABLE(j)) { DISABLE_STEPPER_J(); AFTER_CHANGE(j, false); set_axis_untrusted(J_AXIS); }
#else
  #define  ENABLE_AXIS_J() NOOP
  #define DISABLE_AXIS_J() NOOP
#endif
#if HAS_K_AXIS
  #define  ENABLE_AXIS_K() if (SHOULD_ENABLE(k))  {  ENABLE_STEPPER_K(); AFTER_CHANGE(k, true); }
  #define DISABLE_AXIS_K() if (SHOULD_DISABLE(k)) { DISABLE_STEPPER_K(); AFTER_CHANGE(k, false); set_axis_untrusted(K_AXIS); }
#else
  #define  ENABLE_AXIS_K() NOOP
  #define DISABLE_AXIS_K() NOOP
#endif

//
// Extruder steppers enable / disable macros
//

#if ENABLED(MIXING_EXTRUDER)

  /**
   * Mixing steppers keep all their enable (and direction) states synchronized
   */
  #define _CALL_ENA_E(N)  ENABLE_STEPPER_E##N () ;
  #define _CALL_DIS_E(N) DISABLE_STEPPER_E##N () ;
  #define  ENABLE_AXIS_E0() { RREPEAT(MIXING_STEPPERS, _CALL_ENA_E) }
  #define DISABLE_AXIS_E0() { RREPEAT(MIXING_STEPPERS, _CALL_DIS_E) }

#elif ENABLED(E_DUAL_STEPPER_DRIVERS)

  #define  ENABLE_AXIS_E0() do{  ENABLE_STEPPER_E0();  ENABLE_STEPPER_E1(); }while(0)
  #define DISABLE_AXIS_E0() do{ DISABLE_STEPPER_E0(); DISABLE_STEPPER_E1(); }while(0)

#endif

#ifndef ENABLE_AXIS_E0
  #if E_STEPPERS && HAS_E0_ENABLE
    #define  ENABLE_AXIS_E0() ENABLE_STEPPER_E0()
  #else
    #define  ENABLE_AXIS_E0() NOOP
  #endif
#endif
#ifndef DISABLE_AXIS_E0
  #if E_STEPPERS && HAS_E0_ENABLE
    #define DISABLE_AXIS_E0() DISABLE_STEPPER_E0()
  #else
    #define DISABLE_AXIS_E0() NOOP
  #endif
#endif

#ifndef ENABLE_AXIS_E1
  #if E_STEPPERS > 1 && HAS_E1_ENABLE
    #define  ENABLE_AXIS_E1() ENABLE_STEPPER_E1()
  #else
    #define  ENABLE_AXIS_E1() NOOP
  #endif
#endif
#ifndef DISABLE_AXIS_E1
  #if E_STEPPERS > 1 && HAS_E1_ENABLE
    #define DISABLE_AXIS_E1() DISABLE_STEPPER_E1()
  #else
    #define DISABLE_AXIS_E1() NOOP
  #endif
#endif

#ifndef ENABLE_AXIS_E2
  #if E_STEPPERS > 2 && HAS_E2_ENABLE
    #define  ENABLE_AXIS_E2() ENABLE_STEPPER_E2()
  #else
    #define  ENABLE_AXIS_E2() NOOP
  #endif
#endif
#ifndef DISABLE_AXIS_E2
  #if E_STEPPERS > 2 && HAS_E2_ENABLE
    #define DISABLE_AXIS_E2() DISABLE_STEPPER_E2()
  #else
    #define DISABLE_AXIS_E2() NOOP
  #endif
#endif

#ifndef ENABLE_AXIS_E3
  #if E_STEPPERS > 3 && HAS_E3_ENABLE
    #define  ENABLE_AXIS_E3() ENABLE_STEPPER_E3()
  #else
    #define  ENABLE_AXIS_E3() NOOP
  #endif
#endif
#ifndef DISABLE_AXIS_E3
  #if E_STEPPERS > 3 && HAS_E3_ENABLE
    #define DISABLE_AXIS_E3() DISABLE_STEPPER_E3()
  #else
    #define DISABLE_AXIS_E3() NOOP
  #endif
#endif

#ifndef ENABLE_AXIS_E4
  #if E_STEPPERS > 4 && HAS_E4_ENABLE
    #define  ENABLE_AXIS_E4() ENABLE_STEPPER_E4()
  #else
    #define  ENABLE_AXIS_E4() NOOP
  #endif
#endif
#ifndef DISABLE_AXIS_E4
  #if E_STEPPERS > 4 && HAS_E4_ENABLE
    #define DISABLE_AXIS_E4() DISABLE_STEPPER_E4()
  #else
    #define DISABLE_AXIS_E4() NOOP
  #endif
#endif

#ifndef ENABLE_AXIS_E5
  #if E_STEPPERS > 5 && HAS_E5_ENABLE
    #define  ENABLE_AXIS_E5() ENABLE_STEPPER_E5()
  #else
    #define  ENABLE_AXIS_E5() NOOP
  #endif
#endif
#ifndef DISABLE_AXIS_E5
  #if E_STEPPERS > 5 && HAS_E5_ENABLE
    #define DISABLE_AXIS_E5() DISABLE_STEPPER_E5()
  #else
    #define DISABLE_AXIS_E5() NOOP
  #endif
#endif

#ifndef ENABLE_AXIS_E6
  #if E_STEPPERS > 6 && HAS_E6_ENABLE
    #define  ENABLE_AXIS_E6() ENABLE_STEPPER_E6()
  #else
    #define  ENABLE_AXIS_E6() NOOP
  #endif
#endif
#ifndef DISABLE_AXIS_E6
  #if E_STEPPERS > 6 && HAS_E6_ENABLE
    #define DISABLE_AXIS_E6() DISABLE_STEPPER_E6()
  #else
    #define DISABLE_AXIS_E6() NOOP
  #endif
#endif

#ifndef ENABLE_AXIS_E7
  #if E_STEPPERS > 7 && HAS_E7_ENABLE
    #define  ENABLE_AXIS_E7() ENABLE_STEPPER_E7()
  #else
    #define  ENABLE_AXIS_E7() NOOP
  #endif
#endif
#ifndef DISABLE_AXIS_E7
  #if E_STEPPERS > 7 && HAS_E7_ENABLE
    #define DISABLE_AXIS_E7() DISABLE_STEPPER_E7()
  #else
    #define DISABLE_AXIS_E7() NOOP
  #endif
#endif
