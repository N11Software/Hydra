#pragma once

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

#define Interrupt __attribute__((interrupt))

struct InterruptFrame;
Interrupt void PageFaultHandler(InterruptFrame* frame);
Interrupt void DoubleFaultHandler(InterruptFrame* frame);
Interrupt void GeneralProtectionFaultHandler(InterruptFrame* frame);
Interrupt void PS2KeyboardHandler(InterruptFrame* frame);
Interrupt void PS2MouseHandler(InterruptFrame* frame);
Interrupt void PITHandler(InterruptFrame* frame);

void MapPIC();
void PICEndMaster();
void PICEndSlave();