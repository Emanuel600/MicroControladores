; ------------------------------------------------------------------------------
; Project:	LED Blink
; File:		ledBlink.asm
; Author:	Leandro Schwarz
; Created:	2017-08-07
; Modified:	2017-08-07
; Version:	1.0
; Notes:	Blinks an LED. Fcpu = 16 MHz.
; ------------------------------------------------------------------------------

; ------------------------------------------------------------------------------
; Include definition files
; ------------------------------------------------------------------------------
.include "include/m328Pdef.inc"

; ------------------------------------------------------------------------------
; Register definitions
; ------------------------------------------------------------------------------
.def	pushed			= R16
.def	pusher			= R17
.def    temp0           = R18
.def    temp1           = R19

.equ    LED_DDR         = DDRD
.equ    LED_PORT        = PORTD

; ------------------------------------------------------------------------------
; Functions
; ------------------------------------------------------------------------------

; ------------------------------------------------------------------------------
; Interruption handlers
; ------------------------------------------------------------------------------
.org 0x0000
	JMP resetHandler
.org INT0addr
	JMP int0Handler
.org INT1addr
	JMP int1Handler
.org PCI0addr
	JMP pcint0Handler
.org PCI1addr
	JMP pcint1Handler
.org PCI2addr
	JMP pcint2Handler
.org WDTaddr
	JMP wdtHandler
.org OC2Aaddr
	JMP timer2CompAHandler
.org OC2Baddr
	JMP timer2CompBHandler
.org OVF2addr
	JMP timer2OvfHandler
.org ICP1addr
	JMP timer1CaptureHandler
.org OC1Aaddr
	JMP timer1CompAHandler
.org OC1Baddr
	JMP timer1CompBHandler
.org OVF1addr
	JMP timer1OvfHandler
.org OC0Aaddr
	JMP timer0CompAHandler
.org OC0Baddr
	JMP timer0CompBHandler
.org OVF0addr
	JMP timer0OvfHandler
.org SPIaddr
	JMP spiHandler
.org URXCaddr
	JMP usartRxHandler
.org UDREaddr
	JMP usartDataEmptyHandler
.org UTXCaddr
	JMP usartTxHandler
.org ADCCaddr
	JMP adcHandler
.org ERDYaddr
	JMP eepromHandler
.org ACIaddr
	JMP analogCompHandler
.org TWIaddr
	JMP twiHandler
.org SPMRaddr
	JMP flashHandler
.org INT_VECTORS_SIZE

; ------------------------------------------------------------------------------
; Include other assembly files
; ------------------------------------------------------------------------------

; ------------------------------------------------------------------------------
; Constants stored in Flash memory
; Note: Variables must be multiples of 2, since memory is organized in 16 bits
; ------------------------------------------------------------------------------

; ------------------------------------------------------------------------------
; Main code
; ------------------------------------------------------------------------------
main:
    ; Set LED_PORT as output
    LDI     temp0, 0xFF
    LDI     temp1, 0x00
    OUT     LED_PORT, temp1
    OUT     LED_DDR, temp0
    main_loop:
        ; Push initial values to LED_PORT
        LDI     pushed, 0x02
        push0:
        LDI     pusher, 0x01
        MOV     temp1,  pushed
        OR      temp1,  pusher  ; temp1 = (pushed | pusher)
        OUT     LED_PORT, temp1
        RCALL   delay_250m
        ; Enter main animation loop
        ani_loop:
            ; if (pushed=pusher): _BV(pushed); pusher=0
            CP      pushed, pusher
            BREQ    ani_if
            TST     pusher;     Test pusher
            BREQ    push0;      (pusher==0) ? LSL(pusher) : pusher=1
            BRMI    main_loop;  ((pusher & _BV(7)) == 1): goto main_loop
            LSL     pusher
            RJMP     ani_end
            ; else: _BV(pusher)
            ani_if:
            LSL     pushed
            ANDI    pusher, 0x00
            ani_end:
            MOV     temp1,  pushed
            OR      temp1,  pusher;     temp1 = (pushed | pusher)
            OUT     LED_PORT, temp1
            RCALL   delay_250m
            RJMP     ani_loop
    RJMP     main_loop;                  Should never be called

delay_250m:
    LDI  R18, 41
    LDI  R19, 150
    LDI  R20, 125
d250m_L1:
    DEC  R20
    BRNE d250m_L1
    DEC  R19
    BRNE d250m_L1
    DEC  R18
    BRNE d250m_L1
    RJMP PC+1

    RET

delay_500m:
    LDI  R18, 41
    LDI  R19, 150
    LDI  R20, 125
d500m_L1:
    DEC  R20
    BRNE d500m_L1
    DEC  R19
    BRNE d500m_L1
    DEC  R18
    BRNE d500m_L1
    RJMP PC+1

    RET
; ------------------------------------------------------------------------------
; Interrupt handlers
; ------------------------------------------------------------------------------
int0Handler:
int1Handler:
pcint0Handler:
pcint1Handler:
pcint2Handler:
wdtHandler:
timer2CompAHandler:
timer2CompBHandler:
timer2OvfHandler:
timer1CaptureHandler:
timer1CompAHandler:
timer1CompBHandler:
timer1OvfHandler:
timer0CompAHandler:
timer0CompBHandler:
timer0OvfHandler:
spiHandler:
usartRxHandler:
usartDataEmptyHandler:
usartTxHandler:
adcHandler:
eepromHandler:
analogCompHandler:
twiHandler:
flashHandler:
resetHandler:
	LDI		R19, LOW(RAMEND)
	OUT		SPL, R19
	LDI		R19, HIGH(RAMEND)
	OUT		SPH,R19
	JMP		main
