#pragma once
#include <string>
#include <functional>
#include <boost/beast/http.hpp>
#include <boost/beast/core.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace urls = boost::urls;

using HttpRouteMethodResponseType = http::response<http::string_body>;
using HttpRouteMethodRequestType = http::request<http::string_body, http::basic_fields<std::allocator<char>>>;
using HttpRouteMethodHandler = std::function<http::response<http::string_body>(HttpRouteMethodResponseType&& response, HttpRouteMethodRequestType& req)>;

struct HttpRouteMethods {
    HttpRouteMethodHandler get;
    HttpRouteMethodHandler post;
    HttpRouteMethodHandler put;
    HttpRouteMethodHandler del;
    HttpRouteMethodHandler patch;
    HttpRouteMethodHandler options;
    HttpRouteMethodHandler head;
    HttpRouteMethodHandler undefined;
};

class HttpRouteHandlersBuilder {
    HttpRouteMethods httpRouteMethods;

public:
    HttpRouteHandlersBuilder& post(HttpRouteMethodHandler handler) {
        httpRouteMethods.post = std::move(handler);

        return *this;
    }

    HttpRouteHandlersBuilder& put(HttpRouteMethodHandler handler) {
        httpRouteMethods.put = std::move(handler);

        return *this;
    }

    HttpRouteHandlersBuilder& get(HttpRouteMethodHandler handler) {
        httpRouteMethods.get = std::move(handler);

        return *this;
    }

    HttpRouteHandlersBuilder& del(HttpRouteMethodHandler handler) {
        httpRouteMethods.del = std::move(handler);

        return *this;
    }

    HttpRouteHandlersBuilder& patch(HttpRouteMethodHandler handler) {
        httpRouteMethods.patch = std::move(handler);

        return *this;
    }

    HttpRouteHandlersBuilder& options(HttpRouteMethodHandler handler) {
        httpRouteMethods.options = std::move(handler);

        return *this;
    }

    HttpRouteMethods build() {
        return std::move(httpRouteMethods);
    }
};

http::response<http::string_body> getDefaultResponse(HttpRouteMethodRequestType& req) {
    http::response<http::string_body> res;

    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.content_length(0);
    res.keep_alive(req.keep_alive());

    return res;
}

http::response<http::string_body> getUndefinedMethodResponse() {
    http::response<http::string_body> res;

    res.result(http::status::not_found);
    res.set(http::field::content_type, "application/json");
    res.body() = "Method does not exists";
    res.prepare_payload();

    return res;
}

class HttpRoute {
    std::string endpoint;
    HttpRouteMethods httpRouteMethods;
public:
    HttpRoute() = default;

    HttpRoute(std::string _endpoint, HttpRouteMethods _httpRouteMethods) : endpoint{_endpoint}, httpRouteMethods{_httpRouteMethods}  {}

    ~HttpRoute() = default;

    const std::string& getPoint() {
        return endpoint;
    }

    http::response<http::string_body> get(urls::url_view& url, HttpRouteMethodRequestType& req) {
        if (httpRouteMethods.get) {
            http::response<http::string_body> res = getDefaultResponse(req);

            res.body() = "get";
            res.prepare_payload();

            return httpRouteMethods.get(std::move(res), req);
        }

        return getDefaultResponse(req);
    }

    http::response<http::string_body> post(urls::url_view& url, HttpRouteMethodRequestType& req) {
        if (httpRouteMethods.post) {
            http::response<http::string_body> res = getDefaultResponse(req);

            res.body() = "post";
            res.prepare_payload();

            return httpRouteMethods.post(std::move(res), req);
        }

        return getDefaultResponse(req);
    }

    http::response<http::string_body> put(urls::url_view& url, HttpRouteMethodRequestType& req) {
        if (httpRouteMethods.put) {
            http::response<http::string_body> res = getDefaultResponse(req);

            res.body() = "put";
            res.prepare_payload();

            return httpRouteMethods.put(std::move(res), req);
        }

        return getDefaultResponse(req);
    }

    http::response<http::string_body> del(urls::url_view& url, HttpRouteMethodRequestType& req) {
        if (httpRouteMethods.del) {
            http::response<http::string_body> res = getDefaultResponse(req);

            res.body() = "del";
            res.prepare_payload();

            return httpRouteMethods.del(std::move(res), req);
        }

        return getDefaultResponse(req);
    }

    http::response<http::string_body> head(urls::url_view& url, HttpRouteMethodRequestType& req) {
        if (httpRouteMethods.head) {
            http::response<http::string_body> res = getDefaultResponse(req);

            return httpRouteMethods.head(std::move(res), req);
        }

        return getDefaultResponse(req);
    }

    http::response<http::string_body> patch(urls::url_view& url, HttpRouteMethodRequestType& req) {
        if (httpRouteMethods.patch) {
            http::response<http::string_body> res = getDefaultResponse(req);

            res.body() = "patch";
            res.prepare_payload();

            return httpRouteMethods.patch(std::move(res), req);
        }

        return getDefaultResponse(req);
    }

    http::response<http::string_body> options(urls::url_view& url, HttpRouteMethodRequestType& req) {
        if (httpRouteMethods.options) {
            http::response<http::string_body> res = getDefaultResponse(req);

            return httpRouteMethods.options(std::move(res), req);
        }

        return getDefaultResponse(req);
    }

    http::response<http::string_body> undefined(urls::url_view& url, HttpRouteMethodRequestType& req) {
        if (httpRouteMethods.undefined) {
            http::response<http::string_body> res = getDefaultResponse(req);

            return httpRouteMethods.undefined(std::move(res), req);
        }

        return getUndefinedMethodResponse();
    }

    http::response<http::string_body> byMethod(urls::url_view& url, HttpRouteMethodRequestType& req) {
        switch (req.method()) {
            case http::verb::get:
                return get(url, req);
            case http::verb::head:
                return head(url, req);
            case http::verb::post:
                return post(url, req);
            case http::verb::put:
                return put(url, req);
            case http::verb::delete_:
                return del(url, req);
            case http::verb::options:
                return options(url, req);
            default:
                return undefined(url, req);
        }
    }
};