#!/bin/bash

# This file is more reference, than exact script. 
# Use as copy/paste reference

# Add user
adduser user

# Add user to sudo group
usermod -aG sudo user

# List of available firewall rules
ufw app List

# Allow OpenSsh
ufw allow OpenSSH

# Enable ufw
ufw enable

# Check ufw
ufw status

# Connect via ssh user
ssh user@<ip>

# Install basic tools
sudo apt update
sudo apt upgrade 
# apt install -y git vim # Most probably, already installed

sudo apt install -y nginx

sudo reboot

# Configure firewall for nginx
sudo ufw app list
sudo ufw allow 'Nginx Full'

# Check nginx status
systemctl status nginx

# For quick config apply
sudo systemctl reload nginx

# For restart
sudo nginx -t # Check rules
sudo systemctl restart nginx

# Send frontend to server
tar --create --file wasm.tar qtloader.js qtlogo.svg TermGraph.html TermGraph.js TermGraph.wasm TermGraph.worker.js
scp ./wasm.tar root@146.190.31.160:/home/user

# On server
cd /var/www/html
sudo tar -xvf /home/user/wasm.tar