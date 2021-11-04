# MCU name
MCU = SN32F248B

# BOOTLOADER = flash
SN32_BOOTLOADER_ADDRESS = 0x1FFF0301

OPT_DEFS = -O2

# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = yes      # Enable Bootmagic Lite
MOUSEKEY_ENABLE = no        # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = no         # Console for debug
COMMAND_ENABLE = no         # Commands for debug and configuration
# Do not enable SLEEP_LED_ENABLE. it uses the same timer as BACKLIGHT_ENABLE
SLEEP_LED_ENABLE = no       # Breathing sleep LED during USB suspend
# if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
NKRO_ENABLE = yes           # USB Nkey Rollover
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
RGBLIGHT_ENABLE = no        # Enable keyboard RGB underglow
AUDIO_ENABLE = no           # Audio output
LTO_ENABLE = no
KEYBOARD_SHARED_EP = yes

# Custom matrix handling
CUSTOM_MATRIX = yes
SRC = ../../../drivers/sn32/matrix_sn32f248b.c

# Custom RGB matrix handling
RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = SN32F24xx
SRC += config_led.c
