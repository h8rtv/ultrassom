#pragma once

#include <oatpp/web/protocol/http/Http.hpp>
#include <oatpp/core/macro/component.hpp>

#include "Persistence/UserDb.hpp"
#include "Dto/User.hpp"

class UserService {
private:
  typedef oatpp::web::protocol::http::Status Status;
private:
  OATPP_COMPONENT(std::shared_ptr<UserDb>, userDb); // Inject database component

public:
  oatpp::Object<User> createUser(const oatpp::Object<User>& dto) {
    auto dbResult = userDb->createUser(dto);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

    v_int32 id = oatpp::sqlite::Utils::getLastInsertRowId(dbResult->getConnection());
    auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<User>>>();
    return getUserById(id);
  }

  oatpp::Object<User> getUserById(v_int32 id) {
    auto dbResult = userDb->getUserById(id);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), Status::CODE_404, "User not found");

    auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<User>>>();
    OATPP_ASSERT_HTTP(result->size() == 1, Status::CODE_500, "Unknown error");

    return result[0];
  }

  oatpp::Object<User> getUserByName(const oatpp::String& name) {
    auto dbResult = userDb->getUserByName(name);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), Status::CODE_404, "User not found");

    auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<User>>>();
    OATPP_ASSERT_HTTP(result->size() == 1, Status::CODE_500, "Unknown error");

    return result[0];
  }
};
