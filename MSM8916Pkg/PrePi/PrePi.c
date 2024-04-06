/** @file

  Copyright (c) 2011-2017, ARM Limited. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>

#include <Library/CacheMaintenanceLib.h>
#include <Library/DebugAgentLib.h>
#include <Library/PrePiLib.h>
#include <Library/PrintLib.h>
#include <Library/PrePiHobListPointerLib.h>
#include <Library/TimerLib.h>
#include <Library/PerformanceLib.h>

#include <Ppi/GuidedSectionExtraction.h>
#include <Ppi/ArmMpCoreInfo.h>
#include <Ppi/SecPerformance.h>

#include "PrePi.h"

VOID EFIAPI ProcessLibraryConstructorList(VOID);

VOID ChickenBitSetup(VOID)
{
  DEBUG((EFI_D_INFO | EFI_D_LOAD, "Applying chicken bits\n"));
  __asm__ volatile(
    // "unlock the core for debugging"
    "msr OSLAR_EL1, xzr\n"

    /* Common to all Apple targets */
    "mrs    x28, S3_0_C15_C4_1\n"
    "orr    x28, x28, #0x800\n" //ARM64_REG_HID4_DisDcMVAOps
    "orr    x28, x28, #0x100000000000\n" //ARM64_REG_HID4_DisDcSWL2Ops
    "msr    S3_0_C15_C4_1, x28\n"
    "isb    sy\n"

    /* dont die in wfi kthx */
    "mrs     x28, S3_5_C15_C5_0\n"
    "bic     x28, x28, #0x3000000\n"
    "orr     x28, x28, #0x2000000\n"
    "msr     S3_5_C15_C5_0, x28\n"

    "isb sy\n"
    "dsb sy\n"
  );
  DEBUG((EFI_D_INFO | EFI_D_LOAD, "Applied chicken bits\n"));
}

VOID UartInit(VOID)
{
  for (UINT64 addr = 0x87c0a8000ull; addr < 0x87c0a8000ull + (752 * 1334 * 4); addr += 4) {
    MmioWrite32(addr, 0);
  }

  SerialPortInitialize();

  DEBUG((EFI_D_INFO, "\nEDK2 on iPhone 8 (AArch64)\n"));
  DEBUG(
      (EFI_D_INFO, "Firmware version %s built %a %a\n\n",
       (CHAR16 *)PcdGetPtr(PcdFirmwareVersionString), __TIME__, __DATE__));
}

VOID Main (IN  UINT64  StartTimeStamp)
{
  EFI_HOB_HANDOFF_INFO_TABLE  *HobList;
  EFI_STATUS                  Status;

  UINTN MemoryBase     = 0;
  UINTN MemorySize     = 0;
  UINTN UefiMemoryBase = 0;
  UINTN UefiMemorySize = 0;
  UINTN StacksBase     = 0;
  UINTN StacksSize     = 0;
  
  // Architecture-specific initialization
  // Enable Floating Point
  ArmEnableVFP();

  /* Enable program flow prediction, if supported */
  ArmEnableBranchPrediction();

  // Declare UEFI region
  MemoryBase     = FixedPcdGet64(PcdSystemMemoryBase);
  MemorySize     = FixedPcdGet64(PcdSystemMemorySize);
  UefiMemoryBase = FixedPcdGet64(PcdUefiMemPoolBase);
  UefiMemorySize = FixedPcdGet64(PcdUefiMemPoolSize);
  StacksSize     = FixedPcdGet64(PcdPrePiStackSize);
  StacksBase     = UefiMemoryBase + UefiMemorySize - StacksSize;

  DEBUG(
      (EFI_D_INFO | EFI_D_LOAD,
       "UEFI Memory Base = 0x%llx, Size = 0x%llx, Stack Base = 0x%llx, Stack "
       "Size = 0x%llx\n",
       (VOID *)UefiMemoryBase, UefiMemorySize, (VOID *)StacksBase, StacksSize));
       
  /* Setup chicken bits */
  ChickenBitSetup();
  /*We're not making a soup here yet.*/

  // Declare the PI/UEFI memory region
  // Set up HOB
  HobList = HobConstructor(
      (VOID *)UefiMemoryBase, UefiMemorySize, (VOID *)UefiMemoryBase,
      (VOID *)StacksBase);
  PrePeiSetHobList (HobList);

  // Invalidate cache
  InvalidateDataCacheRange(
      (VOID *)(UINTN)PcdGet64(PcdFdBaseAddress), PcdGet32(PcdFdSize));

  DEBUG((EFI_D_INFO, "Going into MemoryPeim\n"));
  // Initialize MMU
  Status = MemoryPeim(UefiMemoryBase, UefiMemorySize);

  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "Failed to configure MMU\n"));
    CpuDeadLoop();
  }

  DEBUG((EFI_D_LOAD | EFI_D_INFO, "MMU configured from device config\n"));

  // Add HOBs
  BuildStackHob ((UINTN)StacksBase, StacksSize);

  // TODO: Call CpuPei as a library
  BuildCpuHob (40, PcdGet8 (PcdPrePiCpuIoSize));

  // Set the Boot Mode
  SetBootMode (BOOT_WITH_FULL_CONFIGURATION);

  // Initialize Platform HOBs (CpuHob and FvHob)
  Status = PlatformPeim ();
  ASSERT_EFI_ERROR (Status);

  // Now, the HOB List has been initialized, we can register performance information
  //PERF_START (NULL, "PEI", NULL, StartTimeStamp);

  // SEC phase needs to run library constructors by hand.
  ProcessLibraryConstructorList ();

  // Assume the FV that contains the SEC (our code) also contains a compressed FV.
  Status = DecompressFirstFv ();
  ASSERT_EFI_ERROR (Status);

  // Load the DXE Core and transfer control to it
  Status = LoadDxeCoreFromFv (NULL, 0);
  ASSERT_EFI_ERROR (Status);
}

VOID
CEntryPoint ()
{
  UartInit();
  Main(0);
}
