#pragma once
#include <vector>

#include "common/defs.hpp"

// big thank you to the people that contributed to ps3devwiki :-)
// https://www.psdevwiki.com/ps3/SELF_-_SPRX

typedef uint64_t Elf64_Addr;
typedef uint16_t Elf64_Half;
typedef uint64_t Elf64_Off;
typedef int32_t Elf64_Sword;
typedef int64_t Elf64_Sxword;
typedef uint32_t Elf64_Word;
typedef uint64_t Elf64_Lword;
typedef uint64_t Elf64_Xword;

/* 
https://www.psdevwiki.com/ps3/SELF_-_SPRX#SCE_specific_ELF_types_(e_type) 
SCE-specific definitions for e_type: 
*/
#define ET_SCE_EXEC 0xFE00
#define ET_SCE_RELEXEC 0xFE04
#define ET_SCE_STUBLIB 0xFE0C
#define ET_SCE_DYNEXEC 0xFE10
#define ET_SCE_DYNAMIC 0xFE18
#define ET_SCE_IOPRELEXEC 0xFF80
#define ET_SCE_IOPRELEXEC2 0xFF81
#define ET_SCE_EERELEXEC 0xFF90
#define ET_SCE_EERELEXEC2 0xFF91
#define ET_SCE_PSPRELEXEC 0xFFA0
#define ET_SCE_PPURELEXEC 0xFFA4
#define ET_SCE_ARMRELEXEC 0xFFA5
#define ET_SCE_PSPOVERLAY 0xFFA8
#define PT_SCE_RELA 0x60000000
#define PT_SCE_LICINFO_1 0x60000001
#define PT_SCE_LICINFO_2 0x60000002
#define PT_SCE_DYNLIBDATA 0x61000000
#define PT_SCE_PROCESS_PARAM 0x61000001
#define PT_SCE_MODULE_PARAM 0x61000002
#define PT_SCE_RELRO 0x61000010  // for ps4
#define PT_SCE_COMMENT 0x6FFFFF00
#define PT_SCE_LIBVERSION 0x6FFFFF01
#define PT_SCE_UNK_70000001 0x70000001
#define PT_SCE_IOPMOD 0x70000080
#define PT_SCE_EEMOD 0x70000090
#define PT_SCE_PSPRELA 0x700000A0
#define PT_SCE_PSPRELA2 0x700000A1
#define PT_SCE_PPURELA 0x700000A4
#define PT_SCE_SEGSYM 0x700000A8

struct ELF {
  u8 e_ident[16]; 
  u16 e_type;     
  u16 e_machine;  
  u32 e_version;  
  u64 e_entry;    
  u64 e_phoff;    
  u64 e_shoff;    
  u32 e_flags;    
  u16 e_ehsize;   
  u16 e_phentsize;
  u16 e_phnum;    
  u16 e_shentsize;
  u16 e_shnum;    
  u16 e_shstrndx; 
} __attribute__((packed));

struct program_identification_header {
  u64 program_authority_id;
  u32 program_vender_id;
  u32 program_type;
  u64 program_sceversion;
  u64 padding;
} __attribute__((packed));

struct ext_hdr {  
  u64 ext_hdr_version;
  u64 program_identification_hdr_offset;
  u64 ehdr_offset;
  u64 phdr_offset;
  u64 shdr_offset;
  u64 segment_ext_hdr_offset;
  u64 version_hdr_offset;
  u64 supplemental_hdr_offset;
  u64 supplemental_hdr_size;
  u64 padding;
} __attribute__((packed));

struct cf_header {
  uint32_t magic;
  uint32_t version;
  uint16_t attribute;
  uint16_t category;
  uint32_t ext_header_size;
  uint64_t file_offset;
  uint64_t file_size;
} __attribute__((packed));

struct Elf64_Phdr {
  Elf64_Word p_type;   
  Elf64_Word p_flags;  
  Elf64_Off p_offset;  
  Elf64_Addr p_vaddr;  
  Elf64_Addr p_paddr;  
  Elf64_Xword p_filesz;
  Elf64_Xword p_memsz; 
  Elf64_Xword p_align; 
};

using RawExecutableData = std::vector<u8>;

ELF parse_self(RawExecutableData&);