#pragma once
#include <unordered_map>
#include <string>
#include "http_route.hpp"

class HttpRouter {
    std::unordered_map<std::string, HttpRoute> routes;

public:
    void addRoute(HttpRoute&& route) {
        routes.emplace(route.getPoint(), std::move(route));
    }

    std::optional<std::reference_wrapper<HttpRoute>> getHttpRoute(std::string path) {
        auto route = routes.find(path);

        if (route != routes.end()) {
            return route->second;
        }

        return std::nullopt;
    }
};