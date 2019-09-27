#pragma once

#include "sqlite_orm.h"
#include "tools/sf_json.hpp"
#include "tools/sf_single_instance.hpp"
#include <string>

using namespace std;
using namespace skyfire;
using namespace sqlite_orm;

struct admin_user {
    int id;
    string name;
    string password;
};

struct blog_info {
    string title;
    string desc;
};

struct blog {
    int id;
    string title;
    string publish_time;
    int watch_number;
    int top;
    int sub_group;
    int hide;
};

struct draft {
    int id;
    string title;
    string content;
};

struct blog_content {
    int id;
    string content;
};

struct blog_big_group {
    int id;
    string group_name;
};

struct blog_sub_group {
    int id;
    int big_group;
    string group_name;
};

struct label {
    int id;
    string label_name;
};

struct blog_label {
    int blog_id;
    int label_id;
};

struct comment {
    int id;
    string name;
    int blog_id; // 文章id或者评论id
    string publish_time;
    string content;
    string qq;
    string email;
    int audit;
};

SF_JSONIFY(admin_user, id, name, password)
SF_JSONIFY(blog_info, title, desc)
SF_JSONIFY(blog, id, title, publish_time, watch_number, top, sub_group, hide)
SF_JSONIFY(draft, id, title, content)
SF_JSONIFY(blog_content, id, content)
SF_JSONIFY(blog_big_group, id, group_name)
SF_JSONIFY(blog_sub_group, id, big_group, group_name)
SF_JSONIFY(label, id, label_name)
SF_JSONIFY(blog_label, blog_id, label_id)
SF_JSONIFY(comment, id, name, blog_id, publish_time, content, qq, email, audit)

inline auto init_user_storage(const string& path)
{
    return make_storage(
        path,
        make_table("admin_user",
            make_column("id", &admin_user::id, autoincrement(),
                primary_key(), unique()),
            make_column("name", &admin_user::name, unique()),
            make_column("password", &admin_user::password)),
        make_table("blog_info", make_column("title", &blog_info::title),
            make_column("desc", &blog_info::desc)),
        make_table("blog",
            make_column("id", &blog::id, autoincrement(), primary_key(),
                unique()),
            make_column("title", &blog::title),
            make_column("publish_time", &blog::publish_time),
            make_column("watch_number", &blog::watch_number),
            make_column("top", &blog::top),
            make_column("sub_group", &blog::sub_group),
            make_column("hide", &blog::hide)),
        make_table("draft",
            make_column("id", &draft::id, autoincrement(), primary_key(),
                unique()),
            make_column("title", &draft::title),
            make_column("content", &draft::content)),
        make_table("blog_content",
            make_column("id", &blog_content::id, primary_key()),
            make_column("content", &blog_content::content)),
        make_table("blog_big_group",
            make_column("id", &blog_big_group::id, autoincrement(),
                primary_key(), unique()),
            make_column("group_name", &blog_big_group::group_name)),
        make_table("blog_sub_group",
            make_column("id", &blog_sub_group::id, autoincrement(),
                primary_key(), unique()),
            make_column("big_group", &blog_sub_group::big_group),
            make_column("group_name", &blog_sub_group::group_name)),
        make_table("label",
            make_column("id", &label::id, autoincrement(), primary_key(),
                unique()),
            make_column("label_name", &label::label_name)),
        make_table("blog_label", make_column("blog_id", &blog_label::blog_id),
            make_column("label_id", &blog_label::label_id)),
        make_table("comment", make_column("id", &comment::id, autoincrement(), primary_key()),
            make_column("name", &comment::name),
            make_column("blog_id", &comment::blog_id),
            make_column("publish_time", &comment::publish_time),
            make_column("content", &comment::content),
            make_column("qq", &comment::qq),
            make_column("email", &comment::email),
            make_column("audit", &comment::audit)));
}

string hash_password(const std::string& password);

class database : public sf_single_instance<database> {
private:
    using StorageType = decltype(init_user_storage(""));
    std::unique_ptr<StorageType> storage__;

public:
    bool check_user_empty();

    bool check_blog_info_empty();

    shared_ptr<admin_user> check_user(const string& name,
        const string& password);

    shared_ptr<blog_big_group> check_big_group(const string& group_name);

    shared_ptr<blog_sub_group> check_sub_group(int ig_id,
        const string& group_name);

    int sub_group_count(int big_group);

    void delete_big_group(int big_group);

    void delete_sub_group(int sub_group);

    void update_user_info(const admin_user& user);

    void insert_user(const admin_user& user);

    vector<blog_big_group> all_big_group();

    vector<blog_sub_group> all_sub_group();

    map<int, int> sub_group_blog_count();

    int sub_group_blog_count(int sub_group);

    void insert_big_group(const blog_big_group& big_group);

    void insert_sub_group(const blog_sub_group& sub_group);

    void update_big_group(const blog_big_group& big_group);

    void update_sub_group(const blog_sub_group& sub_group);

    shared_ptr<blog_big_group> big_group(int id);

    shared_ptr<blog_sub_group> sub_group(int id);

    shared_ptr<label> check_label(const string& name);

    void update_label(const label& lab);

    void insert_label(const label& lab);

    void delete_label(int id);

    vector<label> all_label();

    shared_ptr<label> get_label(int id);

    vector<blog> all_blog();

    shared_ptr<blog_content> get_blog_content(int id);

    vector<label> blog_labels(int blog_id);

    void update_blog(const blog& b);

    void delete_blog(int blog_id);

    int insert_blog(const blog& b);

    shared_ptr<blog> get_blog(int blog_id);

    void insert_blog_content(const blog_content& bc);

    void delete_blog_content(int blog_id);

    void update_blog_content(const blog_content& bc);

    vector<draft> all_draft();

    int insert_draft(const draft& df);

    void update_draft(const draft& df);

    shared_ptr<draft> get_draft(int id);

    void delete_draft(int id);

    shared_ptr<blog_label> check_blog_label(int blog_id, int label_id);

    void insert_blog_label(const blog_label& bl);

    void delete_blog_label(int blog_id, int label_id);

    vector<blog> top_blogs(int sub_group);

    vector<blog> normal_blogs(int sub_group);

    void insert_blog_info(const blog_info& bi);

    void update_blog_info(const string& title, const string& desc);

    shared_ptr<blog_info> get_blog_info();

    vector<blog> top_blogs();

    vector<blog> normal_blogs();

    void insert_comment(const comment& c);

    void delete_comment(int id);

    void delete_blogs_comment(int blog_id);

    void update_comment(const comment& c);

    vector<comment> get_blog_comment(int blog_id);

    vector<comment> get_comment();

    shared_ptr<comment> get_comment(int comment_id);

    explicit database(const string& path);
};
