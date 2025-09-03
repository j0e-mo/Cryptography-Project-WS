# SHA-256 Embedded Application for TM4C123GH6PM

**Author:** Yousouf Soliman

## Overview

This project is a proof-of-concept embedded application that demonstrates SHA-256 cryptographic hashing on the Texas Instruments TM4C123GH6PM microcontroller. The application computes SHA-256 hashes of test data and stores the results in the internal EEPROM while providing status feedback via UART communication.

## Features

- SHA-256 cryptographic hashing using Mbed TLS library
- UART communication for status reporting
- EEPROM storage of hash results
- Custom hardware abstraction layer with developed drivers
- Continuous operation with memory management

## Hardware Requirements

### Development Board
- **TI Tiva C Series LaunchPad Evaluation Kit (EK-TM4C123GXL)**
  - Microcontroller: TM4C123GH6PM
  - ARM Cortex-M4F processor running at 80MHz
  - 256KB Flash memory
  - 32KB SRAM
  - 2KB EEPROM

### External Components
- No external components required beyond the LaunchPad board
- USB cable for programming and UART communication via debug port

## Software Requirements

### Development Environment
- **Code Composer Studio (CCS) 12.7.1**
  - Download from [TI's official website](https://www.ti.com/tool/CCSTUDIO)
  - Ensure TM4C123x device support is installed

### Libraries and Drivers

#### External Libraries
- **Mbed TLS 3.6.4**: Cryptographic library for SHA-256 implementation
- **TivaWare 2.2.0.295**: TI peripheral driver library (specifically for TM4C123GXL)
  - Used only for EEPROM driver functionality

#### Custom Developed Drivers
- **UART Driver**: Custom UART communication driver
- **PORT Driver**: GPIO port configuration and management driver

#### Service Layer
- **Common Macros File**: System-wide macro definitions
- **Compiler Header File**: Compiler-specific configurations
- **Platform Types File**: Platform-specific data type definitions
- **Standard Types File**: Standard data type definitions
- **Registers File**: Hardware register definitions and mappings

## Hardware Configuration

### UART Communication
- **UART0 Instance**: Uses pins PA0 (RX) and PA1 (TX)
- **Debug Port Connection**: UART0 can be accessed via the on-board debug circuitry through USB connection
- **Configuration**: 115200 baud rate, 8 data bits, no parity, 1 stop bit (8-N-1)

### EEPROM
- **Internal EEPROM**: 2KB internal EEPROM for hash result storage
- **Address Mapping**: 
  - Input data location: `0xAABBCCCDD`
  - Output data location: `0xDDCCBBAA`

## Project Structure

```
Project_Root/
├── main.c                          # Main application file
├── mbedtls_includes/               # Mbed TLS library headers
│   └── mbedtls/
│       └── sha256.h
├── Service_Layer/                  # Hardware abstraction layer
│   ├── Std_Types.h                # Standard type definitions
│   ├── Common_Macros.h            # System macros
│   ├── Compiler.h                 # Compiler configurations
│   ├── Platform_Types.h           # Platform-specific types
│   └── Registers.h                # Hardware register definitions
├── UART/                          # Custom UART driver
│   └── uart.h
├── PORT/                          # Custom PORT driver
│   └── Port.h
└── driverlib/                     # TivaWare EEPROM driver
    └── eeprom.h
```

## Setup Instructions

### 1. Install Development Environment

1. Download and install Code Composer Studio 12.7.1
2. During installation, ensure TM4C123x device support is selected
3. Launch CCS and set up a workspace

### 2. Install Required Libraries

#### Mbed TLS 3.6.4
1. Download Mbed TLS 3.6.4 from [Mbed TLS official repository](https://github.com/Mbed-TLS/mbedtls)
2. Extract the library to your project directory
3. Include the necessary header files in your project path

#### TivaWare 2.2.0.295
1. Download TivaWare from [TI's official website](https://www.ti.com/tool/SW-TM4C)
2. Install TivaWare to your system (typically `C:\ti\TivaWare_C_Series-2.2.0.295`)
3. Add the TivaWare path to your project's include directories

### 3. Project Configuration

#### Create New CCS Project
1. Open Code Composer Studio
2. File → New → CCS Project
3. Select:
   - Target: TM4C123GH6PM
   - Connection: Stellaris In-Circuit Debug Interface
   - Project templates: Empty Project (with main.c)

#### Configure Build Settings
1. Right-click project → Properties
2. Navigate to Build → ARM Compiler → Include Options
3. Add include paths for:
   - Mbed TLS headers directory
   - TivaWare include directory
   - Custom driver directories (UART, PORT, Service_Layer)

4. Navigate to Build → ARM Linker → File Search Path
5. Add library paths for:
   - Mbed TLS library files
   - TivaWare driver library

### 4. Hardware Connection

1. Connect the TM4C123GH6PM LaunchPad to your PC via USB
2. The debug USB connection provides both programming interface and UART communication
3. No additional wiring required

## Building the Project

### 1. Import Source Files
1. Copy all source files into your CCS project directory
2. Refresh the project in CCS (F5 or right-click → Refresh)

### 2. Build Process
1. Right-click project → Build Project
2. Ensure no compilation errors occur
3. Verify all dependencies are properly linked

### 3. Troubleshooting Build Issues
- **Include path errors**: Verify all include directories are correctly configured
- **Library linking errors**: Check that Mbed TLS and TivaWare libraries are properly linked
- **Missing headers**: Ensure all custom driver files are present in the project

## Running the Application

### 1. Program the Microcontroller
1. Right-click project → Debug As → Code Composer Debug Session
2. The program will be automatically loaded to the microcontroller
3. Click Resume (F8) to start execution

### 2. UART Communication Setup
1. Open a serial terminal application (PuTTY, TeraTerm, CCS Terminal, etc.)
2. Configure terminal settings:
   - Baud Rate: 115200
   - Data Bits: 8
   - Parity: None
   - Stop Bits: 1
   - Flow Control: None
3. Connect to the appropriate COM port (LaunchPad Virtual COM Port)

### 3. Expected Output
The application will continuously output status messages via UART:

```
SHA-256 function was Successful

EEPROM Resources Exhausted.
```

## Application Behavior

### Main Functionality
1. **Initialization**: Hardware peripherals (GPIO, UART, EEPROM) are configured
2. **Hash Computation**: The test string "test string for sha 256" is processed using SHA-256
3. **Status Reporting**: Success or failure status is transmitted via UART
4. **EEPROM Operations**: 
   - Hash results are written to EEPROM
   - Data is read back from EEPROM for verification
   - Process continues until EEPROM memory is exhausted
5. **Continuous Operation**: Application runs in an infinite loop

### Memory Management
- The application systematically utilizes the 2KB internal EEPROM
- Memory exhaustion message is displayed when all EEPROM space is used
- Each iteration processes 4-byte data blocks

## Configuration Options

### SHA Algorithm Selection
The application supports both SHA-256 and SHA-224 algorithms:
```c
#define SHA256 0U    // Use SHA-256 (default)
#define SHA224 1U    // Use SHA-224
```

### EEPROM Address Configuration
```c
#define INPUT_DATA_ADDRESS_LOCATION 0xAABBCCCDD
#define OUTPUT_DATA_ADDRESS_LOCATION 0xDDCCBBAA
#define REQUIRED_DATA_SIZE 4U
```

## API Reference

### Key Functions

#### `mbedtls_sha256()`
- **Purpose**: Computes SHA-256 hash of input data
- **Parameters**: Input data, data size, output buffer, algorithm flag
- **Returns**: 0 on success, negative value on failure

#### `EEPROMProgram()`
- **Purpose**: Writes data to internal EEPROM
- **Parameters**: Source address, destination address, data size

#### `EEPROMRead()`
- **Purpose**: Reads data from internal EEPROM
- **Parameters**: Destination address, source address, data size

#### `UART0_SendString()`
- **Purpose**: Transmits string data via UART0
- **Parameters**: Pointer to null-terminated string

## Debugging and Troubleshooting

### Common Issues

1. **UART Communication Problems**
   - Verify COM port selection in terminal application
   - Check baud rate settings (115200)
   - Ensure LaunchPad drivers are properly installed

2. **Build Errors**
   - Verify include paths for all libraries and custom drivers
   - Check that all source files are present
   - Ensure library versions match specifications

3. **Runtime Issues**
   - Use CCS debugger to step through code execution
   - Check UART output for error messages
   - Verify EEPROM initialization success

### Debug Features
- The application provides real-time status feedback via UART
- CCS debugging tools can be used for detailed code analysis
- Hardware breakpoints available for step-by-step execution

## Performance Characteristics

- **Hash Computation Speed**: Depends on Mbed TLS optimization and system clock (80MHz)
- **UART Communication**: 115200 baud rate provides adequate speed for status reporting
- **Memory Usage**: Efficient use of 2KB EEPROM with systematic addressing
- **Power Consumption**: Standard operation for TM4C123GH6PM at 80MHz

## Future Enhancements

- Support for different hash algorithms (SHA-1, SHA-512)
- External memory interface for larger data storage
- Network connectivity for remote hash verification
- Real-time clock integration for timestamping
- Enhanced error handling and recovery mechanisms

## License and Legal Information

This project is a proof-of-concept implementation. Users should verify compliance with relevant regulations when using cryptographic functions in production applications.

## Support and Contribution

For questions, issues, or contributions related to this project, please contact the author or refer to the official documentation of the respective libraries used.

---

**Note**: This project demonstrates cryptographic hashing capabilities on embedded systems and serves as a foundation for more complex security applications on ARM Cortex-M4 microcontrollers.
