#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/program_options.hpp>

#include <functional>
#include <iostream>
#include <string>

#include "processor.h"

namespace net = boost::asio;
using tcp = net::ip::tcp;
namespace beast = boost::beast;
namespace ws = beast::websocket;
namespace po = boost::program_options;

void fail(beast::error_code ec, char const* what) {
  std::cerr << what << ": " << ec.message() << '\n';
}

void process(ws::stream<beast::tcp_stream>& wsock, net::yield_context yield) {
  std::cout << "process\n";
  beast::error_code ec;
  Processor p;
  for(;;)
  {
    beast::flat_buffer ibuf;
    wsock.async_read(ibuf, yield[ec]);
    if (ec == ws::error::closed) break;
    if (ec) return fail(ec, "read");
    auto request = beast::buffers_to_string(ibuf.data());
    std::cout << "request=" << request << '\n';
    auto response = p.serve(request);
    std::cout << "response=" << response << '\n';
    wsock.text(wsock.got_text());
    wsock.async_write(net::buffer(response), yield[ec]);
    if (ec) return fail(ec, "write");
  }
}

void wait(net::io_context& ioc,
          tcp::endpoint const& endpoint,
          net::yield_context yield) {
  std::cout << "wait\n";
  tcp::acceptor acceptor{ioc, endpoint};
  for(;;) {
    beast::error_code ec;
    ws::stream<beast::tcp_stream> wsock{ioc};
    acceptor.async_accept(beast::get_lowest_layer(wsock).socket(), yield[ec]);
    if (ec) return fail(ec, "accept tcp");
    wsock.async_accept(yield[ec]);
    if (ec) return fail(ec, "accept ws");
    net::spawn(acceptor.get_executor(),
        std::bind(&process, std::move(wsock), std::placeholders::_1));
  }
}

int main(int argc, char* argv[]) {
  unsigned short port{8080};

  po::options_description args("Using");
  args.add_options()
    ("help", "produce help message")
    ("port", po::value<unsigned short>(&port)->default_value(8080), "port to listen");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, args), vm);
  po::notify(vm);

  if (vm.count("help")) {
      std::cout << args << "\n";
      return 1;
  }

  net::io_context ioc;
  tcp::endpoint point{tcp::v6(), port};
  net::spawn(ioc, [&ioc, point](auto yield) {
    wait(ioc, point, yield);
  });
  ioc.run();

  return 0;
}
