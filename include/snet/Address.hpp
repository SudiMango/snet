#pragma once

#include <cstdint>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>

namespace snet {

class Address {

public:
  Address(std::string domain, uint16_t port, bool passive = false);

  sockaddr_in& getAddress();
  socklen_t& getAddressLen();

private:
  sockaddr_in addr{};
  socklen_t addr_len{};

  void resolve(std::string domain, std::string port, bool passive = false);

};

}