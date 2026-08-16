#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace snet {

class PacketHandler {

public:

  /*
    Structs
  */

  static constexpr std::size_t HEADER_SIZE = sizeof(uint32_t) + sizeof(uint64_t);

  struct Header {
    uint32_t sender_ip;
    uint64_t sent_time;
  };

  struct Packet {
    Header header;
    std::vector<std::byte> data;
  };

  /*
    Handling packets
  */

  static std::vector<std::byte> getSerializedPacket(const Packet& packet);
  static Packet getDeserializedPacket(std::vector<std::byte> data);

  /*
    Writing
  */

  static void write_uint8(Packet& packet, uint8_t data);
  static void write_uint16(Packet& packet, uint16_t data);
  static void write_uint32(Packet& packet, uint32_t data);
  static void write_uint64(Packet& packet, uint64_t data);

  static void write_int8(Packet& packet, int8_t data);
  static void write_int16(Packet& packet, int16_t data);
  static void write_int32(Packet& packet, int32_t data);
  static void write_int64(Packet& packet, int64_t data);

  static void write_float(Packet& packet, float data);
  static void write_double(Packet& packet, double data);
  static void write_bool(Packet& packet, bool data);
  static void write_string(Packet& packet, const std::string& data);

  /*
    Reading
  */

  static uint8_t read_uint8(const Packet& packet, std::size_t& index);
  static uint16_t read_uint16(const Packet& packet, std::size_t& index);
  static uint32_t read_uint32(const Packet& packet, std::size_t& index);
  static uint64_t read_uint64(const Packet& packet, std::size_t& index);

  static int8_t read_int8(const Packet& packet, std::size_t& index);
  static int16_t read_int16(const Packet& packet, std::size_t& index);
  static int32_t read_int32(const Packet& packet, std::size_t& index);
  static int64_t read_int64(const Packet& packet, std::size_t& index);

  static float read_float(const Packet& packet, std::size_t& index);
  static double read_double(const Packet& packet, std::size_t& index);
  static bool read_bool(const Packet& packet, std::size_t& index);
  static std::string read_string(const Packet& packet, std::size_t& index);

private:
  static std::vector<std::byte> serializeHeader(const Header& header);
  static Header deserializeHeader(const std::vector<std::byte> data);

};

using Packet = PacketHandler::Packet;
using Header = PacketHandler::Header;

}