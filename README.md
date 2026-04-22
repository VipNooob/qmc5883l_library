# QMC5883L Arduino Library

Arduino library for working with the **QMC5883L 3-axis magnetometer** over **I2C**.

## About the QMC5883L

The QMC5883L is a low-cost 3-axis magnetometer that is commonly used in place of the discontinued HMC5883L.

Many modules labeled as "HMC5883L" actually contain a QMC5883L chip, which uses a different I2C address and register map. Because of this, standard HMC5883L libraries will not work correctly.

This library is built specifically for the QMC5883L.

- Default I2C address: `0x0D` (not `0x1E` like HMC5883L)

---

## The library provides:
- sensor initialization
- full control over measurement parameters:
  - operating mode
  - data rate
  - measurement range
  - oversampling
- status monitoring (data ready, overflow, missed data)
- raw magnetic field readings
- converted magnetic field readings in µT

## Installation

### Option 1 — Manual installation
1. Download this repository as ZIP
2. In Arduino IDE open: **Sketch → Include Library → Add .ZIP Library**
3. Select the downloaded archive

### Option 2 — Git clone
Clone the repository into your Arduino libraries folder:

```bash
git clone https://github.com/VipNooob/qmc5883l_library.git
