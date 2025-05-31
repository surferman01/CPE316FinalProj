/*A**************************************************************************
* NAME:         nf_drv_load.a51
*----------------------------------------------------------------------------
* Copyright (c) 2004 Atmel.
*----------------------------------------------------------------------------
* RELEASE:      snd1c-demo-nf-mmc-4_6_0      
* REVISION:     1.3     
*----------------------------------------------------------------------------
* PURPOSE:
* This file contains the In System Programming execution routine
*****************************************************************************/

;_____ I N C L U D E S ____________________________________________________


;_____ M A C R O S ________________________________________________________

EXTRN XDATA (nf_data)

;_____ D E F I N I T I O N ________________________________________________

NF_MODULE   SEGMENT CODE

RSEG    NF_MODULE
;_____ D E C L A R A T I O N ______________________________________________

PUBLIC  nf_init_buffer
PUBLIC  nf_upload_buffer
PUBLIC  nf_download_buffer



;*F**************************************************************************
; NAME: nf_init_buffer
;----------------------------------------------------------------------------
; PARAMS:
;
; return:
;----------------------------------------------------------------------------
; PURPOSE: 
;   init buffer with 0xFF
;----------------------------------------------------------------------------
; EXAMPLE:
;----------------------------------------------------------------------------
; NOTE: 
;----------------------------------------------------------------------------
; REQUIREMENTS: 
;****************************************************************************
nf_init_buffer:
    MOV     R0,#0H
    
INIT_START:
    MOV     A,#0FFH

    MOVX    @R0,A       ;1
    INC     R0
    MOVX    @R0,A       ;2
    INC     R0
    MOVX    @R0,A       ;3
    INC     R0
    MOVX    @R0,A       ;4
    INC     R0
    MOVX    @R0,A       ;5
    INC     R0
    MOVX    @R0,A       ;6
    INC     R0
    MOVX    @R0,A       ;7
    INC     R0
    MOVX    @R0,A       ;8
    INC     R0
    MOVX    @R0,A       ;9
    INC     R0
    MOVX    @R0,A       ;10
    INC     R0
    MOVX    @R0,A       ;11
    INC     R0
    MOVX    @R0,A       ;12
    INC     R0
    MOVX    @R0,A       ;13
    INC     R0
    MOVX    @R0,A       ;14
    INC     R0
    MOVX    @R0,A       ;15
    INC     R0
    MOVX    @R0,A       ;16
    INC     R0
    MOVX    @R0,A       ;17
    INC     R0
    MOVX    @R0,A       ;18
    INC     R0
    MOVX    @R0,A       ;19
    INC     R0
    MOVX    @R0,A       ;20
    INC     R0
    MOVX    @R0,A       ;21
    INC     R0
    MOVX    @R0,A       ;22
    INC     R0
    MOVX    @R0,A       ;23
    INC     R0
    MOVX    @R0,A       ;24
    INC     R0
    MOVX    @R0,A       ;25
    INC     R0
    MOVX    @R0,A       ;26
    INC     R0
    MOVX    @R0,A       ;27
    INC     R0
    MOVX    @R0,A       ;28
    INC     R0
    MOVX    @R0,A       ;29
    INC     R0
    MOVX    @R0,A       ;30
    INC     R0
    MOVX    @R0,A       ;31
    INC     R0
    MOVX    @R0,A       ;32
    INC     R0

    MOV     A,R0
    JNZ     INIT_START      ; restart if R0 <> 0
    RET



;*F**************************************************************************
; NAME: nf_upload_buffer
;----------------------------------------------------------------------------
; PARAMS:
;
; return:
;----------------------------------------------------------------------------
; PURPOSE: 
;   upload_buffer NF->XRAM 
;----------------------------------------------------------------------------
; EXAMPLE:
;----------------------------------------------------------------------------
; NOTE: 
;----------------------------------------------------------------------------
; REQUIREMENTS: 
;****************************************************************************
nf_upload_buffer:
    MOV     R0,#0H
    MOV     DPTR, #nf_data
UP_START_TRANSFERT:
    MOVX    A,@DPTR     ;1
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;2
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;3
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;4
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;5
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;6
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;7
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;8
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;9
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;10
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;11
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;12
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;13
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;14
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;15
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;16
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;17
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;18
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;19
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;20
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;21
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;22
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;23
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;24
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;25
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;26
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;27
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;28
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;29
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;30
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;31
    MOVX    @R0,A
    INC     R0
    MOVX    A,@DPTR     ;32
    MOVX    @R0,A
    INC     R0
    MOV     A,R0
    JNZ     UP_START_TRANSFERT      ; restart if R0 <> 0
    RET




;*F**************************************************************************
; NAME: nf_download_buffer
;----------------------------------------------------------------------------
; PARAMS:
;
; return:
;----------------------------------------------------------------------------
; PURPOSE: 
;   download_buffer XRAM->NF 
;----------------------------------------------------------------------------
; EXAMPLE:
;----------------------------------------------------------------------------
; NOTE: 
;----------------------------------------------------------------------------
; REQUIREMENTS: 
;****************************************************************************
nf_download_buffer:
    MOV     R0,#0H
    MOV     DPTR, #nf_data

DOWN_START_TRANSFERT:
    MOVX    A,@R0      ;1
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;2
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;3
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;4
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;5
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;6
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;7
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;8
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;9
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;10
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;11
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;12
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;13
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;14
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;15
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;16
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;17
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;18
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;19
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;20
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;21
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;22
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;23
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;24
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;25
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;26
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;27
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;28
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;29
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;30
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;31
    MOVX    @DPTR,A
    INC     R0

    MOVX    A,@R0      ;32
    MOVX    @DPTR,A
    INC     R0

    MOV     A,R0
    JNZ     DOWN_START_TRANSFERT

    RET

END
