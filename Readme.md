# These packages are to demonstrate how we enable a sandbox (such as WebAssembly) in UEFI firmware.

## Feature:
1) WebAssemblyPkg

This package is a wrapper for [wasm-micro-runtime](https://github.com/bytecodealliance/wasm-micro-runtime) to run wasm binary in UEFI environment.
It is similar to CryptoPkg which is a wrapper for openssl.

## Prerequisit:

  Build a standard C file to a wasm file with wasm SDK.
  
  1. [wasi-sdk](https://github.com/WebAssembly/wasi-sdk)
  
  2. [emsdk](https://emscripten.org/docs/getting_started/downloads.html)
  
  3. [rustwasm](https://rustwasm.github.io/)


## Build:
This repo uses below submodules:

  WebAssemblyPkg/Library/WebAssemblyInterpreterLib/wasm-micro-runtime

  NOTE: Current version only support GCC5 X64 build.

## Run:

  Build WebAssemblyPkg and generate WasmApp.efi.
  Run WasmApp.efi <Demo.wasm> in the UEFI shell.

  NOTE: Current project is just a demo to support printf(). More system APIs can be added later.

## Known limitation:
This package is only the sample code to show the concept.
It does not have a full validation and does not meet the production quality yet.
Any codes including the API definition, the libary and the drivers are subject to change.


