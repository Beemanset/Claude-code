# CYD Handheld Radar - M314 Motion Tracker

A handheld motion tracker inspired by the iconic M314 Motion Tracker from the Aliens film franchise. Built using the ESP32 Cheap Yellow Display (CYD) and LD2450 mmWave radar sensor.

![Project Status](https://img.shields.io/badge/Status-In%20Development-yellow)
![License](https://img.shields.io/badge/License-MIT-blue)

## Features

- **Multi-Target Tracking**: Track up to 3 targets simultaneously
- **8m Detection Range**: LD2450 mmWave radar with 6-8m effective range
- **Visual Radar Display**: Animated sweeping arc display with distance markings
- **Proximity Audio**: Variable tone/beep rate based on target distance
- **Tactile Controls**: 5-way navigation switch for menu control
- **Battery Powered**: 18650 rechargeable lithium battery (3-5 hour runtime)
- **Real-time Coordinates**: X,Y position tracking for each detected target
- **Compact Design**: Handheld form factor (~120x80x40mm)

## Hardware Specifications

### Core Components

- **ESP32-2432S028R (CYD)**: 2.8" TFT touchscreen display, 240MHz dual-core
- **HLK-LD2450**: 24GHz mmWave FMCW radar sensor
- **18650 Battery**: 3000-3500mAh lithium-ion cell
- **Power System**: TP4056 charger + MT3608 boost converter (3.7V → 5V)

### Peripherals

- **Audio**: PAM8403 amplifier + 8Ω speaker
- **Controls**: 5-way tactile navigation switch breakout board
- **Monitoring**: LED battery level indicator

### Technical Specifications

| Specification | Value |
|--------------|-------|
| Detection Range | 6-8 meters |
| Detection Angle | ±60° azimuth, ±35° elevation |
| Max Targets | 3 simultaneous |
| Range Resolution | 0.75m |
| Range Accuracy | 0.15m |
| Refresh Rate | 10Hz |
| Display | 240x320 ILI9341 TFT |
| Power Consumption | 310mA typical, 710mA peak |
| Battery Life | 4-9 hours (depending on usage) |

## Project Status

**Current Phase**: Hardware design and documentation

- [x] Component selection and compatibility verification
- [x] GPIO pin mapping
- [x] Power system design
- [x] Wiring diagrams
- [ ] PCB design (optional)
- [ ] 3D printable enclosure design
- [ ] Software development
- [ ] Graphics library implementation
- [ ] Audio system implementation
- [ ] Assembly and testing

## Documentation

- [Parts List](PARTS_LIST.md) - Complete bill of materials with specifications
- [Wiring Guide](WIRING_GUIDE.md) - Detailed pin connections and wiring diagrams
- [Assembly Instructions](ASSEMBLY.md) - Step-by-step build guide

## Quick Start

### Prerequisites

- Arduino IDE or PlatformIO
- USB cable for ESP32 programming
- Soldering equipment
- Basic electronics tools

### Hardware Modifications Required

1. **Remove RGB LED from CYD** - Frees GPIO 4, 16, 17
2. **Solder pin headers to CN1** - Access GPIO 21, 22, 27, 35

### GPIO Pin Assignments

| GPIO | Function | Connected To |
|------|----------|--------------|
| 22 | UART RX | LD2450 TX |
| 27 | UART TX | LD2450 RX |
| 25 | DAC/Audio | PAM8403 amplifier |
| 4 | Button | 5-way UP |
| 16 | Button | 5-way DOWN |
| 17 | Button | 5-way LEFT |
| 21 | Button | 5-way RIGHT |
| 2 | Button | 5-way CENTER |
| 35 | ADC | Battery voltage (optional) |

## Software Dependencies

- **Arduino Libraries**:
  - TFT_eSPI or LVGL (graphics)
  - HLK-LD2450 library (ESPHome compatible)
  - ezButton (button debouncing)

- **ESP32 Board Support**: ESP32 Arduino Core

## Cost Estimate

**Total Project Cost**: $58-95 USD

- Budget build (generic parts): ~$58
- Quality build (name-brand parts): ~$95

See [PARTS_LIST.md](PARTS_LIST.md) for detailed breakdown.

## Inspiration

This project is inspired by the M314 Motion Tracker prop from James Cameron's *Aliens* (1986). The original prop featured:
- Sweeping radar arc display
- Audio feedback with variable pitch
- Handheld form factor
- Military-style ruggedized design

Our modern recreation uses mmWave radar technology to provide real motion tracking capabilities similar to what was imagined in the film.

## Safety Notes

- Use protected 18650 cells only (with built-in protection circuit)
- Do not exceed 5V input to ESP32 or LD2450
- Ensure proper polarity when connecting battery
- Add fuse protection to battery positive terminal (recommended)
- Do not operate while charging (unless using proper UPS module)

## Contributing

This is an open-source hardware project. Contributions welcome:
- PCB designs
- 3D printable enclosure models
- Software improvements
- Documentation updates
- Alternative component suggestions

## License

MIT License - See LICENSE file for details

## Credits

- **Concept**: Inspired by Aliens (1986) M314 Motion Tracker
- **Hardware**: ESP32 CYD community, Hi-Link Electronic (LD2450)
- **Documentation**: Created with Claude Code

## Contact & Support

For questions, issues, or suggestions:
- Open an issue on GitHub
- Check existing documentation
- Review wiring diagrams carefully before connecting power

## Acknowledgments

- Random Nerd Tutorials - ESP32 CYD documentation
- ESP32 community forums
- Adafruit and SparkFun learning resources
- Original Aliens prop designers and makers

---

**Disclaimer**: This is a hobbyist project for educational purposes. Not intended for commercial sale or professional security applications.
