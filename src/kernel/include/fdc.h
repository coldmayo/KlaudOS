#ifndef fdc_h_INCLUDED
#define fdc_h_INCLUDED
#include <stdint.h>
#include <stdbool.h>
 
// I/O ports (primary controller, base 0x3F0)
#define FDC_DOR   0x3F2   // Digital Output Register (write)
#define FDC_MSR   0x3F4   // Main Status Register (read)
#define FDC_FIFO  0x3F5   // Data FIFO (read/write)
#define FDC_CCR   0x3F7   // Configuration Control Register (write)
 
// Digital Output Register bits
#define FDC_DOR_DRIVE0   0x00
#define FDC_DOR_RESET    0x04   // 0 = reset asserted, 1 = normal operation
#define FDC_DOR_DMA_IO   0x08   // 1 = DMA/IRQ enabled
#define FDC_DOR_MOTOR0   0x10   // motor enable, drive 0
 
// Main Status Register bits
#define FDC_MSR_BUSY     0x10
#define FDC_MSR_NON_DMA  0x20
#define FDC_MSR_DIO      0x40  // 1 = FDC->CPU (read fifo), 0 = CPU->FDC (write fifo)
#define FDC_MSR_RQM      0x80  // FIFO ready for the next byte
 
// Commands
#define FDC_CMD_SPECIFY          0x03
#define FDC_CMD_WRITE_DATA       0xC5  // 0x45 | MT(0x80) | MFM(0x40) -> written as base|flags below
#define FDC_CMD_READ_DATA        0xE6  // 0x06 | MT(0x80) | MFM(0x40)
#define FDC_CMD_RECALIBRATE      0x07
#define FDC_CMD_SENSE_INTERRUPT  0x08
#define FDC_CMD_SEEK             0x0F
 
// ST0 status bits
#define FDC_ST0_IC_MASK   0xC0
#define FDC_ST0_SEEK_END  0x20
 
// Geometry: standard 3.5" 1.44MB floppy
#define FDC_SECTORS_PER_TRACK 18
#define FDC_HEADS             2
#define FDC_CYLINDERS         80
#define FDC_SECTOR_SIZE       512
#define FDC_DRIVE             0   // drive 0 (A:)
#define FDC_GAP3_LENGTH       0x1B
#define FDC_DTL               0xFF // unused when sector size code != 0
 
bool init_fdc(void);
bool fdc_read_sectors(uint32_t lba, uint8_t num_sectors, void *buffer);
bool fdc_write_sectors(uint32_t lba, void *buffer, uint8_t num_sectors);
 
// Call this from your IRQ6 handler (PIC IRQ6 must be unmasked).
void fdc_irq_handler(void);

#endif
