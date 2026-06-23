# I2C Based Smart Switch Module Using CH32V003

## Overview

This project implements an I2C-based Smart Switch Module using the WCH CH32V003 RISC-V microcontroller.

The module acts as an intelligent I2C slave device that can be controlled by a master controller such as ESP32, STM32, Raspberry Pi, or other industrial controllers. It provides relay switching functionality through I2C commands and is designed for scalable home automation and industrial automation systems.

The project demonstrates low-cost distributed control using the CH32V003 MCU and I2C communication.

---

## Features

* I2C Slave Communication
* Relay Control
* Addressable I2C Device
* Multiple Module Support
* Low-Cost Hardware Design
* Home Automation Ready
* Industrial Automation Ready
* RISC-V Microcontroller Based
* Embedded C Firmware
* Modular Architecture

---

## Hardware Used

| Component     | Purpose           |
| ------------- | ----------------- |
| CH32V003      | Main Controller   |
| Relay Module  | Load Switching    |
| I2C Interface | Communication     |
| Power Supply  | System Power      |
| LEDs          | Status Indication |

---

## Development Environment

| Tool             | Purpose                 |
| ---------------- | ----------------------- |
| MounRiver Studio | Firmware Development    |
| Embedded C       | Application Development |
| WCH Toolchain    | Compilation             |
| CH32V003         | Target MCU              |

---

## Target Platform

```text
Microcontroller : CH32V003
Architecture    : RISC-V
IDE             : MounRiver Studio
Language        : Embedded C
```

---

## Project Structure

```text
.
├── Core/
├── User/
├── Startup/
├── Peripheral/
├── CH32V003FUN/
├── main.c
├── i2c_slave.c
├── relay_control.c
├── system_ch32v00x.c
├── README.md
└── LICENSE
```

---

## System Architecture

```text
              +-------------------+
              | I2C Master        |
              | ESP32 / STM32     |
              +-------------------+
                       |
                       |
                     I2C
                       |
                       v

              +-------------------+
              | CH32V003          |
              | I2C Smart Switch  |
              +-------------------+
                       |
                       |
                       v

              +-------------------+
              | Relay Output      |
              +-------------------+
                       |
                       v

                   Load
```

---

## Working Principle

### I2C Communication

The module acts as an I2C slave device.

Example:

```text
Slave Address : 0x02
```

Master sends command:

```text
0x01 → Relay ON
0x00 → Relay OFF
```

The CH32V003 receives the command and updates relay state accordingly.

---

## Relay Control Logic

```text
Receive I2C Command
        |
        v
Decode Command
        |
        v
Update Relay State
        |
        v
Control Connected Load
```

---

## Functional Modules

### I2C Module

#### I2C_Init()

* Initializes I2C peripheral.
* Configures slave address.

#### I2C_EventHandler()

* Handles received commands.

#### I2C_Read()

* Reads incoming data from master.

#### I2C_Write()

* Sends status information.

---

### Relay Module

#### Relay_Init()

* Configures relay GPIO.

#### Relay_ON()

* Activates relay output.

#### Relay_OFF()

* Deactivates relay output.

#### Relay_Toggle()

* Toggles current relay state.

---

### System Module

#### System_Init()

* Initializes peripherals.

#### GPIO_Init()

* Configures I/O pins.

---

## Communication Example

### Master Command

```text
Address : 0x02
Data    : 0x01
```

### Module Response

```text
Relay ON
```

---

## Build Instructions

### Development IDE

```text
MounRiver Studio
```

### Target MCU

```text
CH32V003
```

### Build Project

```text
Project → Build
```

### Flash Firmware

```text
WCH-LinkE
```

---

## Screenshots

Add screenshots of:

* MounRiver Studio Project
* CH32V003 Hardware
* I2C Communication Test
* Relay Switching
* Logic Analyzer Capture

---

## Demo Video

```markdown
## 🎥 Demo Video

[▶ Watch Demo](./i2c_smart_switch_demo.mp4)
```

---

## Concepts Demonstrated

* RISC-V Microcontroller Programming
* CH32V003 Development
* I2C Slave Implementation
* Embedded C
* Relay Control
* GPIO Management
* Industrial Communication
* Firmware Development

---

## Applications

* Home Automation
* Industrial Automation
* Smart Switch Boards
* Distributed Control Systems
* Building Automation
* IoT Gateways

---

## Future Improvements

* Multi-Relay Support
* Touch Switch Integration
* CAN Bus Version
* RS485 Version
* OTA Firmware Updates
* Wireless Gateway Integration

---

## Learning Outcomes

Through this project, I gained practical experience in:

* CH32V003 Development
* RISC-V Architecture
* MounRiver Studio
* I2C Protocol
* Embedded Firmware Development
* Relay Control Systems
* Industrial Communication Protocols

---

## Author

**Darsh Patel**

Electronics & Communication Engineer

Firmware Engineer | Embedded Systems | IoT Developer

GitHub: https://github.com/darshpatel001

---

## License

This project is licensed under the MIT License.

