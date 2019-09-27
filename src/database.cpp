#include "database.h"
#include "digestpp.hpp"
#include "tools/sf_logger.hpp"

database::database(const string& path)
{
    sf_info("database path", path);
    storage__ = make_unique<StorageType>(init_user_storage(path));
    storage__->sync_schema();
}

bool database::check_user_empty()
{
    return storage__->count<admin_user>() == 0;
}

bool database::check_blog_info_empty()
{
    return storage__->count<blog_info>() == 0;
}

shared_ptr<admin_user> database::check_user(const string& name,
    const string& password)
{
    auto users = storage__->get_all<admin_user>(
        where(c(&admin_user::name) == name and c(&admin_user::password) == hash_password(password)));
    if (users.empty()) {
        return nullptr;
    }
    return std::make_shared<admin_user>(users[0]);
}

void database::update_user_info(const admin_user& user)
{
    storage__->update(user);
}

string hash_password(const std::string& password)
{
    return digestpp::sha512().absorb(password).hexdigest();
}

void database::insert_user(const admin_user& user) { storage__->insert(user); }

vector<blog_big_group> database::all_big_group()
{
    return storage__->get_all<blog_big_group>();
}

vector<blog_sub_group> database::all_sub_group()
{
    return storage__->get_all<blog_sub_group>();
}

map<int, int> database::sub_group_blog_count()
{
    map<int, int> ret;
    auto data = storage__->select(columns(&blog::sub_group, count(&blog::id)),
        group_by(&blog::sub_group));
    for (auto& p : data) {
        ret[get<0>(p)] = get<1>(p);
    }
    return ret;
}

int database::sub_group_blog_count(int sub_group)
{
    return storage__->count<blog>(where(c(&blog::sub_group) == sub_group));
}

shared_ptr<blog_big_group> database::check_big_group(const string& group_name)
{
    auto big_groups = storage__->get_all<blog_big_group>(
        where(c(&blog_big_group::group_name) == group_name));
    if (big_groups.empty()) {
        return nullptr;
    }
    return make_shared<blog_big_group>(big_groups[0]);
}

shared_ptr<blog_sub_group> database::check_sub_group(int big_group,
    const string& group_name)
{
    auto sub_groups = storage__->get_all<blog_sub_group>(
        where(c(&blog_sub_group::big_group) == big_group and c(&blog_sub_group::group_name) == group_name));
    if (sub_groups.empty()) {
        return nullptr;
    }
    return make_shared<blog_sub_group>(sub_groups[0]);
}

void database::insert_big_group(const blog_big_group& big_group)
{
    storage__->insert(big_group);
}

void database::insert_sub_group(const blog_sub_group& sub_group)
{
    storage__->insert(sub_group);
}

int database::sub_group_count(int big_group)
{
    return storage__->count<blog_sub_group>(
        where(c(&blog_sub_group::big_group) == big_group));
}

void database::delete_big_group(int big_group)
{
    storage__->remove<blog_big_group>(big_group);
}

void database::delete_sub_group(int sub_group)
{
    storage__->remove<blog_sub_group>(sub_group);
}

void database::update_big_group(const blog_big_group& big_group)
{
    storage__->update(big_group);
}

void database::update_sub_group(const blog_sub_group& sub_group)
{
    storage__->update(sub_group);
}

shared_ptr<blog_big_group> database::big_group(int id)
{
    return storage__->get_pointer<blog_big_group>(id);
}

shared_ptr<blog_sub_group> database::sub_group(int id)
{
    return storage__->get_pointer<blog_sub_group>(id);
}

shared_ptr<label> database::check_label(const string& name)
{
    auto data = storage__->get_all<label>(where(c(&label::label_name) == name));
    if (data.empty()) {
        return nullptr;
    }
    return make_shared<label>(data[0]);
}

void database::update_label(const label& lab) { storage__->update(lab); }

void database::insert_label(const label& lab) { storage__->insert(lab); }

void database::delete_label(int id) { storage__->remove<label>(id); }

vector<label> database::all_label() { return storage__->get_all<label>(); }

shared_ptr<label> database::get_label(int id)
{
    return storage__->get_pointer<label>(id);
}

vector<blog> database::all_blog() { return storage__->get_all<blog>(); }

shared_ptr<blog_content> database::get_blog_content(int id)
{
    return storage__->get_pointer<blog_content>(id);
}

