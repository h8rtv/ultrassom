#pragma once

#include "oatpp-sqlite/orm.hpp"

#include "Dto/User.hpp"

#include OATPP_CODEGEN_BEGIN(DbClient)

class UserDb : public oatpp::orm::DbClient {
public:

  UserDb(const std::shared_ptr<oatpp::orm::Executor>& executor)
    : oatpp::orm::DbClient(executor)
  {
    oatpp::orm::SchemaMigration migration(executor, "users");
    migration.addFile(1, DATABASE_MIGRATIONS "/001_users_init.sql");
    migration.migrate();

    auto version = executor->getSchemaVersion();
    OATPP_LOGD("UserDb", "Migration - OK. Version=%lld.", version);
  }

  QUERY(createUser,
        "INSERT INTO users"
        "(name) VALUES "
        "(:user.name);",
        PARAM(oatpp::Object<User>, user))

  QUERY(getUserById,
        "SELECT * FROM users WHERE id=:id;",
        PARAM(oatpp::Int32, id))

  QUERY(getUserByName,
        "SELECT * FROM users WHERE name=:name;",
        PARAM(oatpp::String, name))
};

#include OATPP_CODEGEN_END(DbClient)
