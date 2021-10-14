#!/usr/bin/env bash

echo "sudo dpkg --add-architecture i386 "
sudo dpkg --add-architecture i386 

echo "wget -nc https://dl.winehq.org/wine-builds/winehq.key"
wget -nc https://dl.winehq.org/wine-builds/winehq.key

echo "sudo apt-key add winehq.key"
sudo apt-key add winehq.key

echo "sudo add-apt-repository 'deb https://dl.winehq.org/wine-builds/ubuntu/ focal main'"
sudo add-apt-repository 'deb https://dl.winehq.org/wine-builds/ubuntu/ focal main'

echo "sudo apt update"
sudo apt update

echo "sudo apt install --install-recommends winehq-stable"
sudo apt install --install-recommends winehq-stable