/** @file
  App Support.

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/WebAssemblyInterpreterLib.h>

extern UINTN  Argc;
extern CHAR16 **Argv;

EFI_STATUS
GetArg (
  VOID
  );

EFI_STATUS
ReadFileToBuffer (
  IN  CHAR16                               *FileName,
  OUT UINTN                                *BufferSize,
  OUT VOID                                 **Buffer
  );

EFI_STATUS
EFIAPI
TestEntryMain (
  IN     EFI_HANDLE                 ImageHandle,
  IN     EFI_SYSTEM_TABLE           *SystemTable
  )
{
  EFI_STATUS  Status;
  UINTN       BufferSize;
  VOID        *Buffer;

  GetArg ();

  if (Argc != 2) {
    Print (L"Invalid file name\n");
    return EFI_SUCCESS;
  }

  Status = ReadFileToBuffer (Argv[1], &BufferSize, &Buffer);
  if (EFI_ERROR(Status)) {
    Print (L"Fail to read file\n");
    return EFI_SUCCESS;
  }

  WasmUefiInterpreter (Buffer, BufferSize);

  return EFI_SUCCESS;
}
