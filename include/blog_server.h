#pragma once

#include "blog_config.h"
#include "config_manager.h"
#include "database.h"
#include "inja.hpp"
#include "network/sf_http_server.hpp"
// #include "network/sf_websocket_param_t.hpp"

using namespace skyfire;

class blog_server : public sf_make_instance_t<blog_server, sf_empty_class> {
private:
    std::shared_ptr<sf_http_server> server__;
    std::shared_ptr<config_manager> config__;

    inja::Environment env;

    blog_config blog_config__;

    std::shared_ptr<database> database__ = nullptr;

    static nlohmann::json sf_json_to_nlo_json(const sf_json& js)
    {
        return nlohmann::json::parse(js.to_string());
    }

    static bool check_param(const sf_http_param_t& param,
        const vector<string>& keys)
    {
        for (auto& p : keys) {
            if (!param.contains(p)) {
                return false;
            }
        }
        return true;
    }

    void admin_root(const sf_http_request& req, sf_http_response& res);

    void admin_login(const sf_http_request& req, sf_http_response& res);

    bool admin_check(const sf_http_request& req, sf_http_response& res);

    void user_info(const sf_http_request& req, sf_http_response& res);

    void set_base_info(const sf_http_request& req, sf_http_response& res);

    void change_password(const sf_http_request& req, sf_http_response& res);

    void logout(const sf_http_request& req, sf_http_response& res);

    void setup_server(const sf_http_server_config& server_conf);

    void group_info(const sf_http_request& req, sf_http_response& res);

    sf_json group_info();

    void add_big_group(const sf_http_request& req, sf_http_response& res);

    void add_sub_group(const sf_http_request& req, sf_http_response& res);

    void delete_big_group(const sf_http_request& req, sf_http_response& res);

    void delete_sub_group(const sf_http_request& req, sf_http_response& res);

    void rename_big_group(const sf_http_request& req, sf_http_response& res);

    void rename_sub_group(const sf_http_request& req, sf_http_response& res);

    void add_label(const sf_http_request& req, sf_http_response& res);

    void rename_label(const sf_http_request& req, sf_http_response& res);

    void get_label(const sf_http_request& req, sf_http_response& res);

    void delete_label(const sf_http_request& req, sf_http_response& res);

    void blog_list(const sf_http_request& req, sf_http_response& res);

    void get_blog(const sf_http_request& req, sf_http_response& res);

    void draft_list(const sf_http_request& req, sf_http_response& res);

    void delete_draft_list(const sf_http_request& req, sf_http_response& res);

    void update_draft(const sf_http_request& req, sf_http_response& res);

    void delete_draft(const sf_http_request& req, sf_http_response& res);

    void add_blog(const sf_http_request& req, sf_http_response& res);

    void editor(const sf_http_request& req, sf_http_response& res);

    void get_draft(const sf_http_request& req, sf_http_response& res);

    void set_top(const sf_http_request& req, sf_http_response& res);

    void set_top_bat(const sf_http_request& req, sf_http_response& res);

    void set_hide_bat(const sf_http_request& req, sf_http_response& res);

    void delete_blog_bat(const sf_http_request& req, sf_http_response& res);

    void update_blog_group_bat(const sf_http_request& req,
        sf_http_response& res);

    void add_blog_label_bat(const sf_http_request& req, sf_http_response& res);

    void set_hide(const sf_http_request& req, sf_http_response& res);

    void delete_blog(const sf_http_request& req, sf_http_response& res);

    void get_blog_content(const sf_http_request& req, sf_http_response& res);

    void add_blog_label(const sf_http_request& req, sf_http_response& res);

    void delete_blog_label(const sf_http_request& req, sf_http_response& res);

    void update_blog_group(const sf_http_request& req, sf_http_response& res);

    void user_get_blog(const sf_http_request& req, sf_http_response& res);

    void user_get_all_blog(const sf_http_request& req, sf_http_response& res);

    void user_get_content(const sf_http_request& req, sf_http_response& res);

    void user_get_label(const sf_http_request& req, sf_http_response& res);

    void get_blog_info(const sf_http_request& req, sf_http_response& res);

    void read_blog(const sf_http_request& req, sf_http_response& res);

    void uploaded_file_list(const sf_http_request& req, sf_http_response& res);

    void heart_beat(const sf_http_request& req, sf_http_response& res);

    void upload_file(const sf_http_request& req, sf_http_response& res);

    void delete_file(const sf_http_request& req, sf_http_response& res);

    void add_comment(const sf_http_request& req, sf_http_response& res);

    void delete_comment(const sf_http_request& req, sf_http_response& res);

    void audit_comment(const sf_http_request& req, sf_http_response& res);

    void get_comment(const sf_http_request& req, sf_http_response& res);

    void user_get_comment(const sf_http_request& req, sf_http_response& res);

    void get_audited_comment(const sf_http_request& req, sf_http_response& res);

    void download_md(const sf_http_request& req, sf_http_response& res, string id);

public:
    explicit blog_server(const std::string& config_file_path);

    bool start();
};
