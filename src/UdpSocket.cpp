#include "snet/UdpSocket.hpp"
#include "snet/Address.hpp"
#include "snet/PacketHandler.hpp"

#include <netdb.h>
#include <netinet/in.h>
#include <optional>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <sys/socket.h>
#include <system_error>
#include <unistd.h>
#include <fcntl.h>
#include <vector>

namespace snet {

UdpSocket::UdpSocket() {
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    throw std::system_error(errno, std::generic_category(), "failed to create udp socket");
  }

  makeNonBlocking();
}

UdpSocket::~UdpSocket() {
  close();
}

void UdpSocket::close() {
  if (sockfd >= 0) {
    ::close(sockfd);
  }
}

void UdpSocket::bind(Address& addr) {
  auto b = ::bind(sockfd, reinterpret_cast<sockaddr*>(&addr.getAddress()), sizeof(sockaddr));
  if (b != 0) {
    close();
    throw std::system_error(errno, std::generic_category(), "failed to bind socket");
  }
}

ssize_t UdpSocket::send(const Packet& packet, Address& addr) {
  auto data = PacketHandler::getSerializedPacket(packet);
  auto bytes_sent = sendto(sockfd, data.data(), data.size(), 0, reinterpret_cast<sockaddr*>(&addr.getAddress()), addr.getAddressLen());
  return bytes_sent;
}

std::optional<Packet> UdpSocket::receive() {
  sockaddr_in sender_addr{};
  socklen_t sender_len = sizeof(sender_addr);
  std::vector<std::byte> receive_buffer(MAX_PACKET_SIZE);

  Packet packet;

  auto bytes_received = recvfrom(sockfd, receive_buffer.data(), receive_buffer.size(), 0, reinterpret_cast<struct sockaddr*>(&sender_addr), &sender_len);
  if (bytes_received < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      return std::nullopt;
    }

    throw std::system_error(errno, std::generic_category(), "failed to receive udp packet");
  }

  packet = PacketHandler::getDeserializedPacket(receive_buffer);
  return packet;
}


void UdpSocket::makeNonBlocking() {
  int flags = fcntl(sockfd, F_GETFL, 0);
  if (flags == -1) {
    close();
    throw std::system_error(errno, std::generic_category(), "failed to get socket flags");
  }

  flags |= O_NONBLOCK;

  if (fcntl(sockfd, F_SETFL, flags) == -1) {
    close();
    throw std::system_error(errno, std::generic_category(), "failed to set socket non-blocking");
  }
}

}