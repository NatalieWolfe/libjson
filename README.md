libjson
=======

A fork of http://sourceforge.net/projects/libjson/ aiming to clean up the library and bring C++11
features to it. The C compatibility is not maintained.


Goals
-----

1. Eliminate namespace and preprocessor pollution.
2. Add modern C++ features (move constructors, initialization_vector support, std::function, etc)
3. Make source code more readable.
4. Provide amalgamation header and source files for easy importing to existing projects.


Roadmap
-------

1. [x] Remove all code for C library.
2. [x] Remove all ISO strict code.
3. [ ] Clean and document files:
  - [ ] libjson.h
  - [ ] JSONOptions.h
  - [ ] JSONNode.h
  - [ ] JSONNode.cpp
  - [ ] internalJSONNode.h
  - [ ] internalJSONNode.cpp
  - [ ] ...
  - [ ] (dependency) libbase64++.h
  - [ ] (dependency) mempool.h
4. [ ] Convert build to gyp
5. [ ] Convert tests to Google Test
6. [ ] Move everything inside the namespace "json"
7. [ ] Add move constructors to classes:
  - [ ] JSONNode
  - [ ] internalJSONNode
  - [ ] ...
8. [ ] Add initialization_vector support to classes:
  - [ ] JSONNode
9. [ ] Update callbacks to use `std::function`
  - [ ] Debug callbacks
  - [ ] Stream callbacks
  - [ ] Mutex callbacks
  - [ ] Memory callbacks


Cleaning Checklist
------------------

- Convert tabs to spaces.
- Sort included files alphabetically.
- Align all code at 4-space tabs.
- Replace `#define` include guards with `#pragma once`
- Reformat preprocessor statements.
- Wrap all lines at 100 characters.
- Add doxygen-style documentation to all classes, functions, and member variables.
- Remove deprecated functionality.
