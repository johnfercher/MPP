#ifndef _NET_H_
#define _NET_H_

// Default
#include "iostream"
#include "unistd.h"

// SSL-Vision
#include "messages_robocup_ssl_detection.pb.h"
#include "messages_robocup_ssl_geometry.pb.h"
#include "messages_robocup_ssl_refbox_log.pb.h"
#include "messages_robocup_ssl_wrapper.pb.h"
#include "referee.pb.h"

// grSim
#include "grSim_Packet.pb.h"
#include "grSim_Commands.pb.h"
#include "grSim_Replacement.pb.h"

// Protobuf
#include <google/protobuf/text_format.h>

// Qt
#include "qt.h"

using namespace std;	// to use string

class Net{
private:
	// send commands to grSim
	QUdpSocket udpsocketSend;
    quint16 portSend;
    QHostAddress *ipSend;
    QByteArray datagramSend;

    // receive commands from grSim
    QUdpSocket udpsocketReceive;
    quint16 portReceive;
    QHostAddress *ipReceive;
    QByteArray datagramReceive;

public:
	Net();

	void setIpPortSend(string ip = "127.0.0.1", int port = 20011);
	void setIpPortReceive(string ip = "224.5.23.2", int port = 10002);

	void sendPacket(grSim_Packet);
	SSL_WrapperPacket receivePacket();
	bool hasNewPacket();
};

#endif // _NET_H_