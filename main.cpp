#include <boost/beast.hpp>

#include <iostream>

namespace net = boost::asio;

int main() {
  std::cout << "spfservice run\n";

  net::io_context ioc;
  boost::beast::multi_buffer b;

  return 0;
}
