#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "common.hpp"
#include "common/defs.hpp"
#include "ppe.hpp"
#include "spdlog/fmt/bundled/base.h"

struct FuncEntry {
  BE<u32> ptr_to_func;
  u32 padding;
};

static constexpr u32 VERSION_PS3  = 2;
static constexpr u32 VERSION_VITA = 3;

// big thank you to the people that contributed to ps3devwiki :-)
// https://www.psdevwiki.com/ps3/SELF_-_SPRX

enum ExecutableFileFormat { SELF, ELF };

using Elf64_Addr   = uint64_t;
using Elf64_Half   = uint16_t;
using Elf64_Off    = uint64_t;
using Elf64_Sword  = int32_t;
using Elf64_Sxword = int64_t;
using Elf64_Word   = uint32_t;
using Elf64_Lword  = uint64_t;
using Elf64_Xword  = uint64_t;

typedef void (*FuncPtr)(PPE&);

// https://www.psdevwiki.com/ps3/SELF_-_SPRX#SCE_specific_ELF_types_(e_type)
// SCE-specific definitions for e_type:

static constexpr u32 ET_SCE_EXEC          = 0xFE00;
static constexpr u32 ET_SCE_RELEXEC       = 0xFE04;
static constexpr u32 ET_SCE_STUBLIB       = 0xFE0C;
static constexpr u32 ET_SCE_DYNEXEC       = 0xFE10;
static constexpr u32 ET_SCE_DYNAMIC       = 0xFE18;
static constexpr u32 ET_SCE_IOPRELEXEC    = 0xFF80;
static constexpr u32 ET_SCE_IOPRELEXEC2   = 0xFF81;
static constexpr u32 ET_SCE_EERELEXEC     = 0xFF90;
static constexpr u32 ET_SCE_EERELEXEC2    = 0xFF91;
static constexpr u32 ET_SCE_PSPRELEXEC    = 0xFFA0;
static constexpr u32 ET_SCE_PPURELEXEC    = 0xFFA4;
static constexpr u32 ET_SCE_ARMRELEXEC    = 0xFFA5;
static constexpr u32 ET_SCE_PSPOVERLAY    = 0xFFA8;
static constexpr u32 PT_NULL              = 0x00000000;
static constexpr u32 PT_LOAD              = 0x00000001;
static constexpr u32 PT_DYNAMIC           = 0x00000002;
static constexpr u32 PT_INTERP            = 0x00000003;
static constexpr u32 PT_NOTE              = 0x00000004;
static constexpr u32 PT_SHLIB             = 0x00000005;
static constexpr u32 PT_PHDR              = 0x00000006;
static constexpr u32 PT_TLS               = 0x00000007;
static constexpr u32 PT_LOOS              = 0x60000000;
static constexpr u32 PT_HIOS              = 0x6FFFFFFF;
static constexpr u32 PT_LOPROC            = 0x70000000;
static constexpr u32 PT_HIPROC            = 0x7FFFFFFF;
static constexpr u32 PT_SCE_RELA          = 0x60000000;
static constexpr u32 PT_SCE_LICINFO_1     = 0x60000001;
static constexpr u32 PT_SCE_LICINFO_2     = 0x60000002;
static constexpr u32 PT_SCE_DYNLIBDATA    = 0x61000000;
static constexpr u32 PT_SCE_PROCESS_PARAM = 0x61000001;
static constexpr u32 PT_SCE_MODULE_PARAM  = 0x61000002;
static constexpr u32 PT_SCE_RELRO         = 0x61000010;  // for ps4
static constexpr u32 PT_SCE_COMMENT       = 0x6FFFFF00;
static constexpr u32 PT_SCE_LIBVERSION    = 0x6FFFFF01;
static constexpr u32 PT_SCE_UNK_70000001  = 0x70000001;
static constexpr u32 PT_SCE_IOPMOD        = 0x70000080;
static constexpr u32 PT_SCE_EEMOD         = 0x70000090;
static constexpr u32 PT_SCE_PSPRELA       = 0x700000A0;
static constexpr u32 PT_SCE_PSPRELA2      = 0x700000A1;
static constexpr u32 PT_SCE_PPURELA       = 0x700000A4;
static constexpr u32 PT_SCE_SEGSYM        = 0x700000A8;
static constexpr u32 SHT_SCE_RELA         = 0x60000000;
static constexpr u32 SHT_SCE_NID          = 0x61000001;
static constexpr u32 SHT_SCE_IOPMOD       = 0x70000080;
static constexpr u32 SHT_SCE_EEMOD        = 0x70000090;
static constexpr u32 SHT_SCE_PSPRELA      = 0x700000A0;
static constexpr u32 SHT_SCE_PPURELA      = 0x700000A4;

