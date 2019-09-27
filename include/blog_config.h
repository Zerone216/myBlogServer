#pragma once

#include "tools/sf_json.hpp"

using namespace skyfire;
using namespace std;

struct blog_config {
    string db_path;
    string static_path;
    string template_path;
    string uploaded_file_path;
};

SF_JSONIFY(blog_config, db_path, static_path, template_path, uploaded_file_path)
