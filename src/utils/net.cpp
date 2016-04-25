#include "net.h"

Net::Net(){

}

void Net::setIpPortSend(string ip, int port){
	ipSend = new QHostAddress(QString::fromStdString(ip));
	portSend = port;
}

void Net::setIpPortReceive(string ip, int port){
	ipReceive = new QHostAddress(QString::fromStdString(ip));
	portReceive = port;

	udpsocketReceive.bind(QHostAddress("0.0.0.0"), portReceive, QUdpSocket::ShareAddress);
    udpsocketReceive.joinMulticastGroup(*ipReceive);
}

void Net::sendPacket(grSim_Packet packet){
    datagramSend.resize(packet.ByteSize());
    packet.SerializeToArray(datagramSend.data(), datagramSend.size());
    udpsocketSend.writeDatagram(datagramSend, *ipSend, portSend);
}

SSL_WrapperPacket Net::receivePacket(){
	SSL_WrapperPacket packet;

    datagramReceive.resize(udpsocketReceive.pendingDatagramSize());
    udpsocketReceive.readDatagram(datagramReceive.data(), datagramReceive.size());
    packet.ParseFromArray(datagramReceive.data(), datagramReceive.size());

    SSL_DetectionRobot robot;

    robot = packet.detection().robots_blue(0);
    cout << robot.orientation() << endl;

    // debug all
    /*string text_str;
    google::protobuf::TextFormat::PrintToString(packet, &text_str);
    cout << text_str << endl;*/

    return packet;
}

bool Net::hasNewPacket(){
	return udpsocketReceive.hasPendingDatagrams();
}