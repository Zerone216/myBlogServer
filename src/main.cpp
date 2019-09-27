#include "blog_server.h"
#include "tools/sf_logger.hpp"
#include "tools/sf_utils.hpp"

using namespace skyfire;

int main() {
    g_logger->add_level_file(SF_INFO_LEVEL, sf_make_time_str() + ".log");
    auto server = blog_server::make_instance("config.json");
    server->start();
}