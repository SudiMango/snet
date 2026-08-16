#pragma once

#include "snet/Address.hpp"
#include "snet/PacketHandler.hpp"
#include <cstddef>
#include <optional>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>

namespace snet {

class UdpSocket {

public:
  static constexpr std::size_t MAX_PACKET_SIZE = 1200;

  UdpSocket();
  ~UdpSocket();

  void close();
  void bind(Address& addr);
  ssize_t send(const Packet& packet, Address& addr);
  std::optional<Packet> receive();

private:
  int sockfd;

  void makeNonBlocking();

};

}