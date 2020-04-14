/** @file
  WASM interpreter UEFI header file

  Allows WASM binary to be interpretted under UEFI (edk2) environment

  Copyright (c) 2019 - 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _WEB_ASSEMBLY_INTERPRETER_LIB_H_
#define _WEB_ASSEMBLY_INTERPRETER_LIB_H_

#include <PiDxe.h>
#include <Uefi.h>

/**
   This service will perform interpretation of wasm format binary.

**/
EFI_STATUS
EFIAPI
WasmUefiInterpreter (
  IN  VOID  *Source,
  IN  UINTN      SourceSize
  );

#endif