static constexpr u32 SHT_NULL          = 0x0;
static constexpr u32 SHT_PROGBITS      = 0x1;
static constexpr u32 SHT_SYMTAB        = 0x2;
static constexpr u32 SHT_STRTAB        = 0x3;
static constexpr u32 SHT_RELA          = 0x4;
static constexpr u32 SHT_HASH          = 0x5;
static constexpr u32 SHT_DYNAMIC       = 0x6;
static constexpr u32 SHT_NOTE          = 0x7;
static constexpr u32 SHT_NOBITS        = 0x8;
static constexpr u32 SHT_REL           = 0x9;
static constexpr u32 SHT_SHLIB         = 0x0A;
static constexpr u32 SHT_DYNSYM        = 0x0B;
static constexpr u32 SHT_INIT_ARRAY    = 0x0E;
static constexpr u32 SHT_FINI_ARRAY    = 0x0F;
static constexpr u32 SHT_PREINIT_ARRAY = 0x10;
static constexpr u32 SHT_GROUP         = 0x11;
static constexpr u32 SHT_SYMTAB_SHNDX  = 0x12;
static constexpr u32 SHT_NUM           = 0x13;

static constexpr u32 PF_X          = 0x1;
static constexpr u32 PF_W          = 0x2;
static constexpr u32 PF_R          = 0x4;
static constexpr u32 PF_SPU_X      = 0x00100000;
static constexpr u32 PF_SPU_W      = 0x00200000;
static constexpr u32 PF_SPU_R      = 0x00400000;
static constexpr u32 PF_RSX_X      = 0x01000000;
static constexpr u32 PF_RSX_W      = 0x02000000;
static constexpr u32 PF_RSX_R      = 0x04000000;
static constexpr size_t ENTRY_SIZE = 0x2C;

struct elf_header_t {
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

struct ext_hdr_t {
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

struct cf_header_t {
  uint32_t magic;
  uint32_t version;
  uint16_t attribute;
  uint16_t category;
  uint32_t ext_header_size;
  // offset to start encapsulated data -- after SELF header
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
} __attribute__((packed));

typedef struct {
  uint32_t sh_name;
  uint32_t sh_type;
  uint64_t sh_flags;
  uint64_t sh_addr;
  uint64_t sh_offset;
  uint64_t sh_size;
  uint32_t sh_link;
  uint32_t sh_info;
  uint64_t sh_addralign;
  uint64_t sh_entsize;
} __attribute__((packed)) ELF_SHDR;

using RawExecutableData = std::vector<u8>;

const std::unordered_map<u32, std::string> sh_type_map = {

    {         SHT_NULL,          "SHT_NULL"},
    {     SHT_PROGBITS,      "SHT_PROGBITS"},
    {       SHT_SYMTAB,        "SHT_SYMTAB"},
    {       SHT_STRTAB,        "SHT_STRTAB"},
    {         SHT_RELA,          "SHT_RELA"},
    {         SHT_HASH,          "SHT_HASH"},
    {      SHT_DYNAMIC,       "SHT_DYNAMIC"},
    {         SHT_NOTE,          "SHT_NOTE"},
    {       SHT_NOBITS,        "SHT_NOBITS"},
    {          SHT_REL,           "SHT_REL"},
    {        SHT_SHLIB,         "SHT_SHLIB"},
    {       SHT_DYNSYM,        "SHT_DYNSYM"},
    {   SHT_INIT_ARRAY,    "SHT_INIT_ARRAY"},
    {   SHT_FINI_ARRAY,    "SHT_FINI_ARRAY"},
    {SHT_PREINIT_ARRAY, "SHT_PREINIT_ARRAY"},
    {        SHT_GROUP,         "SHT_GROUP"},
    { SHT_SYMTAB_SHNDX,  "SHT_SYMTAB_SHNDX"},
    {          SHT_NUM,           "SHT_NUM"},

    {     SHT_SCE_RELA,      "SHT_SCE_RELA"},
    {      SHT_SCE_NID,       "SHT_SCE_NID"},
    {   SHT_SCE_IOPMOD,    "SHT_SCE_IOPMOD"},
    {    SHT_SCE_EEMOD,     "SHT_SCE_EEMOD"},
    {  SHT_SCE_PSPRELA,   "SHT_SCE_PSPRELA"},
    {  SHT_SCE_PPURELA,   "SHT_SCE_PPURELA"},
};
const std::unordered_map<u32, std::string> p_flags_str_map = {
    {    PF_X,     "PF_X"},
    {    PF_W,     "PF_W"},
    {    PF_R,     "PF_R"},
    {PF_SPU_X, "PF_SPU_X"},
    {PF_SPU_W, "PF_SPU_W"},
    {PF_SPU_R, "PF_SPU_R"},
    {PF_RSX_X, "PF_RSX_X"},
    {PF_RSX_W, "PF_RSX_W"},
    {PF_RSX_R, "PF_RSX_R"},
};
const std::unordered_map<u32, std::string> segment_str_map = {

    {             PT_NULL,              "PT_NULL"},
    {             PT_LOAD,              "PT_LOAD"},
    {          PT_DYNAMIC,           "PT_DYNAMIC"},
    {           PT_INTERP,            "PT_INTERP"},
    {             PT_NOTE,              "PT_NOTE"},
    {            PT_SHLIB,             "PT_SHLIB"},
    {             PT_PHDR,              "PT_PHDR"},
    {              PT_TLS,               "PT_TLS"},
    {             PT_LOOS,              "PT_LOOS"},
    {             PT_HIOS,              "PT_HIOS"},
    {           PT_LOPROC,            "PT_LOPROC"},
    {           PT_HIPROC,            "PT_HIPROC"},
    {         PT_SCE_RELA,          "PT_SCE_RELA"},
    {    PT_SCE_LICINFO_1,     "PT_SCE_LICINFO_1"},
    {    PT_SCE_LICINFO_2,     "PT_SCE_LICINFO_2"},
    {   PT_SCE_DYNLIBDATA,    "PT_SCE_DYNLIBDATA"},
    {PT_SCE_PROCESS_PARAM, "PT_SCE_PROCESS_PARAM"},
    { PT_SCE_MODULE_PARAM,  "PT_SCE_MODULE_PARAM"},
    {        PT_SCE_RELRO,         "PT_SCE_RELRO"},
    {      PT_SCE_COMMENT,       "PT_SCE_COMMENT"},
    {   PT_SCE_LIBVERSION,    "PT_SCE_LIBVERSION"},
    { PT_SCE_UNK_70000001,  "PT_SCE_UNK_70000001"},
    {       PT_SCE_IOPMOD,        "PT_SCE_IOPMOD"},
    {        PT_SCE_EEMOD,         "PT_SCE_EEMOD"},
    {      PT_SCE_PSPRELA,       "PT_SCE_PSPRELA"},
    {     PT_SCE_PSPRELA2,      "PT_SCE_PSPRELA2"},
    {      PT_SCE_PPURELA,       "PT_SCE_PPURELA"},
    {       PT_SCE_SEGSYM,        "PT_SCE_SEGSYM"},
};

struct PRXParam {
  BE<u32> size;
  BE<u32> magic;
  BE<u32> version;
  BE<u32> pad0;
  BE<u32> libentstart;
  BE<u32> libentend;
  BE<u32> libstubstart;
  BE<u32> libstubend;
  BE<u16> ver;
  BE<u16> pad1;
  BE<u32> pad2;

