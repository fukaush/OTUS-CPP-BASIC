#include <iostream>
#include <unordered_map>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/strand.hpp>
#include <boost/system/system_error.hpp>
#include <boost/url/url.hpp>
#include <boost/url/parse.hpp>
#include "http_route.hpp"
#include "http_router.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace urls = boost::urls;

using system_error = boost::system::system_error;
using tcp = net::ip::tcp;

void fail(beast::error_code ec, char const* what) {
    std::cerr << what << ": " << ec.message() << "\n";
}

static HttpRouter httpRouter;

class session : public std::enable_shared_from_this<session> {
    beast::tcp_stream stream;
    http::request<http::string_body> req;
    beast::flat_buffer buffer;

    void doClose() {
        beast::error_code errorCode;

        stream.socket().shutdown(tcp::socket::shutdown_send, errorCode);
    }

    void onWrite(bool keepAlive, beast::error_code errorCode, size_t bytesTransferred) {
        boost::ignore_unused(bytesTransferred);

        if (errorCode) {
            return fail(errorCode, "write");
        }

        if (!keepAlive) {
            return doClose();
        }

        doRead();
    }

    template <class Body, class Allocator>
    http::message_generator handleRequest(http::request<Body, http::basic_fields<Allocator>>&& req) {
        urls::url_view url;

        try {
            url = urls::parse_uri_reference(req.target()).value();
        } catch (...) {
            std::cerr << "Error parse URL" << std::endl;

            http::response<http::string_body> res;

            res.result(http::status::bad_request);
            res.set(http::field::content_type, "application/json");
            res.body() = "400 Bad Request";
            res.prepare_payload();

            return res;
        }

        std::optional<std::reference_wrapper<HttpRoute>> route = httpRouter.getHttpRoute(url.path());

        if (!route) {
            http::response<http::string_body> res;

            res.result(http::status::not_found);
            res.set(http::field::content_type, "application/json");

            if (req.method() != http::verb::head) {
                res.body() = "404 Not Found";
                res.prepare_payload();
            } else {
                res.content_length(0);
            }

            return res;
        }

        return route->get().byMethod(url, req);
    }

    void sendResponse(http::message_generator&& msg) {
        beast::async_write(stream, std::move(msg), beast::bind_front_handler(&session::onWrite, shared_from_this(), msg.keep_alive()));
    }

    void onRead(beast::error_code errorCode, size_t bytesTransferred) {
        boost::ignore_unused(bytesTransferred);

        if (errorCode == http::error::end_of_stream) {
            return doClose();
        }

        if (errorCode) {
            fail(errorCode, "read");
        }

        sendResponse(handleRequest(std::move(req)));
    }

    void doRead() {
        req = {};

        stream.expires_after(std::chrono::seconds(30));
        http::async_read(stream, buffer, req, beast::bind_front_handler(&session::onRead, shared_from_this()));
    }

public:
    session(tcp::socket&& socket) : stream(std::move(socket)) {}

    void run() {
        net::dispatch(stream.get_executor(), beast::bind_front_handler(&session::doRead, shared_from_this()));
    }
};

class listener : public std::enable_shared_from_this<listener> {
    net::io_context& ioc;
    tcp::acceptor acceptor;

public:
    listener(net::io_context& _ioc, tcp::endpoint endpoint) : ioc{_ioc}, acceptor{net::make_strand(ioc)} {
        beast::error_code errorCode;

        acceptor.open(endpoint.protocol(), errorCode);

        if (errorCode) {
            throw system_error(errorCode, "open");
        }

        acceptor.set_option(net::socket_base::reuse_address(true), errorCode);

        if (errorCode) {
            throw system_error(errorCode, "set_option");
        }

        acceptor.bind(endpoint, errorCode);
        
        if (errorCode) {
            throw system_error(errorCode, "bind");
        }

        acceptor.listen(boost::asio::socket_base::max_listen_connections, errorCode);

        if (errorCode) {
            throw system_error(errorCode, "listen");
        }
    }

    void run() {
        doAccept();
    }

private:
    void onAccept(beast::error_code errorCode, tcp::socket socket) {
        if (errorCode) {
            return fail(errorCode, "accept");
        }

        std::make_shared<session>(std::move(socket))->run();

        doAccept();
    }

    void doAccept() {
        acceptor.async_accept(
            net::make_strand(ioc),
            beast::bind_front_handler(
                &listener::onAccept,
                shared_from_this()
            )
        );
    }
};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr
            << "Usage: "
            << argv[0]
            << " <address> <port> <threads>"
            << std::endl
            << "Example: " 
            << argv[0]
            << " 0.0.0.0 8080 1"
            << std::endl;

        return EXIT_FAILURE;
    }

    boost::asio::ip::address ipAddress = net::ip::make_address(argv[1]);
    unsigned short port = static_cast<unsigned short>(std::atoi(argv[2]));
    int const threadsCount = std::max<int>(1, std::atoi(argv[3]));
    net::io_context ioc{threadsCount};
    std::vector<std::thread> threads;

    HttpRoute notFound(
        "/404",
        HttpRouteHandlersBuilder()
            .get([](HttpRouteMethodResponseType&& res, HttpRouteMethodRequestType& req) {
                res.body() = "404 Page not found";
                res.prepare_payload();

                return res;
            })
            .build()
    );

    HttpRoute internalServerError(
        "/500",
        HttpRouteHandlersBuilder()
            .get([](HttpRouteMethodResponseType&& res, HttpRouteMethodRequestType& req) {
                res.body() = "500 Internal server error";
                res.prepare_payload();

                return res;
            })
            .build()
    );

    HttpRoute test(
        "/test",
        HttpRouteHandlersBuilder()
            .get([](HttpRouteMethodResponseType&& res, HttpRouteMethodRequestType& req) {
                res.body() = "get called";
                res.prepare_payload();

                return res;
            })
            .post([](HttpRouteMethodResponseType&& res, HttpRouteMethodRequestType& req) {
                return res;
            })
            .put([](HttpRouteMethodResponseType&& res, HttpRouteMethodRequestType& req) {
                return res;
            })
            .del([](HttpRouteMethodResponseType&& res, HttpRouteMethodRequestType& req) {
                res.body() = "delete called";
                res.prepare_payload();

                return res;
            })
            .build()
    );

    httpRouter.addRoute(std::move(notFound));
    httpRouter.addRoute(std::move(internalServerError));
    httpRouter.addRoute(std::move(test));

    try {
        std::make_shared<listener>(ioc, tcp::endpoint(ipAddress, port))->run();
        threads.reserve(threadsCount);

        for (int i = 0; i < threadsCount - 1; i++) {
            threads.emplace_back([&ioc] {
                ioc.run();
            });
        }

        ioc.run();

        return 0;
    } catch (const system_error& e) {
        fail(e.code(), e.what());
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }

    return EXIT_FAILURE;
}