#include "snet/Address.hpp"
#include "snet/UdpSocket.hpp"
#include "NetworkMessage.hpp"
#include <iostream>
#include <string>

using namespace snet;

int main() {
  Address server_addr("", 9090, true);

  UdpSocket socket;
  socket.bind(server_addr);

  while (true) {
    if (auto packet = socket.receive()) {
      NetworkMessage m;
      m.deserialize(*packet);

      std::cout << "Sender IP: " << packet->header.sender_ip << '\n'
                << "Timestamp: " << packet->header.sent_time << '\n'
                << "message: " << m.message << '\n'
                << "big_number: " << std::to_string(m.big_number) << '\n'
                << "yes: " << std::to_string(m.yes) << '\n'
                << "bruh: " << std::to_string(m.bruh) << std::endl;
    }
  }

}