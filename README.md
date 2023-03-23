# Summary

ProcMem is a simple C++ library designed for interaction with the memory space 
of a target process via the Windows API. There are templates and functions for actions
such as reading and writing to an external or internal (via dll injection) 
process. I got tired of defining the class in all of my projects, so I 
compiled it into this library to make it easier on myself.

# Usage

You can either drop the .h and .cpp files into your project and import PMemory.h, or use the compiled .lib.

### Example

```C++
// Creates an object and sets up the Process handle and PID. 
auto Mem = PMemory("process.exe");

// Print the PID.
std::cout << "Process ID: " << Mem.GetPID() << std::endl;

// Gets the memory address of a target DLL loaded into the target process.
auto TargetModule = Mem.GetModuleAddress("targetModule.dll");

// Reads the value at the address of TargetModule + the offset of 0xFFF.
uintptr_t Value Mem.ReadMem<uintptr_t>(TargetModule + 0xFFF);

// Writes a new value to a target address. Returns 0 if it fails.
Mem.WriteMem<uintptr_t>(TargetModule + 0xFFF, NewValue);
```

# Contributing

If there is something you feel it is missing, or you would like to make changes
then feel free to submit a pull request. I may add more to this library in the future.
