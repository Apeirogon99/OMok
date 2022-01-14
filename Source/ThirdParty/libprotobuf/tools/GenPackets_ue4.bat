pushd %~dp0
generate_for_ue4.py --proto_input ../../../../Server/Server/ProtocolMake/Debug/Enum.proto --cpp_out ../../../OMok/ProtoFiles
generate_for_ue4.py --proto_input ../../../../Server/Server/ProtocolMake/Debug/Struct.proto --cpp_out ../../../OMok/ProtoFiles
generate_for_ue4.py --proto_input ../../../../Server/Server/ProtocolMake/Debug/Protocol.proto --cpp_out ../../../OMok/ProtoFiles
IF ERRORLEVEL 1 PAUSE