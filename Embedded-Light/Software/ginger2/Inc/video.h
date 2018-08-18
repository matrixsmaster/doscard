/*
 * video.h
 *
 *  Created on: Aug 19, 2018
 *      Author: MatrixS_Master
 */

#ifndef INC_VIDEO_H_
#define INC_VIDEO_H_

#define TFT_LCD_ADDR						     ((uint32_t)0x60000000)
#define TFT_LCD_WIDTH							 320
#define TFT_LCD_HEIGHT							 240
#define TFT_TOTAL_PIXELS						 (TFT_LCD_WIDTH*TFT_LCD_HEIGHT)
#define TFT_TOTAL_BYTES							 (TFT_TOTAL_PIXELS*2)

extern volatile uint16_t* g_frames;
extern volatile uint8_t g_frame_cnt;

void video_init();

#endif /* INC_VIDEO_H_ */
