
#ifndef BL_FLASH_CONFIG_H
#define BL_FLASH_CONFIG_H

/* ============================================================================
 * Flash Memory Configuration
 * STM32G030F6P6
 * ========================================================================== */

#define BL_FLASH_START_ADDRESS          (0x08000000UL)
#define BL_FLASH_SIZE                   (32UL * 1024UL)
#define BL_FLASH_PAGE_SIZE              (2UL * 1024UL)

#define BL_FLASH_END_ADDRESS            \
    (BL_FLASH_START_ADDRESS + BL_FLASH_SIZE - 1UL)


/* ============================================================================
 * Bootloader Memory: 8 KB
 * ========================================================================== */

#define BL_BOOTLOADER_START_ADDRESS     (0x08000000UL)
#define BL_BOOTLOADER_SIZE              (8UL * 1024UL)

#define BL_BOOTLOADER_END_ADDRESS       \
    (BL_BOOTLOADER_START_ADDRESS + BL_BOOTLOADER_SIZE - 1UL)


/* ============================================================================
 * Main Application Memory: 22 KB
 * ========================================================================== */

#define BL_APP_START_ADDRESS            (0x08002000UL)
#define BL_APP_SIZE                     (22UL * 1024UL)

#define BL_APP_END_ADDRESS              \
    (BL_APP_START_ADDRESS + BL_APP_SIZE - 1UL)


/* ============================================================================
 * Bootloader Metadata: 2 KB
 * ========================================================================== */

#define BL_METADATA_START_ADDRESS       (0x08007800UL)
#define BL_METADATA_SIZE                (2UL * 1024UL)

#define BL_METADATA_END_ADDRESS         \
    (BL_METADATA_START_ADDRESS + BL_METADATA_SIZE - 1UL)

#define BL_FIRMWARE_METADATA_MAGIC      (0x424C4D44UL)

#define BL_UPDATE_STATUS_INVALID        (0x00000000UL)
#define BL_UPDATE_STATUS_VALID          (0xA5A5A5A5UL)


/* ============================================================================
 * FLASH CRC
 * ========================================================================== */

#define BL_FLASH_CRC16_POLYNOMIAL       (0x1021U)
#define BL_FLASH_CRC16_INITIAL          (0xFFFFU)

#endif /* BL_FLASH_CONFIG_H */
