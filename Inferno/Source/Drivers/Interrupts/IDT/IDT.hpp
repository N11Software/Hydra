#pragma once
#include "../../../../../Libraries/LibC/stdint.h"

#define IDT_TA_InterruptGate 0b10001110
#define IDT_TA_CallGate 0b10001100
#define IDT_TA_TrapGate 0b10001111

struct IDTDescriptorEntry {
  uint16_t offset0, selector;
  uint8_t ist, type_attr;
  uint16_t offset1;
  uint32_t offset2, ignore;
  void SetOffset(uint64_t offset);
  uint64_t GetOffset();
};

struct IDTR {
  uint16_t Limit;
  uint64_t Offset;
}__attribute__((packed));