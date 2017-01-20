% Madnex installation procedure
% Thomas Helfer
% 16/01/2017

# Compilation

## `cmake`

### Standard usage (for end users)

~~~~{.bash}
cmake ../trunk/ -DCMAKE_BUILD_TYPE=Release -Denable-doxygen-doc=ON -DCMAKE_INSTALL_PREFIX=/home/th202608/codes/madnex/trunk/install
~~~~

### For `madnex` developpers

~~~~{.bash}
cmake ../trunk/ -DCMAKE_BUILD_TYPE=Release -Denable-doxygen-doc=ON -Denable-sanitize-options=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON 
~~~~

### Tools for developpers

#### `clang-tidy`

#### `coverage`

~~~~{.bash}
cmake ../trunk/ -DCMAKE_BUILD_TYPE=Coverage
~~~~
