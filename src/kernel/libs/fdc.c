#include "include/io.h"
#include "include/fdc.h"

#define DMA_CH2_ADDR     0x04
#define DMA_CH2_COUNT    0x05
#define DMA_CH2_PAGE     0x81
#define DMA_MASK_REG     0x0A
#define DMA_MODE_REG     0x0B
#define DMA_CLEAR_FF_REG 0x0C
 
#define DMA_MODE_READ  0x46 // disk -> memory (single mode, channel 2, write-to-memory)
#define DMA_MODE_WRITE 0x4A // memory -> disk (single mode, channel 2, read-from-memory)
 
// DMA-safe scratch buffer: must sit below 16MB and not cross a 64KB boundary.
// One track's worth (18 * 512 = 9216 bytes) aligned to 32KB guarantees that,
// since 9216 bytes never spans a 64KB boundary regardless of which half of the
// 64KB page the 32KB-aligned address lands in.
static uint8_t fdc_dma_buffer[FDC_SECTOR_SIZE * FDC_SECTORS_PER_TRACK] __attribute__((aligned(0x8000)));
 
static volatile bool fdc_irq_fired = false;
 
void fdc_irq_handler(void) {
    fdc_irq_fired = true;
}
 
extern uint64_t get_ticks(void);
static void fdc_delay_ms(uint32_t ms) {
    uint64_t target = get_ticks() + (ms / 10); // 100 ticks/sec = 10ms per tick
    while (get_ticks() < target) { }
}
 
static bool fdc_wait_irq(void) {
    uint32_t timeout = 10000000;
    while (!fdc_irq_fired && --timeout) { }
    if (!fdc_irq_fired) return false;
    fdc_irq_fired = false;
    return true;
}
 
static bool fdc_wait_rqm(bool for_read) {
    for (uint32_t i = 0; i < 1000000; i++) {
        uint8_t msr = i686_inb(FDC_MSR);
        if (!(msr & FDC_MSR_RQM)) continue;
        bool dio_set = msr & FDC_MSR_DIO;
        if (for_read == dio_set) return true; // direction matches what we want
    }
    return false;
}
 
static bool fdc_write_fifo(uint8_t byte) {
    if (!fdc_wait_rqm(false)) return false;
    i686_outb(FDC_FIFO, byte);
    return true;
}
 
static bool fdc_read_fifo(uint8_t *byte) {
    if (!fdc_wait_rqm(true)) return false;
    *byte = i686_inb(FDC_FIFO);
    return true;
}
 
static bool fdc_send_command(const uint8_t *cmd, int len) {
    for (int i = 0; i < len; i++) {
        if (!fdc_write_fifo(cmd[i])) return false;
    }
    return true;
}
 
static bool fdc_read_result(uint8_t *result, int len) {
    for (int i = 0; i < len; i++) {
        if (!fdc_read_fifo(&result[i])) return false;
    }
    return true;
}
 
static bool fdc_sense_interrupt(uint8_t *st0, uint8_t *cyl) {
    if (!fdc_write_fifo(FDC_CMD_SENSE_INTERRUPT)) return false;
    uint8_t result[2];
    if (!fdc_read_result(result, 2)) return false;
    if (st0) *st0 = result[0];
    if (cyl) *cyl = result[1];
    return true;
}
 
static void fdc_motor_on(void) {
    i686_outb(FDC_DOR, FDC_DOR_MOTOR0 | FDC_DOR_DMA_IO | FDC_DOR_RESET | FDC_DRIVE);
    fdc_delay_ms(300);
}
 
static void fdc_motor_off(void) {
    i686_outb(FDC_DOR, FDC_DOR_DMA_IO | FDC_DOR_RESET | FDC_DRIVE);
}
 
static void lba_to_chs(uint32_t lba, uint16_t *cyl, uint8_t *head, uint8_t *sector) {
    *cyl = (uint16_t)(lba / (FDC_HEADS * FDC_SECTORS_PER_TRACK));
    *head = (uint8_t)((lba / FDC_SECTORS_PER_TRACK) % FDC_HEADS);
    *sector = (uint8_t)((lba % FDC_SECTORS_PER_TRACK) + 1);
}
 
static bool fdc_recalibrate(void) {
    fdc_irq_fired = false;
    uint8_t cmd[2] = { FDC_CMD_RECALIBRATE, FDC_DRIVE };
    if (!fdc_send_command(cmd, 2)) return false;
    if (!fdc_wait_irq()) return false;
    uint8_t st0, cyl;
    if (!fdc_sense_interrupt(&st0, &cyl)) return false;
    if ((st0 & FDC_ST0_IC_MASK) != 0 || cyl != 0) return false;
    return true;
}
 