  void print() {
    fmt::println("magic: {:#016x}", magic);
    fmt::println("version: {:#016x}", version);
    fmt::println("libentstart: {:#016x}", libentstart);
    fmt::println("libentend: {:#016x}", libentend);
    fmt::println("libstubstart: {:#016x}", libstubstart);
    fmt::println("libstubend: {:#016x}", libstubend);
    fmt::println("ver: {:#016x}", ver);
  }
};

struct sceKernelLibraryStubTable {  // size is 0xC
  BE<u16> size;
  BE<u16> version;
  BE<u16> attribute;
  BE<u16> nfunc;
  BE<u16> nvar;
  BE<u16> ntls;
};

struct sceKernelLibraryEntry {
  BE<u16> size;
  BE<u16> version;
  BE<u16> attribute;
  BE<u16> nfunc;
  BE<u16> nvar;
  BE<u16> ntls;

  BE<u8> hashinfo;
  BE<u8> hashinfotls;
  BE<u8> reserved;
  BE<u8> nidaltsets;

  BE<u32> p_libname;
  BE<u32> p_nid_table;
  BE<u32> p_entry_table;
};

struct SceLibStub {  // sceKernelLibraryStubTable_2C
  sceKernelLibraryStubTable stub_table;
  u32 padding;
  BE<u32> p_libname;
  BE<u32> p_func_nid_table;
  BE<u32> p_func_entry_table;
  BE<u32> p_var_nid_table;
  BE<u32> p_var_entry_table;
  BE<u32> p_tls_nid_table;
  BE<u32> p_tls_entry_table;
};

struct SceLibEntry {  // sceKernelLibraryEntryTable_1C
  sceKernelLibraryStubTable stub_table;
  u32 padding;
  BE<u32> p_libname;
  BE<u32> p_func_nid_table;
  BE<u32> p_func_entry_table;
  BE<u32> p_var_nid_table;
  BE<u32> p_var_entry_table;
  BE<u32> p_tls_nid_table;
  BE<u32> p_tls_entry_table;
};

struct ExecutableInfo {
  cf_header_t cf_header;
  ext_hdr_t ext_hdr;
  elf_header_t elf_header;
  std::vector<Elf64_Phdr> program_headers;
};
ExecutableInfo parse_exec(RawExecutableData&);
void load_self(RawExecutableData& self_data, const ExecutableInfo& info, std::vector<u8>& memory);

// using StubAddress = u64;
using EntryAddr = u32;
using NID       = u32;
using TOC       = u64;

struct StubMapEntry {
  std::string name;
  NID nid;
  FuncPtr fn_ptr;
};
inline std::unordered_map<EntryAddr, StubMapEntry> stub_map;