#include "snet/Address.hpp"
#include <cstdint>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <format>
#include <stdexcept>

namespace snet {

Address::Address(std::string domain, uint16_t port, bool passive) {
  resolve(domain, std::to_string(port), passive);
}

sockaddr_in& Address::getAddress() {
  return addr;
}

socklen_t& Address::getAddressLen() {
  return addr_len;
}

void Address::resolve(std::string domain, std::string port, bool passive) {
  addrinfo hints{};
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  if (passive) {
    hints.ai_flags = AI_PASSIVE;
  }

  const char* host = domain.empty() ? nullptr : domain.c_str();
  addrinfo *res = nullptr;
  int addr_status = getaddrinfo(host, port.c_str(), &hints, &res);
  if (addr_status != 0) {
    throw std::runtime_error(std::format("getaddrinfo: {}", gai_strerror(addr_status)));
  }

  addr = *reinterpret_cast<sockaddr_in*>(res->ai_addr);
  addr_len = res->ai_addrlen;
  freeaddrinfo(res);
}

}