[Defines]
  PLATFORM_NAME                  = MSM8916Pkg
  PLATFORM_GUID                  = 28f1a3bf-193a-47e3-a7b9-5a435eaab2ee
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010019
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = MSM8916Pkg/MSM8916Pkg.fdf

!include MSM8916Pkg/MSM8916Pkg.dsc

[PcdsFixedAtBuild.common]
  # System Memory (2GB)
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x800000000
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x80000000
#  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x810C00000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000      # 256K stack
  gMSM8916PkgTokenSpaceGuid.PcdUefiMemPoolBase|0x810D00000         # DXE Heap base address
  gMSM8916PkgTokenSpaceGuid.PcdUefiMemPoolSize|0x003300000         # UefiMemorySize, DXE heap size
  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x810C40000

  # Framebuffer (725x1334)
  gMSM8916PkgTokenSpaceGuid.PcdMipiFrameBufferAddress|0x87c0a8000
  gMSM8916PkgTokenSpaceGuid.PcdMipiFrameBufferWidth|752
  gMSM8916PkgTokenSpaceGuid.PcdMipiFrameBufferHeight|1334
  gMSM8916PkgTokenSpaceGuid.PcdMipiFrameBufferVisibleWidth|752
  gMSM8916PkgTokenSpaceGuid.PcdMipiFrameBufferVisibleHeight|1334
