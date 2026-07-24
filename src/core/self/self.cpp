#include "self/self.hpp"

#include "common.hpp"
#include "nid/nid.hpp"
#include "ppe.hpp"
#include "prx/prx.hpp"
#include "vmm/vmm.hpp"





std::string get_section_name_by_sh_name(u16 sh_name, const RawExecutableData& self_data, u64 offset_to_shstrtab) {
  std::string section_name;

  size_t ptr = 0;

  while (self_data.at(offset_to_shstrtab + sh_name + ptr) != '\0') {
    section_name += self_data.at(offset_to_shstrtab + sh_name + ptr);
    ptr++;
  }

  return section_name;
}

ExecutableInfo parse_exec(RawExecutableData& self_data) {
  std::vector<Elf64_Phdr> program_headers;
  ExecutableFileFormat file_format;
  ext_hdr_t _ext_hdr{};

  self_data.at(0) == '\x7f' ? file_format = ELF : file_format = SELF;

  spdlog::info("This is an {} file!", file_format == ELF ? "ELF" : "SELF");

  if (file_format == SELF) {
    cf_header_t* cf_header = (cf_header_t*)self_data.data();
    cf_header_t _cf_header;

    spdlog::info("magic:                  {}", std::string_view(reinterpret_cast<const char*>(&cf_header->magic), 4));
    spdlog::info("version:                {:#010x}", _cf_header.version = read_p_be<u32>(cf_header->version));
    spdlog::info("attribute:              {:#010x}", _cf_header.attribute = read_p_be<u16>(cf_header->attribute));
    spdlog::info("category:               {:#010x}", _cf_header.category = read_p_be<u16>(cf_header->category));
    spdlog::info("extended header size:   {:#010x}", _cf_header.ext_header_size = read_p_be<u32>(cf_header->ext_header_size));
    spdlog::info("file offset size:       {:#010x}", _cf_header.file_offset = read_p_be<u64>(cf_header->file_offset));
    spdlog::info("file size:              {:#010x}", _cf_header.file_size = read_p_be<u64>(cf_header->file_size));

    if (_cf_header.version != VERSION_PS3) assert(0);

    ext_hdr_t* exthdr = (ext_hdr_t*)(self_data.data() + 0x20);

    spdlog::info("=============== EXTENDED HEADER ===============");
    spdlog::info("ext header version:         {:#010x}", _ext_hdr.ext_hdr_version = read_p_be<u64>(exthdr->ext_hdr_version));
    spdlog::info("program_ident_hdr offset:   {:#010x}", _ext_hdr.program_identification_hdr_offset = read_p_be<u64>(exthdr->program_identification_hdr_offset));
    spdlog::info("ehdr_offset:                {:#010x}", _ext_hdr.ehdr_offset = read_p_be<u64>(exthdr->ehdr_offset));
    spdlog::info("phdr_offset:                {:#010x}", _ext_hdr.phdr_offset = read_p_be<u64>(exthdr->phdr_offset));
    spdlog::info("shdr_offset:                {:#010x}", _ext_hdr.shdr_offset = read_p_be<u64>(exthdr->shdr_offset));
    spdlog::info("segment_ext_hdr_offset:     {:#010x}", _ext_hdr.segment_ext_hdr_offset = read_p_be<u64>(exthdr->segment_ext_hdr_offset));
    spdlog::info("version_hdr_offset:         {:#010x}", _ext_hdr.version_hdr_offset = read_p_be<u64>(exthdr->version_hdr_offset));
    spdlog::info("supplemental_hdr_offset:    {:#010x}", _ext_hdr.supplemental_hdr_offset = read_p_be<u64>(exthdr->supplemental_hdr_offset));
    spdlog::info("supplemental_hdr_size:      {:#010x}", _ext_hdr.supplemental_hdr_size = read_p_be<u64>(exthdr->supplemental_hdr_size));

    program_identification_header* program_ident_hdr = (program_identification_header*)(self_data.data() + _ext_hdr.program_identification_hdr_offset);
    program_identification_header _program_ident_hdr;
    spdlog::info("=============== Program Identification Header ===============");
    spdlog::info("program_authority_id:       {:#016x}", _program_ident_hdr.program_authority_id = read_p_be<u64>(program_ident_hdr->program_authority_id));
    spdlog::info("program_vender_id:          {:#010x}", _program_ident_hdr.program_vender_id = read_p_be<u32>(program_ident_hdr->program_vender_id));
    spdlog::info("program_type:               {:#010x}", _program_ident_hdr.program_type = read_p_be<u32>(program_ident_hdr->program_type));
    spdlog::info("program_sceversion:         {:#016x}", _program_ident_hdr.program_sceversion = read_p_be<u64>(program_ident_hdr->program_sceversion));
  }

  elf_header_t* elf_hdr;

  if (file_format == ELF) {
    elf_hdr = (elf_header_t*)self_data.data();
  } else {
    elf_hdr = (elf_header_t*)(self_data.data() + _ext_hdr.ehdr_offset);
  }

  elf_header_t _elf_hdr;
  spdlog::info("=============== ELF Header ===============");
  spdlog::info("e_ident:                    {}", std::string_view(reinterpret_cast<const char*>(&elf_hdr->e_ident), 16));
  //   spdlog::info("e_ident:       {:#010x}", read_p_be<u64>(elf_hdr->e_ident));
  spdlog::info("e_type:                     {:#010x}", _elf_hdr.e_type = read_p_be<u16>(elf_hdr->e_type));
  spdlog::info("e_machine:                  {:#010x}", _elf_hdr.e_machine = read_p_be<u16>(elf_hdr->e_machine));
  spdlog::info("e_version:                  {:#010x}", _elf_hdr.e_version = read_p_be<u32>(elf_hdr->e_version));
  spdlog::info("e_entry:                    {:#010x}", _elf_hdr.e_entry = read_p_be<u64>(elf_hdr->e_entry));
  spdlog::info("e_phoff:                    {:#010x}", _elf_hdr.e_phoff = read_p_be<u64>(elf_hdr->e_phoff));
  spdlog::info("e_shoff:                    {:#010x}", _elf_hdr.e_shoff = read_p_be<u64>(elf_hdr->e_shoff));
  spdlog::info("e_flags:                    {:#010x}", _elf_hdr.e_flags = read_p_be<u32>(elf_hdr->e_flags));
  spdlog::info("e_ehsize:                   {:#010x}", _elf_hdr.e_ehsize = read_p_be<u16>(elf_hdr->e_ehsize));
  spdlog::info("e_phentsize:                {:#010x}", _elf_hdr.e_phentsize = read_p_be<u16>(elf_hdr->e_phentsize));
  spdlog::info("e_phnum:                    {:#010x}", _elf_hdr.e_phnum = read_p_be<u16>(elf_hdr->e_phnum));
  spdlog::info("e_shentsize:                {:#010x}", _elf_hdr.e_shentsize = read_p_be<u16>(elf_hdr->e_shentsize));
  spdlog::info("e_shnum:                    {:#010x}", _elf_hdr.e_shnum = read_p_be<u16>(elf_hdr->e_shnum));
  spdlog::info("e_shstrndx:                 {:#010x}", _elf_hdr.e_shstrndx = read_p_be<u16>(elf_hdr->e_shstrndx));

  for (size_t i = 0; i < _elf_hdr.e_phnum; i++) {
    Elf64_Phdr* phdr;

    if (file_format == ELF) {
      phdr = (Elf64_Phdr*)(self_data.data() + _elf_hdr.e_phoff + (i * sizeof(Elf64_Phdr)));
    } else {
      phdr = (Elf64_Phdr*)(self_data.data() + _ext_hdr.phdr_offset + (i * sizeof(Elf64_Phdr)));
    }

    Elf64_Phdr _phdr;

    _phdr.p_type = read_p_be<u32>(phdr->p_type);
    spdlog::info("=============== Program Header {} ===============", i);
    spdlog::info("p_type:                   {:#010x} - {}", (u32)_phdr.p_type, segment_str_map.at(_phdr.p_type));
    spdlog::info("p_flags:                  {:#010x}", _phdr.p_flags = read_p_be<u32>(phdr->p_flags));
    spdlog::info("p_offset:                 {:#016x}", _phdr.p_offset = read_p_be<u64>(phdr->p_offset));
    spdlog::info("p_vaddr:                  {:#016x}", _phdr.p_vaddr = read_p_be<u64>(phdr->p_vaddr));
    spdlog::info("p_paddr:                  {:#016x}", _phdr.p_paddr = read_p_be<u64>(phdr->p_paddr));
    spdlog::info("p_filesz:                 {:#016x}", _phdr.p_filesz = read_p_be<u64>(phdr->p_filesz));
    spdlog::info("p_memsz:                  {:#016x}", _phdr.p_memsz = read_p_be<u64>(phdr->p_memsz));
    spdlog::info("p_align:                  {:#016x}", _phdr.p_align = read_p_be<u64>(phdr->p_align));

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

  //   spdlog::info("=============== Section Header {} ===============", i);

  //   spdlog::info("sh_name:                  {} ({:#08x})", get_section_name_by_sh_name(shdr->sh_name, self_data, offset_to_shstrtab), shdr->sh_name);
  //   spdlog::info("sh_type:                  {:#010x} - {}", shdr->sh_type, sh_type_map.at(shdr->sh_type));
  //   spdlog::info("sh_flags:                 {:#010x}", read_p_be<u64>(shdr->sh_flags));
  //   spdlog::info("sh_addr:                  {:#010x}", read_p_be<u64>(shdr->sh_addr));
  //   spdlog::info("sh_offset:                {:#010x}", shdr->sh_offset + cf_header->file_offset);  // relative from ELF header start
  //   spdlog::info("sh_size:                  {:#010x}", read_p_be<u64>(shdr->sh_size));
  //   spdlog::info("sh_link:                  {:#010x}", read_p_be<u32>(shdr->sh_link));
  //   spdlog::info("sh_info:                  {:#010x}", read_p_be<u32>(shdr->sh_info));
  //   spdlog::info("sh_addralign:             {:#010x}", read_p_be<u64>(shdr->sh_addralign));
  //   spdlog::info("sh_entsize:               {:#010x}", read_p_be<u64>(shdr->sh_entsize));
  // }

  return {.cf_header = {}, .ext_hdr = {}, .elf_header = _elf_hdr, .program_headers = program_headers};
}

void load_self(RawExecutableData& self_data, const ExecutableInfo& info, std::vector<u8>& memory) {
  u64 offset_to_file = info.cf_header.file_offset;

  for (const auto& phdr : info.program_headers) {
    if (phdr.p_type != PT_LOAD || phdr.p_memsz == 0) continue;

    spdlog::info("load_self: mapping phdr with vaddr: {:#016x}", phdr.p_vaddr);

    u32 n_required_pages = (phdr.p_memsz / PPC64_PAGE_SIZE) + 1;

    spdlog::info("mapping {} bytes ({} pages) ", phdr.p_memsz, n_required_pages);
    u64 start = phdr.p_offset + offset_to_file;

    auto mapping = vmm.map(phdr.p_vaddr, phdr.p_memsz);

    if (!mapping) FATAL("failed to map");
spdlog::info("page number: {}", mapping->page_numbers);
    mapping->print();

    for (size_t file_img_ptr = 0; file_img_ptr < phdr.p_memsz; file_img_ptr++) {
      // spdlog::info("index: {}", (mapping->page_numbers.at(0) * PPC64_PAGE_SIZE) + file_img_ptr);
      if (file_img_ptr >= phdr.p_filesz) {
        memory.at((mapping->page_numbers.at(0) * PPC64_PAGE_SIZE) + file_img_ptr) = 0;
        continue;
      }

      memory.at((mapping->page_numbers.at(0) * PPC64_PAGE_SIZE) + file_img_ptr) = self_data.at(start + file_img_ptr);
    }
  }

  spdlog::info("all segments loaded");
  return;
  // determine imported modules & stubbed functions
  for (const auto& phdr : info.program_headers) {
    if (phdr.p_type != PT_SCE_LICINFO_2) continue;
    u64 virt_addr = phdr.p_vaddr;

    PRXParam* prx_param = (PRXParam*)(memory.data() + vmm.virt_to_phys(virt_addr));
    prx_param->print();

    size_t n_libraries = (prx_param->libstubend - prx_param->libstubstart) / ENTRY_SIZE;

    for (size_t i = 0; i < n_libraries; i++) {
      SceLibStub* library = (SceLibStub*)(memory.data() + vmm.virt_to_phys(prx_param->libstubstart + (i * ENTRY_SIZE)));

      char* libname   = (char*)(memory.data() + vmm.virt_to_phys(library->p_libname));
      u32* nid_base   = (u32*)(memory.data() + vmm.virt_to_phys(library->p_func_nid_table));
      u32* entry_base = (u32*)(memory.data() + vmm.virt_to_phys(library->p_func_entry_table));

      spdlog::info("library name: {}", libname);
      spdlog::info("nid table: {:#016x}", library->p_func_nid_table);
      spdlog::info("func table: {:#016x}", library->p_func_entry_table);

      // spdlog::info("number of functions: {}", library->stub_table.nfunc);
      for (size_t i = 0; i < library->stub_table.nfunc; i++) {
        u32 nid   = htonl(*(nid_base + i));
        u32 entry = htonl(*(entry_base + i));

        const auto& func_name = get_nid_map().at(nid).name;

        spdlog::info("nid[{}]: {:#016x}: {} - stub addr: {:#016x}", i, nid, func_name, entry);

        stub_map.insert({
            entry, {func_name, nid, NULL}
        });
      }

      // map_module_by_name(libname, memory, library);
    }

    return;
  }

  FATAL("could not find LICINFO_2");
}
