#ifndef INC_SHIFTREG_H_
#define INC_SHIFTREG_H_

#ifdef _SHIFTREG_C
   #define SCOPE
#else
   #define SCOPE extern
#endif

#include "main.h"

#define RCK_GPIO_PIN GPIO_PIN_2
#define RCK_GPIO_EXPANDER GPIOA

#define SRCLK_GPIO_PIN GPIO_PIN_3
#define SRCLK_GPIO_EXPANDER GPIOA

#define SIN_GPIO_PIN GPIO_PIN_8
#define SIN_GPIO_EXPANDER GPIOB

#define NG_GPIO_PIN GPIO_PIN_9
#define NG_GPIO_EXPANDER GPIOB

#undef SCOPE
#endif /* INC_SHIFTREG_H_ */
