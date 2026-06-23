Split Keyboard Firmware Package

File names:
- main.cpp      : Split keyboard firmware for a single Pico per half
- oled_demo.cpp : OLED demo sketch
- platformio.ini : PlatformIO Arduino project file

Pin configuration according to your schematic:
- Row pins: GPIO16, 17, 18, 19, 20
- Col pins: GPIO11, 12, 13, 14, 15
- Rotary encoder pins: GPIO6, GPIO7
- OLED I2C: GPIO26 = SCL, GPIO27 = SDA
- OLED address: 0x3C

Tips:
- Flash the same firmware on both Picos.
- Define SIDE_LEFT = 1 for the left half and 0 for the right half.
- It is easy to modify the keymap arrays if you have another keymap in mind.
- You need to switch the scanning mechanism in case of reverse diodes.