#include <iostream>
#include <vector>
#include <filesystem>
#include <string>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <fstream>

void work(const boost::beast::http::request<boost::beast::http::dynamic_body> &request,
          boost::beast::http::response<boost::beast::http::dynamic_body> &response);
std::string mimeType(const std::string &path);

int main(int argc, char **argv) {
    if (argc == 1) {
        std::string name{std::filesystem::path{argv[0]}.filename().string()};
        std::cout << "use: " << name << " [port]" << std::endl;
    }
    uint16_t port{8080};
    if (argc == 2) {
        port = std::stoi(argv[1]);
    }
    try {
        boost::asio::io_context ioc{};
        boost::asio::ip::tcp::endpoint ep{boost::asio::ip::tcp::v4(), port};
        boost::asio::ip::tcp::acceptor acceptor{ioc, ep};
        std::cout << "server run on port: " << port << std::endl;
        while (true) {
            boost::asio::ip::tcp::socket socket{ioc};
            acceptor.accept(socket);
            boost::beast::http::request<boost::beast::http::dynamic_body> request{};
            boost::beast::flat_buffer buffer{};
            boost::beast::http::read(socket, buffer, request);
            boost::beast::http::response<boost::beast::http::dynamic_body> response{};
            work(request, response);
            boost::beast::http::write(socket, response);
            socket.close();
        }
    } catch(boost::system::system_error &e) {
        std::cout << e.what() << std::endl;
        return -1;
    }
}

void work(const boost::beast::http::request<boost::beast::http::dynamic_body> &request,
          boost::beast::http::response<boost::beast::http::dynamic_body> &response) {
    response.version(request.version());
    response.keep_alive(false);
    std::string path{request.target()};
    std::cout << "connection: " << path << std::endl;
    if (path == "/") {
        response.result(boost::beast::http::status::ok);
        response.set(boost::beast::http::field::content_type, "text/html");
        boost::beast::ostream(response.body()) << "<!DOCTYPE html><html><head><title>Main page</title><head>"
                                               << "<body><h1 align=\"center\">Hello, World!</h1></body></html>" << std::endl;
        response.content_length(response.body().size());
        return;
    }
    if (path[0] == '/') {
        path = path.substr(1);
    }
    if (path.find('?') != std::string::npos) {
        path = path.substr(0, path.find('?'));
    }
    if (std::filesystem::exists(path)) {
        std::streamsize sz{static_cast<std::streamsize>(std::filesystem::file_size(path))};
        std::vector<char> data(sz);
        std::ifstream file{path, std::ios::binary};
        file.read(data.data(), sz);
        file.close();
        response.result(boost::beast::http::status::ok);
        response.set(boost::beast::http::field::content_type, mimeType(path));
        boost::beast::ostream(response.body()).write(data.data(), sz);
        response.content_length(sz);
    } else {
        response.result(boost::beast::http::status::not_found);
        response.set(boost::beast::http::field::content_type, "text/html");
        boost::beast::ostream(response.body()) << "<!DOCTYPE html><html><head><title>Not Found</title><head>"
                                          << "<body><h1 align=\"center\">404 Not found</h1></body></html>" << std::endl;
        response.content_length(response.body().size());
    }
}

std::string mimeType(const std::string &path) {
    std::string ext{std::filesystem::path(path).extension().string()};
    if (ext == ".htm" || ext == ".html") return "text/html";
    if (ext == ".php") return "application/php";
    if (ext == ".css") return "text/css";
    if (ext == ".txt") return "text/plain";
    if (ext == ".js") return "application/javascript";
    if (ext == ".json") return "application/json";
    if (ext == ".gz") return "application/gzip";
    if (ext == ".tar") return "application/x-tar";
    if (ext == ".rar") return "application/vnd.rar";
    if (ext == ".zip") return "application/zip";
    if (ext == ".7z") return "application/x-7z-compressed";
    if (ext == ".pdf") return "application/pdf";
    if (ext == ".xml") return "application/xml";
    if (ext == ".swf") return "application/x-shockwave-flash";
    if (ext == ".flv") return "video/x-flv";
    if (ext == ".png") return "image/png";
    if (ext == ".jpeg" || ext == ".jpg" || ext == ".jpe") return "image/jpeg";
    if (ext == ".gif") return "image/gif";
    if (ext == ".bmp") return "image/bmp";
    if (ext == ".ico") return "image/vnd.microsoft.icon";
    if (ext == ".tiff" || ext == ".tif") return "image/tiff";
    if (ext == ".svg" || ext == ".svgz") return "image/svg+xml";
    return "application/text";
}
