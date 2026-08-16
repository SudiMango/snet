#pragma once

#include "snet/PacketHandler.hpp"

#include <cstddef>
#include <cstdint>
#include <string>

struct NetworkMessage {
  std::string message{};
  std::uint64_t big_number{};
  bool yes{};
  float bruh{};

  void serialize(snet::Packet& packet) const {
    snet::PacketHandler::write_string(packet, message);
    snet::PacketHandler::write_uint64(packet, big_number);
    snet::PacketHandler::write_bool(packet, yes);
    snet::PacketHandler::write_float(packet, bruh);
  }

  void deserialize(const snet::Packet& packet) {
    std::size_t index = 0;

    message = snet::PacketHandler::read_string(packet, index);
    big_number = snet::PacketHandler::read_uint64(packet, index);
    yes = snet::PacketHandler::read_bool(packet, index);
    bruh = snet::PacketHandler::read_float(packet, index);
  }
};