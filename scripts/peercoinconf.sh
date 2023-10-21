#!/bin/bash -ev

mkdir -p ~/.beastbucks
echo "rpcuser=username" >>~/.beastbucks/beastbucks.conf
echo "rpcpassword=`head -c 32 /dev/urandom | base64`" >>~/.beastbucks/beastbucks.conf

