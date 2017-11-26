#!/bin/sh

export INCLUDEDIRS="\$(SolutionDir)/gtest/googletest/include;\$(SolutionDir)/gtest/googletest/"

source toolchain/generate-vcxproj.sh

write_command "_GENERATE PROJECTS" {29C53322-7C20-431A-93B3-42D84826FE11} '"c:\Program Files\git\usr\bin\sh.exe"' --login '$(SolutionDir)\generate-vcxproj.sh'

write_lib protobuf protobuf {01EF6A74-7AF7-41F2-BBF8-6FDE04AFFA0B} .
write_lib protorpc protorpc {5EB12FC8-F54C-4257-829B-41916333921A} .
write_exe cmd/protoc-gen-protorpc protoc-gen-protorpc {C676AF48-ED5F-4089-B356-9A26D3521FF2} .
write_exe . protorpc_test {730E5A44-48DC-4F59-B34D-2D2F4869E5A0}

write_default "_DEFAULT" {1E69E38F-001D-4219-9835-20D636CE1927}
write_solution protorpc.sln