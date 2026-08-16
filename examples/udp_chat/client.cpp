#include "NetworkMessage.hpp"
#include "snet/Address.hpp"
#include "snet/PacketHandler.hpp"
#include "snet/UdpSocket.hpp"
#include <string>

using namespace snet;

int main() {
  Address server_addr("127.0.0.1", 9090);
  UdpSocket socket;

  Packet p{};

  NetworkMessage m{};
  m.message = "hi from client";
  m.big_number = 1234;
  m.yes = false;
  m.bruh = 3.1415;
  m.serialize(p);

  socket.send(p, server_addr);

}