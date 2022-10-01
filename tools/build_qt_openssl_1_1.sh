# Build of qt openssl
# 0. Install qt openssl version with maintenance tool

# 1. Create folder
sudo mkdir /usr/local/ssl

# 2. Configure of qt opennssl at Qt/Tools/OpenSSL/src
sudo ./config --prefix=/usr/local/ssl --openssldir=/usr/local/ssl shared zlib

# 3. Build
sudo make

# 4. Build of tests
sudo make test

# 5. Install
sudo make install

# 6. Add path to build environment in QtCreator
LD_LIBRARY_PATH=/usr/local/ssl/lib