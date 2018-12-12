# Design

## Types
### Typedefs:
* Included all of the typedefs for general data types (word, half-word, byte)
* Included a typedef for `unsigned int` --> `uint`
### Enumerations:
* `AskMode` enum: Tells which mode the CPU is in.
* `AskSignal` enum: What type of signal the CPU is receiving.
* `AskConfig` enum: The flag for the kernel, or what "special commands" the kernel should be doing.
### Structs:
* `AskHostServices` struct: Contains all of the function pointers that refer to the "host", or "shell." This shell is currently coded in `ash.py`
* `AskStats` struct: Contains all of the statistics for the processor, such as total amount of instructions run, Total loads/stores, and more.
### Classes:
* `CPU`: Contains all of the data for the simulated processor, which includes the 16 registers, statistics for the CPU, the flags for the CPU, and the shell that the CPU can interact with. It also contains methods which act on the CPU. This class is coded using the Singleton design pattern, which allows any other class to access a single instance of the class.
* `Instruction`: This will be the parent class of all of the different types of instructions. Since each instruction has some similarities, such as the condition, a superclass is a good idea. This class also contains the `decode` and `execute` methods. Decode takes the raw 32-bit instruction and parses it into a readable and executable instuction. Execute then takes the instruction and does whatever is needed with it.
* `IDataProcess`: This class is for all data processing commands in the processor. It currently supports mov and add. It will decode the command, execute it, and can also print it. 
* `IConditional`: This class is for all branching commands in the processor. It is currently not implemented.
* `ILoadStore`: This class is for all load/store commands in the processor. It supports decoding of load.
* `Operand2`: This class holds all of the logic for the Operand2 part of the data processing command.
* `String`: A string class I coded last semester, which includes many Python-like operations on strings. This is just for if I want to incorporate strings in any way in my program.
## Functions
* `kernel.hpp` contains all of the C-link style functions which allow the host/shell to communicate with the kernel itself. These functions include updating registers, asking for status updates, and more
* `decode` is inside all of the instruction classes, allowing the instruction to be decoded into readable form.
* `execute` is inside all of the instruction classes, allowing the CPU to run the instruction.
* `print` is inside all of the instruction classes, allowing the instruction to be printed in assembly form.
## Files
This program will be organized as follows:
* The home directory of the project contains the `CMakeLists.txt`, this design file, `ash.py`, and the rest of the folders for the project.
* The `test` folder contains any test `*.cpp` files to run with `cmake`. It also contains the file "tests.s" and "tests.txt", which are special tests used in the Refined Design of the project.
* The `build` folder contains all of the build data for `cmake`.
* The `journal` folder contains the webpage for my journal. To open the journal, just open the file "journal.html", and the browser should show it. The headings in the journal are collapsible for ease of readability.
* The `src` directory contains all of the `*.cpp` files for the project.
* The `lib` directory contains all of the `*.hpp` files for the project.
## Integration
The integration with the ArmSim Kernel API is quite simple. With the `CPU` class that I created, the API functions all use the CPU singleton instance to interact with the processor. I did edit some of the API, changing some names of structs to camelCase, and abstracting some code. In order to call into my processor, the API must just use `CPU::instance().[methodName]()` to call a function or access an instance variable. 