# ADC Sensor Log Processor

https://github.com/monty03williams-dot/Resit_ADC.git

## Project description

This project is a C99 command-line program for processing a binary ADC sensor log file.

The program reads `adcsensorlog.bin`, validates the file header, loads the ADC records into memory, converts raw ADC values into voltages, calculates basic per-channel statistics, detects simple fault conditions, checks for sequence gaps, and writes the final summary to `results.txt`.

## Language and tools

- Language: C
- Standard: C99
- Build system: CMake
- Recommended IDE: CLion

## Input file

The program expects a binary input file named:

`adcsensorlog.bin`

During testing, I placed this file inside a data folder:
`data/adcsensorlog.bin`

## Binary file format
The file contains a 24-byte header followed by 4000 records.

The header contains:


- **Magic number** - Used to check that the file is the correct format
- **Version** - File format version
- **Channel count** - Number of ADC channels
- **Record count** - Number of records in the file
- **Sample rate** - Sampling rate in Hz
- **Reserved bytes** - Unused padding/reserved data

Each record contains:

- **Timestamp** - Time of the sample in seconds
- **Channel ID** - ADC channel number
- **Raw value** - 12-bit ADC reading
- **Temperature** - Temperature stored in tenths of degrees Celsius
- **Status flags** - Fault/status information
- **Sequence number** - Used to check record order
- **Reserved bytes** - Unused padding/reserved data

## Voltage conversion
The program calculates voltage from the raw ADC value using:

`voltage = (raw_value / 4095.0) * 3.3`

This is because the ADC is 12-bit, so the raw values range from 0 to 4095, and the reference voltage is 3.3 V.

## Program features

The program performs the following tasks:

- Reads and validates the binary file header
- Reads all ADC records from the file
- Stores records in memory
- Converts raw ADC readings to voltage
- Calculates basic statistics for each channel
- Counts sensor fault flags
- Counts overvoltage readings above 3.0 V
- Counts undervoltage readings below 0.3 V
- Checks for sequence number gaps
- Writes the results to results.txt

## Project files

- **main.c**	- Main program entry point
- **adc.h**	- ADC constants, structs, and function declarations
- **adc.c**	- ADC voltage conversion, fault counting, and sequence checking
- **io.h**	- File input/output function declarations
- **io.c**	- Binary file reading, header validation, and results file writing
- **stats.h**	- Statistics structs and function declarations
- **stats.c**	- Per-channel statistics calculations
- **CMakeLists.txt**	- CMake build configuration
- **README.md**	- Project explanation and instructions

## How to compile
### Using CLion
1. Open CLion.
2. Select **File > Open** and choose the project folder.
3. CLion will detect `CMakeLists.txt` and configure the project automatically.
4. Click **Build > Build Project** or press `Ctrl+F9`.
5. The executable will be created inside `cmake-build-debug`.

## How to run
Run the program with the binary ADC file path as the command-line argument.

Example:

`./adcprocessor data/adcsensorlog.bin
`

On Windows, the executable may be inside the CMake build folder, for example:

`build\adcprocessor.exe data\adcsensorlog.bin
`

In CLion, set the program argument to:

`data/adcsensorlog.bin
`

### Using gcc command line
Open a terminal in the project folder and run:

`gcc -std=c99 -o adcprocessor main.c adc.c io.c stats.c -lm
`
## Output
The program prints a summary to the terminal and also creates:

`results.txt
`

The results file contains:

- Header information
- Per-channel voltage and temperature statistics
- Fault summary
- Sequence gap summary

## Testing
I checked the program by comparing the first few records printed by the program with the human-readable preview file.

For example, the first record should show approximately:

`Record 0: time=0.0000 ch=0 raw=2046 voltage=1.648791 temp=24.9 flags=0x00 seq=0
`

This confirms that the binary file is being read correctly.

## Notes
The voltage values are not read directly from the binary file. They are calculated by the program from the raw ADC values.

The binary record layout is read using packed structs so that the C struct layout matches the exact byte layout of the file.


One thing: change `adcprocessor` in the run command if your executable/project is named something else in `CMakeLists.txt`.
