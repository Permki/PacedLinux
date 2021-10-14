#!/usr/bin/env bash

echo "sudo apt-get --purge remove wine-stable"
sudo apt-get --purge remove wine-stable

echo "sudo apt-get --purge remove wine-development"
sudo apt-get --purge remove wine-development

echo "sudo apt install wine-stable "
sudo apt install wine-stable
