# Server creation description

TODO: Make /var/www catalog available for user, not for root

# User configuration
## Add user
```bash
adduser user
```

## Add user to sudo group
```bash
usermod -aG sudo user
```

## List of available firewall rules
```bash
ufw app List
```

## Allow OpenSsh
```bash
ufw allow OpenSSH
```

## Enable ufw
```bash
ufw enable
```

## Check ufw
```bash
ufw status
```

##  Connect via ssh user
```bash
ssh user@<ip>
```

# Nginx
## Install nginx
```bash
sudo apt update && sudo apt upgrade 
sudo apt install -y nginx
sudo reboot
```

## Configure firewall for nginx
```bash
sudo ufw app list
sudo ufw allow 'Nginx Full'
```

## Check nginx status
```bash
systemctl status nginx
```

## Quickly apply nginx config without restart
```bash
sudo nginx -t # Check rules
sudo systemctl reload nginx
```

## Restart nginx 
```bash
sudo systemctl restart nginx
```

# Send frontend to server
## Exec at build folder
```bash
scp -C qtloader.js qtlogo.svg TermGraph.html TermGraph.js TermGraph.wasm TermGraph.worker.js root@termgraph.app:/var/www/html
```

# Frontend configuration
## Nginx config example at /etc/nginx/sites-enabled/default
```nginx
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
```

# Send backend to server
## Exec at build folder
```bash
scp -C qt-unified-linux-x64-4.4.1-online.run root@termgraph.app:/home/user
scp -C TermGraphBack user@termgraph.app:/home/user
```

## To start server you need Qt:
## Downloading the online installer
```bash
wget https://download.qt.io/official_releases/online_installers/qt-unified-linux-x64-online.run
chmod +x qt-unified-linux-x64-online.run
```

## Copy qt account credentials (in a bad way), because they broken in cli 
Awful ux, honestly
```bash
scp ~/.local/share/Qt/qtaccount.ini root@termgraph.app:/home/user/.local/share/Qt/qtaccount.ini
```

## Start online qt installer with specific package
```bash
./qt-unified-linux-x64-online.run --al --ao --da in "qt.qt6.631.gcc_64" 
```

# Backend configuration
## Systemd service file creation
```bash
export BIN_PATH=/home/user/TermGraphBack
chmod +x "${BIN_PATH}"
export SVC_NAME="term-back"
```

```bash
echo "
[Unit]
Description=${SVC_NAME}
[Service]
ExecStart=${BIN_PATH}
Restart=always
RestartSec=5
[Install]
" | tee /etc/systemd/system/${SVC_NAME}.service
```

```bash
systemctl daemon-reload
```

## Start the process right now and enable it to run on reboot.
```bash
systemctl enable --now "${SVC_NAME}"
systemctl status --no-pager "${SVC_NAME}"
journalctl -fu "${SVC_NAME}"

journalctl -u "${SVC_NAME}"
```


# Certbot section

```
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
```