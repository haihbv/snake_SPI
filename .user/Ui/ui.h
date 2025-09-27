#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif 

typedef struct
{
	void (*ShowStartScreen)(void);
	void (*UpdateStartBlink)(void);
	
	void (*ShowOverScreen)(void);
	void (*UpdateOverBlink)(void);
	
	void (*ShowWinScreen)(void);
	void (*UpdateWinBlink)(void);
	
	void (*ClearToGameBg)(void);
} UI_Driver_t;

extern UI_Driver_t ui;
          
void UI_ShowStartScreen(void);      
void UI_UpdateStartBlink(void);

void UI_ShowOverScreen(void);
void UI_UpdateOverBlink(void);

void UI_ShowWinScreen(void);
void UI_UpdateWinBlink(void);

void UI_ClearToGameBg(void);       



#ifdef __cplusplus
}
#endif /* __cplusplus */

