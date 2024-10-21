# C++ Template

CMake starting point for my C++ projects. Configured for VSCode debugging

## After cloning the template:

1. Find and replace `CppTemplate` with your project name
2. Find and replace `cpp_template_compiler_flags` with `<YOUR_PROJECT_NAME>_compiler_flags`

## How to run

1. Create a `build` directory, e.g. `mkdir build`
2. Generate a native build system, `cd build && cmake ..`
3. Compile/link the project, `cmake --build .`
4. Run the project `./bin/CppTemplate` (assuming you're on mac/linux. You'll probably just want to open and run the project in VS if you're on windows)

There are a couple of handy scripts for mac/linux (I haven't created these for windows yet). Make sure to cd into the scripts directory and run them from there:

- `scripts/build.sh` - cds into the build directory, generates a native build system, compiles/links the project, and runs the executable
- `scripts/clean_build.sh` - removes the `build` directory and runs `build.sh`

### Notes

- When adding a new .cpp file make sure to run `cmake ..` e.g. `cmake .. && cmake --build . && ./bin/CppTemplate` otherwise the change to the sources won't get picked up by CMake

- To get debugging working you may have to follow these steps:

  1. Open the Command Palette and search for `CMake: Configure`. This will configure your project for building.
  2. Once configured, build the project by selecting `CMake: Build` or running the build task.
