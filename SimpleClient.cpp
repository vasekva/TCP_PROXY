#include <iostream>
#include "olc_net.hpp"

enum class CustomMsgTypes : uint32_t
{
	FireBullet,
	MovePlayer
};

class CustomClient : public olc::net::client_interface<CustomMsgTypes>
{

};

int main()
{
	olc::net::message<CustomMsgTypes> msg;
	msg.header.id = CustomMsgTypes::FireBullet;

	int a = 1;

	return (0);
}