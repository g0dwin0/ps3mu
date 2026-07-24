#include "nid/nid.hpp"

std::unordered_map<u32, NidTableMapEntry>& get_nid_map() {
    static std::unordered_map<u32, NidTableMapEntry> m = [] {
        std::unordered_map<u32, NidTableMapEntry> tmp;
        tmp.reserve(std::size(nid_table_map));
        for (auto& e : nid_table_map) {
            tmp.emplace(e.key, NidTableMapEntry{e.name, e.func_ptr, e.toc});
        }
        return tmp;
    }();
    return m;
}