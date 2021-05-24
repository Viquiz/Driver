#ifndef _PACKET_
#define _PACKET_
#include <Arduino.h>
// ----------------------------------------
// This file defines the protocol to communicate between host (ESP-32) and client (ESP-8266)
// ----------------------------------------
enum packet_t : uint8_t
{
    BEACON = 0,
    RESPOND_BEACON,
    REQ_BEACON,
    RECV_ANSW,
    RESPOND_ANSW,
};

// Act as a header
struct BasePacket
{
    const packet_t type;
    BasePacket(packet_t type) : type(type) {}
};

// Unicast/Broadcast host address with password
struct BeaconPacket : BasePacket
{
    uint32_t password;
    BeaconPacket() : BasePacket(packet_t::BEACON) {}
    BeaconPacket(int password) : BeaconPacket() { this->password = password; }
};

// When client receive a BeaconPacket, it will respond a RespondBeaconPacket to the server
struct RespondBeaconPacket : BasePacket
{
    RespondBeaconPacket() : BasePacket(packet_t::RESPOND_BEACON) {}
};

// If for some reason the BeaconPacket can't reach the client,
// client can broadcast this to actively receive the server address
struct RequestBeaconPacket : BasePacket
{
    int password;
    RequestBeaconPacket() : BasePacket(packet_t::REQ_BEACON) {}
    RequestBeaconPacket(int password) : RequestBeaconPacket() { this->password = password; }
};

// Host receive this from client(s)
struct BtnPacket : BasePacket
{
    uint8_t button;
    BtnPacket() : BasePacket(packet_t::RECV_ANSW) {}
    BtnPacket(uint8_t button) : BtnPacket() { this->button = button; }
};

// Send correct answer to client(s)
struct AnswPacket : BasePacket
{
    uint8_t answer;
    AnswPacket() : BasePacket(packet_t::RESPOND_ANSW) {}
    AnswPacket(uint8_t answer) : AnswPacket() { this->answer = answer; }
};
#endif // !_PACKET_