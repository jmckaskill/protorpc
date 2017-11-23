#!/bin/sh

export PATH=$PATH:/c/c/protoc/bin/:../bin/Debug

protoc --protorpc_out=. protocol.proto &>err.txt || exit 1
