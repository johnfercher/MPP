all:	
	@echo Protos ...

	@protoc -I=. --cpp_out=. grSim_Commands.proto
	@protoc -I=. --cpp_out=. grSim_Replacement.proto
	@protoc -I=. --cpp_out=. grSim_Packet.proto
	@protoc -I=. --cpp_out=. messages_robocup_ssl_detection.proto
	@protoc -I=. --cpp_out=. messages_robocup_ssl_geometry.proto
	@protoc -I=. --cpp_out=. messages_robocup_ssl_refbox_log.proto
	@protoc -I=. --cpp_out=. messages_robocup_ssl_wrapper.proto
	@protoc -I=. --cpp_out=. referee.proto
