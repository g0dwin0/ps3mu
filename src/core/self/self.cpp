#include "self/self.hpp"

#include "common.hpp"

std::string get_section_name_by_sh_name(u16 sh_name, const RawExecutableData& self_data, u64 offset_to_shstrtab) {
  std::string section_name;

  size_t ptr = 0;

  while (self_data.at(offset_to_shstrtab + sh_name + ptr) != '\0') {
    section_name += self_data.at(offset_to_shstrtab + sh_name + ptr);
    ptr++;
  }

  return section_name;
}

ExecutableInfo parse_self(RawExecutableData& self_data) {
  static constexpr u32 VERSION_PS3 = 2;
  std::vector<Elf64_Phdr> program_headers;
  ext_hdr_t _ext_hdr;

  cf_header_t* cf_header = (cf_header_t*)self_data.data();
  cf_header_t _cf_header;

  fmt::println("magic:                  {}", std::string_view(reinterpret_cast<const char*>(&cf_header->magic), 4));
  fmt::println("version:                {:#010x}", _cf_header.version = read_p_be<u32>(cf_header->version));
  fmt::println("attribute:              {:#010x}", _cf_header.attribute = read_p_be<u16>(cf_header->attribute));
  fmt::println("category:               {:#010x}", _cf_header.category = read_p_be<u16>(cf_header->category));
  fmt::println("extended header size:   {:#010x}", _cf_header.ext_header_size = read_p_be<u32>(cf_header->ext_header_size));
  fmt::println("file offset size:       {:#010x}", _cf_header.file_offset = read_p_be<u64>(cf_header->file_offset));
  fmt::println("file size:              {:#010x}", _cf_header.file_size = read_p_be<u64>(cf_header->file_size));

  if (read_p_be<u32>(cf_header->version) != VERSION_PS3) {
    spdlog::error("invalid certified header, bad version (got {})", cf_header->version);
    assert(0);
  }

  ext_hdr_t* exthdr = (ext_hdr_t*)(self_data.data() + 0x20);

  fmt::println("=============== EXTENDED HEADER ===============");
  fmt::println("ext header version:         {:#010x}", _ext_hdr.ext_hdr_version = read_p_be<u64>(exthdr->ext_hdr_version));
  fmt::println("program_ident_hdr offset:   {:#010x}", _ext_hdr.program_identification_hdr_offset = read_p_be<u64>(exthdr->program_identification_hdr_offset));
  fmt::println("ehdr_offset:                {:#010x}", _ext_hdr.ehdr_offset = read_p_be<u64>(exthdr->ehdr_offset));
  fmt::println("phdr_offset:                {:#010x}", _ext_hdr.phdr_offset = read_p_be<u64>(exthdr->phdr_offset));
  fmt::println("shdr_offset:                {:#010x}", _ext_hdr.shdr_offset = read_p_be<u64>(exthdr->shdr_offset));
  fmt::println("segment_ext_hdr_offset:     {:#010x}", _ext_hdr.segment_ext_hdr_offset = read_p_be<u64>(exthdr->segment_ext_hdr_offset));
  fmt::println("version_hdr_offset:         {:#010x}", _ext_hdr.version_hdr_offset = read_p_be<u64>(exthdr->version_hdr_offset));
  fmt::println("supplemental_hdr_offset:    {:#010x}", _ext_hdr.supplemental_hdr_offset = read_p_be<u64>(exthdr->supplemental_hdr_offset));
  fmt::println("supplemental_hdr_size:      {:#010x}", _ext_hdr.supplemental_hdr_size = read_p_be<u64>(exthdr->supplemental_hdr_size));

  program_identification_header* program_ident_hdr = (program_identification_header*)(self_data.data() + _ext_hdr.program_identification_hdr_offset);
  program_identification_header _program_ident_hdr;
  fmt::println("=============== Program Identification Header ===============");
  fmt::println("program_authority_id:       {:#016x}", _program_ident_hdr.program_authority_id = read_p_be<u64>(program_ident_hdr->program_authority_id));
  fmt::println("program_vender_id:          {:#010x}", _program_ident_hdr.program_vender_id = read_p_be<u32>(program_ident_hdr->program_vender_id));
  fmt::println("program_type:               {:#010x}", _program_ident_hdr.program_type = read_p_be<u32>(program_ident_hdr->program_type));
  fmt::println("program_sceversion:         {:#016x}", _program_ident_hdr.program_sceversion = read_p_be<u64>(program_ident_hdr->program_sceversion));

  elf_header* elf_hdr = (elf_header*)(self_data.data() + _ext_hdr.ehdr_offset);
  elf_header _elf_hdr;
  fmt::println("=============== ELF Header ===============");
  fmt::println("e_ident:                    {}", std::string_view(reinterpret_cast<const char*>(&elf_hdr->e_ident), 16));
  //   fmt::println("e_ident:       {:#010x}", read_p_be<u64>(elf_hdr->e_ident));
  fmt::println("e_type:                     {:#010x}", _elf_hdr.e_type = read_p_be<u16>(elf_hdr->e_type));
  fmt::println("e_machine:                  {:#010x}", _elf_hdr.e_machine = read_p_be<u16>(elf_hdr->e_machine));
  fmt::println("e_version:                  {:#010x}", _elf_hdr.e_version = read_p_be<u32>(elf_hdr->e_version));
  fmt::println("e_entry:                    {:#010x}", _elf_hdr.e_entry = read_p_be<u64>(elf_hdr->e_entry));
  fmt::println("e_phoff:                    {:#010x}", _elf_hdr.e_phoff = read_p_be<u64>(elf_hdr->e_phoff));
  fmt::println("e_shoff:                    {:#010x}", _elf_hdr.e_shoff = read_p_be<u64>(elf_hdr->e_shoff));
  fmt::println("e_flags:                    {:#010x}", _elf_hdr.e_flags = read_p_be<u32>(elf_hdr->e_flags));
  fmt::println("e_ehsize:                   {:#010x}", _elf_hdr.e_ehsize = read_p_be<u16>(elf_hdr->e_ehsize));
  fmt::println("e_phentsize:                {:#010x}", _elf_hdr.e_phentsize = read_p_be<u16>(elf_hdr->e_phentsize));
  fmt::println("e_phnum:                    {:#010x}", _elf_hdr.e_phnum = read_p_be<u16>(elf_hdr->e_phnum));
  fmt::println("e_shentsize:                {:#010x}", _elf_hdr.e_shentsize = read_p_be<u16>(elf_hdr->e_shentsize));
  fmt::println("e_shnum:                    {:#010x}", _elf_hdr.e_shnum = read_p_be<u16>(elf_hdr->e_shnum));
  fmt::println("e_shstrndx:                 {:#010x}", _elf_hdr.e_shstrndx = read_p_be<u16>(elf_hdr->e_shstrndx));

  for (size_t i = 0; i < _elf_hdr.e_phnum; i++) {
    Elf64_Phdr* phdr = (Elf64_Phdr*)(self_data.data() + _ext_hdr.phdr_offset + (i * sizeof(Elf64_Phdr)));
    Elf64_Phdr _phdr;

    fmt::println("=============== Program Header {} ===============", i);
    fmt::println("p_type:                   {:#010x} - {}", _phdr.p_type = read_p_be<u32>(phdr->p_type), segment_str_map.at(_phdr.p_type));
    fmt::println("p_flags:                  {:#010x}", _phdr.p_flags = read_p_be<u32>(phdr->p_flags));
    fmt::println("p_offset:                 {:#016x}", _phdr.p_offset = read_p_be<u64>(phdr->p_offset));
    fmt::println("p_vaddr:                  {:#016x}", _phdr.p_vaddr = read_p_be<u64>(phdr->p_vaddr));
    fmt::println("p_paddr:                  {:#016x}", _phdr.p_paddr = read_p_be<u64>(phdr->p_paddr));
    fmt::println("p_filesz:                 {:#016x}", _phdr.p_filesz = read_p_be<u64>(phdr->p_filesz));
    fmt::println("p_memsz:                  {:#016x}", _phdr.p_memsz = read_p_be<u64>(phdr->p_memsz));
    fmt::println("p_align:                  {:#016x}", _phdr.p_align = read_p_be<u64>(phdr->p_align));

    program_headers.push_back(_phdr);
  }

  // ELF_SHDR* shdr = (ELF_SHDR*)(self_data.data() + exthdr->shdr_offset + (elf_hdr->e_shstrndx * sizeof(ELF_SHDR)));
  // ELF_SHDR _shdr;
  // u64 offset_to_shstrtab = read_p_be<u64>(shdr->sh_offset) + _cf_header.file_offset;

  // for (size_t i = 0; i < elf_hdr->e_shnum; i++) {
  //   ELF_SHDR* shdr = (ELF_SHDR*)(self_data.data() + exthdr->shdr_offset + (i * sizeof(ELF_SHDR)));
  //   read_p_be<u32>(shdr->sh_type);
  //   read_p_be<u64>(shdr->sh_offset);
  //   read_p_be<u32>(shdr->sh_name);

  //   fmt::println("=============== Section Header {} ===============", i);

  //   fmt::println("sh_name:                  {} ({:#08x})", get_section_name_by_sh_name(shdr->sh_name, self_data, offset_to_shstrtab), shdr->sh_name);
  //   fmt::println("sh_type:                  {:#010x} - {}", shdr->sh_type, sh_type_map.at(shdr->sh_type));
  //   fmt::println("sh_flags:                 {:#010x}", read_p_be<u64>(shdr->sh_flags));
  //   fmt::println("sh_addr:                  {:#010x}", read_p_be<u64>(shdr->sh_addr));
  //   fmt::println("sh_offset:                {:#010x}", shdr->sh_offset + cf_header->file_offset);  // relative from ELF header start
  //   fmt::println("sh_size:                  {:#010x}", read_p_be<u64>(shdr->sh_size));
  //   fmt::println("sh_link:                  {:#010x}", read_p_be<u32>(shdr->sh_link));
  //   fmt::println("sh_info:                  {:#010x}", read_p_be<u32>(shdr->sh_info));
  //   fmt::println("sh_addralign:             {:#010x}", read_p_be<u64>(shdr->sh_addralign));
  //   fmt::println("sh_entsize:               {:#010x}", read_p_be<u64>(shdr->sh_entsize));
  // }

  return {.cf_header = _cf_header, .ext_hdr = _ext_hdr, .elf_header = _elf_hdr, .program_headers = program_headers};
}

void load_self(RawExecutableData& self_data, const ExecutableInfo& info, std::vector<u8>& memory) {
  u64 offset_to_file = info.cf_header.file_offset;
  for (const auto& phdr : info.program_headers) {
    if (phdr.p_type != PT_LOAD) continue;

    u64 start = phdr.p_offset + offset_to_file;

    for (size_t i = 0; i < phdr.p_filesz; i++) {
      memory.at(phdr.p_vaddr + i) = self_data.at(start + i);
    }
  }

  fmt::println("all segments loaded");
}