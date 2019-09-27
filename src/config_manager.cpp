//
// Created by skyfire on 19-8-5.
//

#include "config_manager.h"
#include <fstream>
#include "tools/sf_utils.h"

using namespace skyfire;

config_manager::config_manager(const std::string &filename) {
    byte_array data;
    if (!sf_read_file(filename, data)) {
        return;
    }
    config__ = sf_json::from_string(to_string(data));
    inited__ = !config__.is_null();
}

bool config_manager::inited() const { return inited__; }
