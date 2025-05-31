//! @file conf_scheduler.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! This file contains the possible external configuration of the scheduler
//! This file will be given to any external customer
//!
//! @version 1.1 c5131-nRF24L01-demo-receiver-1_0_0 $Id: conf_scheduler.h,v 1.1 2006/05/04 12:34:16 cgasnier Exp $
//!
//! @todo
//! @bug
#ifndef _CONF_SCHEDULER_H_
#define _CONF_SCHEDULER_H_


//! --- Scheduler Configuration --- 
// ***** Task init *****
#define Scheduler_task_1_init    usb_task_init           // Init the USB task
//#define Scheduler_task_2_init
//#define Scheduler_task_3_init
//#define Scheduler_task_4_init
//#define Scheduler_task_5_init


// ***** Task definition *****
#define Scheduler_task_1         usb_task                // Mass storage task
//#define Scheduler_task_2
//#define Scheduler_task_3
//#define Scheduler_task_4
//#define Scheduler_task_5
                                
// ***** Type of scheduler *****
// Possible value: SCH_TIMED, SCH_TASK, SCH_FREE
#define SCHEDULER_TYPE           SCHEDULER_FREE

#endif  //! _CONF_SCHEDULER_H_

