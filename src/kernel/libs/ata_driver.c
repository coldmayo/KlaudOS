#include "include/io.h"
#include "include/ata_driver.h"

static void ata_400ns_delay(void) {
    for (int i = 0; i < 4; i++) {
        i686_inb(ATA_PRIMARY_CTRL);
    }
}

static bool ata_wait_bsy_clear(void) {
    for (uint32_t i = 0; i < 1000000; i++) {
        if (!(i686_inb(ATA_PRIMARY_IO + ATA_REG_STATUS) & ATA_SR_BSY)) return true;
    }
    return false;
}

static bool ata_wait_drq(void) {
    for (uint32_t i = 0; i < 1000000; i++) {
        uint8_t status = i686_inb(ATA_PRIMARY_IO + ATA_REG_STATUS);
        if (status & ATA_SR_ERR) return false;
        if (status & ATA_SR_DRQ) return true;
    }
    return false;
}

bool init_ata(void) {
    i686_outb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, ATA_MASTER);
    ata_400ns_delay();

    i686_outb(ATA_PRIMARY_IO + ATA_REG_SECCOUNT0, 0);
    i686_outb(ATA_PRIMARY_IO + ATA_REG_LBA0, 0);
    i686_outb(ATA_PRIMARY_IO + ATA_REG_LBA1, 0);
    i686_outb(ATA_PRIMARY_IO + ATA_REG_LBA2, 0);
    i686_outb(ATA_PRIMARY_IO + ATA_REG_COMMAND, ATA_CMD_IDENTIFY);

    if (i686_inb(ATA_PRIMARY_IO + ATA_REG_LBA1) != 0) {
        return false;
    }
    if (!ata_wait_bsy_clear()) {
        return false;
    }

    // ATAPI devices leave LBA1/LBA2 nonzero here; bail if this isn't a plain ATA disk
    if (i686_inb(ATA_PRIMARY_IO + ATA_REG_LBA1) != 0 || i686_inb(ATA_PRIMARY_IO + ATA_REG_LBA2) != 0) {
        return false;
    }
    if (!ata_wait_drq()) return false;

    uint16_t identify_data[256];
    i686_insw(ATA_PRIMARY_IO + ATA_REG_DATA, identify_data, 256); // must drain the buffer even if unused

    return true;
}

bool ata_read_sectors(uint32_t lba, uint8_t num_sectors, void *buffer) {
    if (!ata_wait_bsy_clear()) return false;

    i686_outb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, ATA_MASTER | ((lba >> 24) & 0x0F));
    i686_outb(ATA_PRIMARY_IO + ATA_REG_SECCOUNT0, num_sectors);
    i686_outb(ATA_PRIMARY_IO + ATA_REG_LBA0, (uint8_t)lba);
    i686_outb(ATA_PRIMARY_IO + ATA_REG_LBA1, (uint8_t)(lba >> 8));
    i686_outb(ATA_PRIMARY_IO + ATA_REG_LBA2, (uint8_t)(lba >> 16));
    i686_outb(ATA_PRIMARY_IO + ATA_REG_COMMAND, ATA_CMD_READ_PIO);

    uint16_t *buf16 = (uint16_t *)buffer;
    for (uint8_t s = 0; s < num_sectors; s++) {
        if (!ata_wait_bsy_clear() || !ata_wait_drq()) return false;
        i686_insw(ATA_PRIMARY_IO + ATA_REG_DATA, buf16, 256); // 256 words = 512 bytes
        buf16 += 256;
    }
    return true;
}

bool ata_write_sectors(uint32_t lba, void * buffer, uint8_t num_sectors) {
    if (!ata_wait_bsy_clear()) return false;

    i686_outb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, ATA_MASTER | ((lba >> 24) & 0x0F));
    i686_outb(ATA_PRIMARY_IO + ATA_REG_SECCOUNT0, num_sectors);
    i686_outb(ATA_PRIMARY_IO + ATA_REG_LBA0, (uint8_t)lba);
    i686_outb(ATA_PRIMARY_IO + ATA_REG_LBA1, (uint8_t)(lba >> 8));
    i686_outb(ATA_PRIMARY_IO + ATA_REG_LBA2, (uint8_t)(lba >> 16));
    i686_outb(ATA_PRIMARY_IO + ATA_REG_COMMAND, ATA_CMD_WRITE_PIO);

    const uint16_t *buf16 = (const uint16_t *)buffer;
    for (uint8_t s = 0; s < num_sectors; s++) {
        if (!ata_wait_bsy_clear() || !ata_wait_drq()) return false;
        i686_outsw(ATA_PRIMARY_IO + ATA_REG_DATA, buf16, 256);
        buf16 += 256;
    }

    i686_outb(ATA_PRIMARY_IO + ATA_REG_COMMAND, ATA_CMD_CACHE_FLUSH); // commit writes, don't just buffer
    ata_wait_bsy_clear();
    return true;
}
