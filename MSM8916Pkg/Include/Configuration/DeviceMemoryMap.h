#ifndef _DEVICE_MEMORY_MAP_H_
#define _DEVICE_MEMORY_MAP_H_

#include <Library/ArmLib.h>

#define MAX_ARM_MEMORY_REGION_DESCRIPTOR_COUNT 128

/* Below flag is used for system memory */
#define SYSTEM_MEMORY_RESOURCE_ATTR_CAPABILITIES                               \
  EFI_RESOURCE_ATTRIBUTE_PRESENT | EFI_RESOURCE_ATTRIBUTE_INITIALIZED |        \
      EFI_RESOURCE_ATTRIBUTE_TESTED | EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |     \
      EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |                               \
      EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |                         \
      EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE |                            \
      EFI_RESOURCE_ATTRIBUTE_EXECUTION_PROTECTABLE

typedef enum { NoHob, AddMem, AddDev, HobOnlyNoCacheSetting, MaxMem } DeviceMemoryAddHob;

#define MEMORY_REGION_NAME_MAX_LENGTH 32

typedef struct {
  CHAR8                        Name[MEMORY_REGION_NAME_MAX_LENGTH];
  EFI_PHYSICAL_ADDRESS         Address;
  UINT64                       Length;
  DeviceMemoryAddHob           HobOption;
  EFI_RESOURCE_TYPE            ResourceType;
  EFI_RESOURCE_ATTRIBUTE_TYPE  ResourceAttribute;
  EFI_MEMORY_TYPE              MemoryType;
  ARM_MEMORY_REGION_ATTRIBUTES ArmAttributes;
} ARM_MEMORY_REGION_DESCRIPTOR_EX, *PARM_MEMORY_REGION_DESCRIPTOR_EX;

#define MEM_RES EFI_RESOURCE_MEMORY_RESERVED
#define MMAP_IO EFI_RESOURCE_MEMORY_MAPPED_IO
#define SYS_MEM EFI_RESOURCE_SYSTEM_MEMORY

#define SYS_MEM_CAP SYSTEM_MEMORY_RESOURCE_ATTR_CAPABILITIES
#define INITIALIZED EFI_RESOURCE_ATTRIBUTE_INITIALIZED
#define WRITE_COMBINEABLE EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE
#define UNCACHEABLE EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE

#define Reserv EfiReservedMemoryType
#define Conv EfiConventionalMemory
#define BsData EfiBootServicesData
#define BsCode EfiBootServicesCode
#define RtData EfiRuntimeServicesData
#define RtCode EfiRuntimeServicesCode
#define MmIO EfiMemoryMappedIO

#define NS_DEVICE ARM_MEMORY_REGION_ATTRIBUTE_NONSECURE_DEVICE
#define DEVICE ARM_MEMORY_REGION_ATTRIBUTE_NONSECURE_DEVICE
#define WRITE_THROUGH ARM_MEMORY_REGION_ATTRIBUTE_WRITE_THROUGH
#define WRITE_THROUGH_XN ARM_MEMORY_REGION_ATTRIBUTE_WRITE_THROUGH
#define WRITE_BACK ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK
#define WRITE_BACK_XN ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK
#define UNCACHED_UNBUFFERED ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED
#define UNCACHED_UNBUFFERED_XN ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED

static ARM_MEMORY_REGION_DESCRIPTOR_EX gDeviceMemoryDescriptorEx[] = {
/*                                                    EFI_RESOURCE_ EFI_RESOURCE_ATTRIBUTE_ EFI_MEMORY_TYPE ARM_REGION_ATTRIBUTE_
     MemLabel(32 Char.),  MemBase,    MemSize, BuildHob, ResourceType, ResourceAttribute, MemoryType, CacheAttributes
*/

//--------------------- Register ---------------------
//--------------------- DDR --------------------- */
    {"walter hartwell white",0x800000000, 0x40000000, AddMem, SYS_MEM, SYS_MEM_CAP,  Conv,   WRITE_BACK},

    //{"slut",              0x200000000, 0x100000000, AddDev, MMAP_IO, UNCACHEABLE,  MmIO,   UNCACHED_UNBUFFERED},
    //{"TZ1",               0x800000000, 0x000100000, AddDev, MMAP_IO, UNCACHEABLE,  MmIO,   NS_DEVICE},
    //{"HLOS 1",            0x800100000, 0x000f00000, AddMem, SYS_MEM, SYS_MEM_CAP,  Conv,   WRITE_BACK},
    //{"UEFI FD",           0x801000000, 0x000200000, AddMem, SYS_MEM, SYS_MEM_CAP,  BsData, WRITE_BACK},
    //{"HLOS 2",            0x801200000, 0x000a00000, AddMem, SYS_MEM, SYS_MEM_CAP,  Conv,   WRITE_BACK},
    //{"UEFI Stack",        0x801C00000, 0x000040000, AddMem, MEM_RES, UNCACHEABLE,  RtCode, NS_DEVICE},
    //{"CPU Vectors",       0x801C40000, 0x000010000, AddMem, SYS_MEM, SYS_MEM_CAP,  BsCode, WRITE_BACK},
    //{"HLOS 2",            0x801C50000, 0x07a458000, AddMem, SYS_MEM, SYS_MEM_CAP,  Conv,   WRITE_BACK},
    //{"Display Reserved",  0x87c0a8000, 0x0003d4000, AddMem, MEM_RES, SYS_MEM_CAP,  Reserv, WRITE_THROUGH_XN},
    //{"HLOS 2.5",          0x87c47c000, 0x000b2c000, AddMem, SYS_MEM, SYS_MEM_CAP,  Conv,   WRITE_BACK},
    //{"TZ0",               0x87cfa8000, 0x001570000, AddDev, MMAP_IO, UNCACHEABLE,  MmIO,   NS_DEVICE},
    //{"HLOS 3",            0x87e518000, 0x001ae8000, AddMem, SYS_MEM, SYS_MEM_CAP,  Conv,   WRITE_BACK},

    /* Terminator for MMU */
    { "Terminator", 0, 0, 0, 0, 0, 0, 0}};

#endif