static bool fdc_seek(uint16_t cylinder, uint8_t head) {
    fdc_irq_fired = false;
    uint8_t cmd[3] = { FDC_CMD_SEEK, (uint8_t)((head << 2) | FDC_DRIVE), (uint8_t)cylinder };
    if (!fdc_send_command(cmd, 3)) return false;
    if (!fdc_wait_irq()) return false;
    uint8_t st0, cyl;
    if (!fdc_sense_interrupt(&st0, &cyl)) return false;
    if (!(st0 & FDC_ST0_SEEK_END)) return false;
    if (cyl != cylinder) return false;
    return true;
}
 
 static void fdc_setup_dma(bool for_read, uint32_t byte_count) {
     uint32_t addr = (uint32_t)(uintptr_t)fdc_dma_buffer;
     uint32_t count = byte_count - 1;   // DMA count register is "bytes - 1"

     i686_outb(DMA_MASK_REG, 0x06);
     i686_outb(DMA_CLEAR_FF_REG, 0xFF);

     i686_outb(DMA_CH2_ADDR, (uint8_t)(addr & 0xFF));
     i686_outb(DMA_CH2_ADDR, (uint8_t)((addr >> 8) & 0xFF));
     i686_outb(DMA_CH2_PAGE, (uint8_t)((addr >> 16) & 0xFF));

     i686_outb(DMA_CLEAR_FF_REG, 0xFF);
     i686_outb(DMA_CH2_COUNT, (uint8_t)(count & 0xFF));
     i686_outb(DMA_CH2_COUNT, (uint8_t)((count >> 8) & 0xFF));

     i686_outb(DMA_MODE_REG, for_read ? DMA_MODE_READ : DMA_MODE_WRITE);
     i686_outb(DMA_MASK_REG, 0x02);
 }
 
bool init_fdc(void) {
    // Reset the controller.
    i686_outb(FDC_DOR, 0x00);
    fdc_delay_ms(1);
    fdc_irq_fired = false;
    i686_outb(FDC_DOR, FDC_DOR_DMA_IO | FDC_DOR_RESET);
 
    if (!fdc_wait_irq()) return false;
 
    // Reset raises one IRQ per drive the BIOS thinks might exist; drain them.
    for (int i = 0; i < 4; i++) {
        uint8_t st0, cyl;
        fdc_sense_interrupt(&st0, &cyl);
    }
 
    // Transfer speed: 500kbps for a 1.44MB drive.
    i686_outb(FDC_CCR, 0x00);
 
    // SPECIFY: step rate/head unload time, head load time/no-DMA flag.
    // 0xDF / 0x02 are the standard values used for 1.44MB drives.
    uint8_t specify[3] = { FDC_CMD_SPECIFY, 0xDF, 0x02 };
    if (!fdc_send_command(specify, 3)) return false;
 
    fdc_motor_on();
    bool ok = fdc_recalibrate();
    fdc_motor_off();
    return ok;
}
 
 // Transfers up to a full track in one FDC command. Returns sectors actually
 // transferred (may be less than requested if it hit a track boundary), or 0 on failure.
 static uint32_t fdc_transfer_chunk(uint32_t lba, void *buffer, uint32_t sectors_wanted, bool write) {
     uint16_t cyl;
     uint8_t head, start_sector;
     lba_to_chs(lba, &cyl, &head, &start_sector);

     // Don't cross a track boundary in a single command.
     uint32_t sectors_left_in_track = FDC_SECTORS_PER_TRACK - (start_sector - 1);
     uint32_t chunk = sectors_wanted < sectors_left_in_track ? sectors_wanted : sectors_left_in_track;
     uint32_t byte_count = chunk * FDC_SECTOR_SIZE;

     if (!fdc_seek(cyl, head)) return 0;

     if (write) {
         for (uint32_t i = 0; i < byte_count; i++) {
             fdc_dma_buffer[i] = ((uint8_t *)buffer)[i];
         }
     }

     fdc_setup_dma(!write, byte_count);
     fdc_irq_fired = false;

     uint8_t end_sector = (uint8_t)(start_sector + chunk - 1); // EOT: last sector this command touches
     uint8_t cmd[9] = {
         write ? FDC_CMD_WRITE_DATA : FDC_CMD_READ_DATA,
         (uint8_t)((head << 2) | FDC_DRIVE),
         (uint8_t)cyl,
         head,
         start_sector,
         2,
         end_sector,
         FDC_GAP3_LENGTH,
         FDC_DTL
     };
     if (!fdc_send_command(cmd, 9)) return 0;
     if (!fdc_wait_irq()) return 0;

     uint8_t result[7];
     if (!fdc_read_result(result, 7)) return 0;
     if ((result[0] & FDC_ST0_IC_MASK) != 0) return 0;
     if (result[1] != 0 || result[2] != 0) return 0;

     if (!write) {
         for (uint32_t i = 0; i < byte_count; i++) {
             ((uint8_t *)buffer)[i] = fdc_dma_buffer[i];
         }
     }
     return chunk;
 }

 static bool fdc_transfer(uint32_t lba, void *buffer, uint32_t num_sectors, bool write) {
     fdc_motor_on();                     // once for the whole run, not per sector
     uint8_t *buf8 = (uint8_t *)buffer;
     uint32_t remaining = num_sectors;
     uint32_t cur_lba = lba;
     while (remaining > 0) {
         uint32_t done = fdc_transfer_chunk(cur_lba, buf8, remaining, write);
         if (done == 0) {
             fdc_motor_off();
             return false;
         }
         remaining -= done;
         cur_lba += done;
         buf8 += done * FDC_SECTOR_SIZE;
     }
     fdc_motor_off();
     return true;
 }

 bool fdc_read_sectors(uint32_t lba, uint8_t num_sectors, void *buffer) {
     return fdc_transfer(lba, buffer, num_sectors, false);
 }

 bool fdc_write_sectors(uint32_t lba, void *buffer, uint8_t num_sectors) {
     return fdc_transfer(lba, buffer, num_sectors, true);
 }
 
