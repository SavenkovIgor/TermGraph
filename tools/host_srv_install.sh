#!/bin/bash

# This file is more reference, than exact script. 
# Use as copy/paste reference
# TODO: Make /var/www catalog available for user, not for root

# 1. Add user
adduser user

# 2. Add user to sudo group
usermod -aG sudo user

# 3. List of available firewall rules
ufw app List

# 4. Allow OpenSsh
ufw allow OpenSSH

# 5. Enable ufw
ufw enable

# 6. Check ufw
ufw status

# 7. Connect via ssh user
ssh user@<ip>

# 8. Install nginx
sudo apt update && sudo apt upgrade 
sudo apt install -y nginx
sudo reboot

# 9. Configure firewall for nginx
sudo ufw app list
sudo ufw allow 'Nginx Full'

# 10. Check nginx status
systemctl status nginx

# Quickly apply nginx config without restart
sudo nginx -t # Check rules
sudo systemctl reload nginx

# Restart nginx 
sudo systemctl restart nginx


# Send frontend to server
# Exec at build folder
scp -C qtloader.js qtlogo.svg TermGraph.html TermGraph.js TermGraph.wasm TermGraph.worker.js root@termgraph.app:/var/www/html


# Frontend configuration
# Nginx config example at /etc/nginx/sites-enabled/default
server {
    root /var/www/html;

    server_name termgraph.app www.termgraph.app;

    location / {
        # First attempt to serve request as file, then
        # as directory, then fall back to displaying a 404.
        try_files $uri $uri/TermGraph.html =404;
    }

    location /api/ {
        proxy_pass http://localhost:26748;
    }

    add_header 'Access-Control-Allow-Origin'  '*';
    add_header 'Cross-Origin-Embedder-Policy' 'require-corp';
    add_header 'Cross-Origin-Opener-Policy'   'same-origin';

    # SSL configuration
    listen [::]:443 ssl ipv6only=on; # managed by Certbot
    listen 443 ssl; # managed by Certbot
    ssl_certificate /etc/letsencrypt/live/termgraph.app/fullchain.pem; # managed by Certbot
    ssl_certificate_key /etc/letsencrypt/live/termgraph.app/privkey.pem; # managed by Certbot
    include /etc/letsencrypt/options-ssl-nginx.conf; # managed by Certbot
    ssl_dhparam /etc/letsencrypt/ssl-dhparams.pem; # managed by Certbot
}

server {
    if ($host = www.termgraph.app) {
        return 301 https://$host$request_uri;
    } # managed by Certbot


    if ($host = termgraph.app) {
        return 301 https://$host$request_uri;
    } # managed by Certbot

    listen 80 default_server;
    listen [::]:80 default_server;

    server_name termgraph.app www.termgraph.app;

    return 404; # managed by Certbot
}


# Send backend to server
# Exec at build folder
scp -C qt-unified-linux-x64-4.4.1-online.run root@termgraph.app:/home/user
scp -C TermGraphBack user@termgraph.app:/home/user


# Backend configuration
# Systemd service file creation
export BIN_PATH=/home/user/TermGraphBack
chmod +x "${BIN_PATH}"
# e.g. export SVC_NAME="my-app"
export SVC_NAME="term-back"


echo "
[Unit]
Description=${SVC_NAME}
[Service]
ExecStart=${BIN_PATH}
Restart=always
RestartSec=5
[Install]
" | tee /etc/systemd/system/${SVC_NAME}.service

systemctl daemon-reload

# Start the process right now and enable it to run on reboot.
systemctl enable --now "${SVC_NAME}"
systemctl status --no-pager "${SVC_NAME}"
journalctl -fu "${SVC_NAME}"

journalctl -u "${SVC_NAME}"



# Certbot section

# Start certbot first time
sudo certbot --nginx -d termgraph.app -d www.termgraph.app

# Saving debug log to /var/log/letsencrypt/letsencrypt.log

# Successfully received certificate.
# Certificate is saved at: /etc/letsencrypt/live/termgraph.app/fullchain.pem
# Key is saved at:         /etc/letsencrypt/live/termgraph.app/privkey.pem
# This certificate expires on 2022-12-01.
# These files will be updated when the certificate renews.
# Certbot has set up a scheduled task to automatically renew this certificate in the background.

# Deploying certificate
# Successfully deployed certificate for termgraph.app to /etc/nginx/sites-enabled/default
# Successfully deployed certificate for www.termgraph.app to /etc/nginx/sites-enabled/default
# Congratulations! You have successfully enabled HTTPS on https://termgraph.app and https://www.termgraph.app
