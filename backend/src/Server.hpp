#pragma once

#include <oatpp/network/Server.hpp>

#include "Controller/UserController.hpp"
#include "Controller/ImageController.hpp"
#include "Component/App.hpp"

#include <iostream>

namespace Server {
  void run() {

    /* Register Components in scope of run() method */
    AppComponent components;

    /* Get router component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

    /* Create UserController and add all of its endpoints to router */
    auto user = std::make_shared<UserController>();
    user->addEndpointsToRouter(router);

    /* Create ImageController and add all of its endpoints to router */
    auto image = std::make_shared<ImageController>();
    image->addEndpointsToRouter(router);

    /* Get connection handler component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

    /* Get connection provider component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

    /* Create server which takes provided TCP connections and passes them to HTTP connection handler */
    oatpp::network::Server server(connectionProvider, connectionHandler);

    /* Print info about server port */
    OATPP_LOGI("MyApp", "Server running on port %s", connectionProvider->getProperty("port").getData());

    /* Run server */
    server.run();
    
    /* stop db connection pool */
    OATPP_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, dbConnectionProvider);
    dbConnectionProvider->stop();
  }
}