vector<label> database::blog_labels(int blog_id)
{
    auto data = storage__->select(
        columns(&label::id, &label::label_name),
        join<blog_label>(on(c(&label::id) == &blog_label::label_id and c(&blog_label::blog_id) == blog_id)));
    vector<label> ret(data.size());
    for (auto i = 0UL; i < data.size(); ++i) {
        ret[i] = label { get<0>(data[i]), std::get<1>(data[i]) };
    }
    return ret;
}

int database::insert_blog(const blog& b) { return storage__->insert(b); }

void database::delete_blog(int blog_id)
{
    storage__->remove<blog>(blog_id);
    storage__->remove_all<blog_label>(
        where(c(&blog_label::blog_id) == blog_id));
    storage__->remove<blog_content>(blog_id);
}

void database::update_blog(const blog& b) { storage__->update(b); }

shared_ptr<blog> database::get_blog(int id)
{
    return storage__->get_pointer<blog>(id);
}

void database::insert_blog_content(const blog_content& bc)
{
    storage__->insert(bc);
}

void database::delete_blog_content(int blog_id)
{
    storage__->remove<blog_content>(blog_id);
}

void database::update_blog_content(const blog_content& bc)
{
    storage__->update(bc);
}

vector<draft> database::all_draft() { return storage__->get_all<draft>(); }

int database::insert_draft(const draft& df) { return storage__->insert(df); }

void database::update_draft(const draft& df) { storage__->update(df); }

shared_ptr<draft> database::get_draft(int id)
{
    return storage__->get_pointer<draft>(id);
}

void database::delete_draft(int id) { storage__->remove<draft>(id); }

shared_ptr<blog_label> database::check_blog_label(int blog_id, int label_id)
{
    auto data = storage__->get_all<blog_label>(
        where(c(&blog_label::blog_id) == blog_id and c(&blog_label::label_id) == label_id));
    if (data.empty()) {
        return nullptr;
    }
    return make_shared<blog_label>(data[0]);
}

void database::insert_blog_label(const blog_label& bl)
{
    storage__->insert(bl);
}

void database::delete_blog_label(int blog_id, int label_id)
{
    storage__->remove_all<blog_label>(
        where(c(&blog_label::label_id) == label_id and c(&blog_label::blog_id) == blog_id));
}

vector<blog> database::top_blogs(int sub_group)
{
    return storage__->get_all<blog>(where(c(&blog::sub_group) == sub_group and c(&blog::hide) == 0 and c(&blog::top) == 1));
}

vector<blog> database::normal_blogs(int sub_group)
{
    return storage__->get_all<blog>(where(c(&blog::sub_group) == sub_group and c(&blog::hide) == 0 and c(&blog::top) == 0));
}
vector<blog> database::top_blogs()
{
    return storage__->get_all<blog>(
        where(c(&blog::hide) == 0 and c(&blog::top) == 1));
}

vector<blog> database::normal_blogs()
{
    return storage__->get_all<blog>(
        where(c(&blog::hide) == 0 and c(&blog::top) == 0));
}

void database::insert_blog_info(const blog_info& bi) { storage__->insert(bi); }

void database::update_blog_info(const string& title, const string& desc)
{
    storage__->update_all(sqlite_orm::set(c(&blog_info::title) = title,
        c(&blog_info::desc) = desc));
}

shared_ptr<blog_info> database::get_blog_info()
{
    auto data = storage__->get_all<blog_info>();
    if (data.empty()) {
        return nullptr;
    }
    return make_shared<blog_info>(data[0]);
}

void database::insert_comment(const comment& c) { storage__->insert(c); }

void database::delete_comment(int id) { storage__->remove<comment>(id); }

void database::delete_blogs_comment(int blog_id)
{
    storage__->remove_all<comment>(
        where(c(&comment::blog_id) == blog_id));
}

void database::update_comment(const comment& c) { storage__->update(c); }

vector<comment> database::get_blog_comment(int blog_id)
{
    return storage__->get_all<comment>(where(c(&comment::blog_id) == blog_id));
}

vector<comment> database::get_comment()
{
    return storage__->get_all<comment>();
}

shared_ptr<comment> database::get_comment(int comment_id)
{
    return storage__->get_pointer<comment>(comment_id);
}