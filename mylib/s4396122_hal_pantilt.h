/**
 * *****************************************************************************
 * @file    s4396122_hal_pantilt.c
 * @author  Daniel Fitzmaurice = 43961229
 * @date    080317
 * @brief   Pan and Tilt control system
 *          REFERENCE: stage2
 * *****************************************************************************
 * s4396122_hal_pantilt_init() = initialise the servo
 * s4396122_hal_pantilt_pan_write(int) = sets the pan of the servo to the int
 * s4396122_hal_pantilt_pan_read() = returns the pan of the servo
 */

#ifndef HAL_PANTILT_H
#define HAL_PANTILT_H

#define s4396122_hal_pantilt_pan_write(int angle) pantilt_angle_write(0, angle)
#define s4396122_hal_pantilt_pan_read() pantilt_angle_read(0)

void s4396122_hal_pantilt_init();

#endif
