# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/PP_ju/pico-sdk/tools/pioasm"
  "C:/Users/PP_ju/Documents/GitHub/ese5190-2022-lab2b-esp/lab/02_repl/REPL/build/pioasm"
  "C:/Users/PP_ju/Documents/GitHub/ese5190-2022-lab2b-esp/lab/02_repl/REPL/build/pico-sdk/src/rp2_common/cyw43_driver/pioasm"
  "C:/Users/PP_ju/Documents/GitHub/ese5190-2022-lab2b-esp/lab/02_repl/REPL/build/pico-sdk/src/rp2_common/cyw43_driver/pioasm/tmp"
  "C:/Users/PP_ju/Documents/GitHub/ese5190-2022-lab2b-esp/lab/02_repl/REPL/build/pico-sdk/src/rp2_common/cyw43_driver/pioasm/src/PioasmBuild-stamp"
  "C:/Users/PP_ju/Documents/GitHub/ese5190-2022-lab2b-esp/lab/02_repl/REPL/build/pico-sdk/src/rp2_common/cyw43_driver/pioasm/src"
  "C:/Users/PP_ju/Documents/GitHub/ese5190-2022-lab2b-esp/lab/02_repl/REPL/build/pico-sdk/src/rp2_common/cyw43_driver/pioasm/src/PioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/PP_ju/Documents/GitHub/ese5190-2022-lab2b-esp/lab/02_repl/REPL/build/pico-sdk/src/rp2_common/cyw43_driver/pioasm/src/PioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/PP_ju/Documents/GitHub/ese5190-2022-lab2b-esp/lab/02_repl/REPL/build/pico-sdk/src/rp2_common/cyw43_driver/pioasm/src/PioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
