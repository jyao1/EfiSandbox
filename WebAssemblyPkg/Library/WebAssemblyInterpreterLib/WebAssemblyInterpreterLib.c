/** @file
  WASM interpreter interfaces

  Copyright (c) 2019 - 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include "WebAssemblyInterpreterLib.h"
#include "bh_platform.h"
#include "bh_log.h"
#include "bh_assert.h"
#include "bh_read_file.h"
#include "wasm_export.h"

static int app_argc;
static char **app_argv;

bool
wasm_application_execute_main(wasm_module_inst_t module_inst,
                              int32_t argc, char *argv[]);

static void*
app_instance_main(wasm_module_inst_t module_inst)
{
  const char *exception;

  wasm_application_execute_main(module_inst, app_argc, app_argv);
  if ((exception = wasm_runtime_get_exception(module_inst)))
    DEBUG ((DEBUG_INFO, "Exception Result: - %s\n", exception));

  return NULL;
}

static char global_heap_buf[256 * 1024] = { 0 };

/**
  Interpret one wasm format binary

  If the wasm binary are interpreted and run successfully, then
  EFI_SUCCESS is returned.  
  Otherwise, RETURN_INVALID_PARAMETER is returned.

  @param  Source      The buffer containing the wasm binary path/name.
  @param  SourceSize  The size of source buffer.

  @retval EFI_SUCCESS Interpretion completed successfully
  @retval EFI_INVALID_PARAMETER
                      Not in a valid wasm format.
**/
EFI_STATUS
EFIAPI
WasmUefiInterpreter (
  IN VOID *   Source,
  IN UINTN          SourceSize
  )
{
  uint8 *wasm_file_buf = NULL;
  uint32 wasm_file_size;
  wasm_module_t wasm_module = NULL;
  wasm_module_inst_t wasm_module_inst = NULL;
  RuntimeInitArgs init_args;
  char error_buf[128];

  if (0 == SourceSize)
    return EFI_INVALID_PARAMETER;
  wasm_file_size = (uint32)SourceSize;
  wasm_file_buf  = Source;

  memset(&init_args, 0, sizeof(RuntimeInitArgs));

  init_args.mem_alloc_type = Alloc_With_Pool;
  init_args.mem_alloc_option.pool.heap_buf = global_heap_buf;
  init_args.mem_alloc_option.pool.heap_size = sizeof(global_heap_buf);

  /* initialize runtime environment */
  os_printf("%a", "wasm_runtime_full_init...\n");
  if (!wasm_runtime_full_init(&init_args)) {
    os_printf("Init runtime environment failed.\n");
    return EFI_INVALID_PARAMETER;
  }

  /* load WASM byte buffer from WASM bin file */
  //if (!(wasm_file_buf = (uint8*) bh_read_file_to_buffer(Source,
  //                               &wasm_file_size)))
  //  goto fail1;

  /* load WASM module */
  os_printf("wasm_runtime_load...\n");
  if (!(wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size,
            error_buf, sizeof(error_buf)))) {
    os_printf("%s\n", error_buf);
    goto fail1;
  }

  /* instantiate the module */
  os_printf("wasm_runtime_instantiate...\n");
  if (!(wasm_module_inst = wasm_runtime_instantiate(wasm_module,
                                                8 * 1024,
                                                8 * 1024,
                                                error_buf,
                                                sizeof(error_buf)))) {
    os_printf("%s\n", error_buf);
    goto fail2;
  }
  
  os_printf("app_instance_main...\n");
  app_instance_main(wasm_module_inst);

  /* destroy the module instance */
  wasm_runtime_deinstantiate(wasm_module_inst);
  wasm_runtime_unload(wasm_module);
  wasm_runtime_destroy();

  return EFI_SUCCESS;

fail2:
  /* unload the module */
  wasm_runtime_unload(wasm_module);

fail1:
  /* destroy runtime environment */
  wasm_runtime_destroy();

  return EFI_INVALID_PARAMETER;
}
