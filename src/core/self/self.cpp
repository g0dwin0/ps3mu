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

  cf_header_t* cf_header = (cf_header_t*)self_data.data();

  fmt::println("magic:                  {}", std::string_view(reinterpret_cast<const char*>(&cf_header->magic), 4));
  fmt::println("version:                {:#010x}", NTOHL(cf_header->version));
  fmt::println("attribute:              {:#010x}", NTOHS(cf_header->attribute));
  fmt::println("category:               {:#010x}", NTOHS(cf_header->category));
  fmt::println("extended header size:   {:#010x}", NTOHL(cf_header->ext_header_size));
  fmt::println("file offset size:       {:#010x}", NTOHLL(cf_header->file_offset));
  fmt::println("file size:              {:#010x}", NTOHLL(cf_header->file_size));

  if (cf_header->version != VERSION_PS3) {
    spdlog::error("invalid certified header, bad version (got {})", cf_header->version);
    assert(0);
  }

  ext_hdr* exthdr = (ext_hdr*)(self_data.data() + 0x20);

  fmt::println("=============== EXTENDED HEADER ===============");
  fmt::println("ext header version:         {:#010x}", NTOHLL(exthdr->ext_hdr_version));
  fmt::println("program_ident_hdr offset:   {:#010x}", NTOHLL(exthdr->program_identification_hdr_offset));
  fmt::println("ehdr_offset:                {:#010x}", NTOHLL(exthdr->ehdr_offset));
  fmt::println("phdr_offset:                {:#010x}", NTOHLL(exthdr->phdr_offset));
  fmt::println("shdr_offset:                {:#010x}", NTOHLL(exthdr->shdr_offset));
  fmt::println("segment_ext_hdr_offset:     {:#010x}", NTOHLL(exthdr->segment_ext_hdr_offset));
  fmt::println("version_hdr_offset:         {:#010x}", NTOHLL(exthdr->version_hdr_offset));
  fmt::println("supplemental_hdr_offset:    {:#010x}", NTOHLL(exthdr->supplemental_hdr_offset));
  fmt::println("supplemental_hdr_size:      {:#010x}", NTOHLL(exthdr->supplemental_hdr_size));

  program_identification_header* program_ident_hdr = (program_identification_header*)(self_data.data() + exthdr->program_identification_hdr_offset);

  fmt::println("=============== Program Identification Header ===============");
  fmt::println("program_authority_id:       {:#010x}", NTOHLL(program_ident_hdr->program_authority_id));
  fmt::println("program_vender_id:          {:#010x}", NTOHL(program_ident_hdr->program_vender_id));
  fmt::println("program_type:               {:#010x}", NTOHL(program_ident_hdr->program_type));
  fmt::println("program_sceversion:         {:#010x}", NTOHLL(program_ident_hdr->program_sceversion));

  elf_header* elf_hdr = (elf_header*)(self_data.data() + exthdr->ehdr_offset);
  fmt::println("=============== ELF Header ===============");
  fmt::println("e_ident:                    {}", std::string_view(reinterpret_cast<const char*>(&elf_hdr->e_ident), 16));
  //   fmt::println("e_ident:       {:#010x}", NTOHLL(elf_hdr->e_ident));
  fmt::println("e_type:                     {:#010x}", NTOHS(elf_hdr->e_type));
  fmt::println("e_machine:                  {:#010x}", NTOHS(elf_hdr->e_machine));
  fmt::println("e_version:                  {:#010x}", NTOHL(elf_hdr->e_version));
  fmt::println("e_entry:                    {:#010x}", NTOHLL(elf_hdr->e_entry));
  fmt::println("e_phoff:                    {:#010x}", NTOHLL(elf_hdr->e_phoff));
  fmt::println("e_shoff:                    {:#010x}", NTOHLL(elf_hdr->e_shoff));
  fmt::println("e_flags:                    {:#010x}", NTOHL(elf_hdr->e_flags));
  fmt::println("e_ehsize:                   {:#010x}", NTOHS(elf_hdr->e_ehsize));
  fmt::println("e_phentsize:                {:#010x}", NTOHS(elf_hdr->e_phentsize));
  fmt::println("e_phnum:                    {:#010x}", NTOHS(elf_hdr->e_phnum));
  fmt::println("e_shentsize:                {:#010x}", NTOHS(elf_hdr->e_shentsize));
  fmt::println("e_shnum:                    {:#010x}", NTOHS(elf_hdr->e_shnum));
  fmt::println("e_shstrndx:                 {:#010x}", NTOHS(elf_hdr->e_shstrndx));

  for (size_t i = 0; i < elf_hdr->e_phnum; i++) {
    Elf64_Phdr* phdr = (Elf64_Phdr*)(self_data.data() + exthdr->phdr_offset + (i * sizeof(Elf64_Phdr)));

    NTOHL(phdr->p_type);

    fmt::println("p type: {}", phdr->p_type);

    fmt::println("=============== Program Header {} ===============", i);
    fmt::println("p_type:                   {:#010x} - {}", phdr->p_type, segment_str_map.at(phdr->p_type));
    fmt::println("p_flags:                  {:#010x}", NTOHL(phdr->p_flags));
    fmt::println("p_offset:                 {:#010x}", NTOHLL(phdr->p_offset));
    fmt::println("p_vaddr:                  {:#010x}", NTOHLL(phdr->p_vaddr));
    fmt::println("p_paddr:                  {:#010x}", NTOHLL(phdr->p_paddr));
    fmt::println("p_filesz:                 {:#010x}", NTOHLL(phdr->p_filesz));
    fmt::println("p_memsz:                  {:#010x}", NTOHLL(phdr->p_memsz));
    fmt::println("p_align:                  {:#010x}", NTOHLL(phdr->p_align));

    program_headers.push_back(*phdr);
  }

  ELF_SHDR* shdr         = (ELF_SHDR*)(self_data.data() + exthdr->shdr_offset + (elf_hdr->e_shstrndx * sizeof(ELF_SHDR)));
  u64 offset_to_shstrtab = ntohll(shdr->sh_offset) + cf_header->file_offset;

  for (size_t i = 0; i < elf_hdr->e_shnum; i++) {
    ELF_SHDR* shdr = (ELF_SHDR*)(self_data.data() + exthdr->shdr_offset + (i * sizeof(ELF_SHDR)));
    NTOHL(shdr->sh_type);
    NTOHLL(shdr->sh_offset);
    NTOHL(shdr->sh_name);

    fmt::println("=============== Section Header {} ===============", i);

    fmt::println("sh_name:                  {} ({:#08x})", get_section_name_by_sh_name(shdr->sh_name, self_data, offset_to_shstrtab), shdr->sh_name);
    fmt::println("sh_type:                  {:#010x} - {}", shdr->sh_type, sh_type_map.at(shdr->sh_type));
    fmt::println("sh_flags:                 {:#010x}", NTOHLL(shdr->sh_flags));
    fmt::println("sh_addr:                  {:#010x}", NTOHLL(shdr->sh_addr));
    fmt::println("sh_offset:                {:#010x}", shdr->sh_offset + cf_header->file_offset);  // relative from ELF header start
    fmt::println("sh_size:                  {:#010x}", NTOHLL(shdr->sh_size));
    fmt::println("sh_link:                  {:#010x}", NTOHL(shdr->sh_link));
    fmt::println("sh_info:                  {:#010x}", NTOHL(shdr->sh_info));
    fmt::println("sh_addralign:             {:#010x}", NTOHLL(shdr->sh_addralign));
    fmt::println("sh_entsize:               {:#010x}", NTOHLL(shdr->sh_entsize));
  }

  return {.cf_header = *cf_header, .ext_hdr = *exthdr, .elf_header = *elf_hdr, .program_headers = program_headers};
}

void load_self(RawExecutableData& self_data, const ExecutableInfo& info, std::vector<u8>& memory) {
  u64 offset_to_file = info.cf_header.file_offset;
  for (const auto& phdr : info.program_headers) {

    if(phdr.p_type != PT_LOAD) continue;

    u64 start = phdr.p_offset + offset_to_file;

    for(size_t i = 0; i < phdr.p_filesz; i++) {
      memory.at(phdr.p_vaddr + i) = self_data.at(start+i);
    }
    
  }

  fmt::println("all segments loaded");
}