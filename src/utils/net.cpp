/*The MIT License (MIT)

Copyright (c) 2016 Johnathan Fercher

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
*/

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