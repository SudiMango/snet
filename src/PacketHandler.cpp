#include "snet/PacketHandler.hpp"
#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <endian.h>
#include <netinet/in.h>
#include <vector>

namespace snet {

std::vector<std::byte> PacketHandler::getSerializedPacket(const Packet& packet) {
  std::vector<std::byte> serialized;
  std::vector<std::byte> s_header = serializeHeader(packet.header);
  serialized.insert(serialized.end(), s_header.begin(), s_header.end());
  serialized.insert(serialized.end(), packet.data.begin(), packet.data.end());
  return serialized;
}

Packet PacketHandler::getDeserializedPacket(std::vector<std::byte> data) {
  Packet p{};
  p.header = deserializeHeader(data);
  p.data.assign(data.begin() + HEADER_SIZE, data.end());
  return p;
}

std::vector<std::byte>
PacketHandler::serializeHeader(const Header& header) {
  std::vector<std::byte> serialized;
  serialized.reserve(HEADER_SIZE);

  const uint32_t sender_ip = htonl(header.sender_ip);
  const uint64_t sent_time = htobe64(header.sent_time);

  const auto sender_ip_bytes =
      std::bit_cast<std::array<std::byte, sizeof(sender_ip)>>(sender_ip);
  const auto sent_time_bytes =
      std::bit_cast<std::array<std::byte, sizeof(sent_time)>>(sent_time);

  serialized.insert(
      serialized.end(),
      sender_ip_bytes.begin(),
      sender_ip_bytes.end()
  );

  serialized.insert(
      serialized.end(),
      sent_time_bytes.begin(),
      sent_time_bytes.end()
  );

  return serialized;
}

Header PacketHandler::deserializeHeader(const std::vector<std::byte> data) {

  uint32_t sender_ip;
  uint64_t sent_time;

  std::memcpy(
      &sender_ip,
      data.data(),
      sizeof(sender_ip)
  );

  std::memcpy(
      &sent_time,
      data.data() + sizeof(sender_ip),
      sizeof(sent_time)
  );

  return Header{
      .sender_ip = ntohl(sender_ip),
      .sent_time = be64toh(sent_time),
  };
}

/*
  Writing
*/

// uint

void PacketHandler::write_uint8(Packet& packet, uint8_t data) {
  auto n_data = std::bit_cast<std::byte>(data);
  packet.data.push_back(n_data);
}

void PacketHandler::write_uint16(Packet& packet, uint16_t data) {
  auto n_data = htons(data);
  auto bytes = std::bit_cast<std::array<std::byte, 2>>(n_data);
  packet.data.insert(packet.data.end(), bytes.begin(), bytes.end());
}

void PacketHandler::write_uint32(Packet& packet, uint32_t data) {
  auto n_data = htonl(data);
  auto bytes = std::bit_cast<std::array<std::byte, 4>>(n_data);
  packet.data.insert(packet.data.end(), bytes.begin(), bytes.end());
}

void PacketHandler::write_uint64(Packet& packet, uint64_t data) {
  auto n_data = htobe64(data);
  auto bytes = std::bit_cast<std::array<std::byte, 8>>(n_data);
  packet.data.insert(packet.data.end(), bytes.begin(), bytes.end());
}


// int


void PacketHandler::write_int8(Packet& packet, int8_t data) {
  uint8_t u = std::bit_cast<uint8_t>(data);
  write_uint8(packet, u);
}

void PacketHandler::write_int16(Packet& packet, int16_t data) {
  uint16_t u = std::bit_cast<uint16_t>(data);
  write_uint16(packet, u);
}

void PacketHandler::write_int32(Packet& packet, int32_t data) {
  uint32_t u = std::bit_cast<uint32_t>(data);
  write_uint32(packet, u);
}

void PacketHandler::write_int64(Packet& packet, int64_t data) {
  uint64_t u = std::bit_cast<uint64_t>(data);
  write_uint64(packet, u);
}


// other complex ones


void PacketHandler::write_float(Packet& packet, float data){
  auto n_data = std::bit_cast<uint32_t>(data);
  write_uint32(packet, n_data);
}

void PacketHandler::write_double(Packet& packet, double data) {
  auto n_data = std::bit_cast<uint64_t>(data);
  write_uint64(packet, n_data);
}

void PacketHandler::write_bool(Packet& packet, bool data) {
  uint8_t n_data = data ? 1 : 0;
  write_uint8(packet, n_data);
}

void PacketHandler::write_string(Packet& packet, const std::string& data) {
  uint16_t len = static_cast<uint16_t>(data.length());
  write_uint16(packet, len);

  std::vector<std::byte> bytes(len);
  std::transform(data.begin(), data.end(), bytes.begin(), [](char c) {
    return std::bit_cast<std::byte>(c);
  });

  packet.data.insert(packet.data.end(), bytes.begin(), bytes.end());
}

/*
  Reading
*/

// uint

uint8_t PacketHandler::read_uint8(const Packet& packet, std::size_t& index) {
  uint8_t result = static_cast<uint8_t>(packet.data.at(index));
  index += 1;
  return result;
}

uint16_t PacketHandler::read_uint16(const Packet& packet, std::size_t& index) {
  uint16_t result;
  std::memcpy(&result, packet.data.data() + index, sizeof(uint16_t));
  index += 2;
  return ntohs(result);
}

uint32_t PacketHandler::read_uint32(const Packet& packet, std::size_t& index) {
  uint32_t result;
  std::memcpy(&result, packet.data.data() + index, sizeof(uint32_t));
  index += 4;
  return ntohl(result);
}

uint64_t PacketHandler::read_uint64(const Packet& packet, std::size_t& index) {
  uint64_t result;
  std::memcpy(&result, packet.data.data() + index, sizeof(uint64_t));
  index += 8;
  return be64toh(result);
}


// int


int8_t PacketHandler::read_int8(const Packet& packet, std::size_t& index) {
  uint8_t n_data = read_uint8(packet, index);
  int8_t result = std::bit_cast<int8_t>(n_data);
  return result;
}

int16_t PacketHandler::read_int16(const Packet& packet, std::size_t& index) {
  uint16_t n_data = read_uint16(packet, index);
  int16_t result = std::bit_cast<int16_t>(n_data);
  return result;
}

int32_t PacketHandler::read_int32(const Packet& packet, std::size_t& index) {
  uint32_t n_data = read_uint32(packet, index);
  int32_t result = std::bit_cast<int32_t>(n_data);
  return result;
}

int64_t PacketHandler::read_int64(const Packet& packet, std::size_t& index) {
  uint64_t n_data = read_uint64(packet, index);
  int64_t result = std::bit_cast<int64_t>(n_data);
  return result;
}


// other complex ones


float PacketHandler::read_float(const Packet& packet, std::size_t& index) {
  uint32_t n_data = read_uint32(packet, index);
  float result = std::bit_cast<float>(n_data);
  return result;
}

double PacketHandler::read_double(const Packet& packet, std::size_t& index) {
  uint64_t n_data = read_uint64(packet, index);
  double result = std::bit_cast<double>(n_data);
  return result;
}

bool PacketHandler::read_bool(const Packet& packet, std::size_t& index) {
  uint8_t n_data = read_uint8(packet, index);
  return n_data == 1 ? true : false;
}

std::string PacketHandler::read_string(const Packet& packet, std::size_t& index) {
  uint16_t len = read_uint16(packet, index);
  std::string result;
  result.reserve(len);

  while (len > 0) {
    uint8_t letter_byte = read_uint8(packet, index);
    char letter = std::bit_cast<char>(letter_byte);
    result.push_back(letter);
    len--;
  }

  return result;
}

}