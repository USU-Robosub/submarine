### Argument Details

```
./setup.sh -[code] [parameter] -[code] [parameter]
```

| Code | Parameters | Action | Packages Installed | Time |
| ---- | ---------- | ------ | ------------------ | ---- |
| r | | Clones the repository into the directory `submarine` | | Fast |
| c | | Installs dependencies of the C++ code | cmake | Fast |
| j | | Installs Node.js JavaScript runtime | nvm, node | Fast |
| a | | Installs PlatformIO embedded system compiler | python, python-pip, platformio | Fast |
| v | pi, amd | Installs dependencies for vision code. `pi` will compile OpenCV optimized for the Raspberry PI. `amd` will compile OpenCV for laptops or desktops | streameye, opencv (+dependencies) | Slow |
| p |  | Installs PM2 process manager, and sets it to run on boot | pm2 | Fast |
| 9 |  | Installs Cloud9 IDE for remote editing | cloud9 | Slow |
| h |  | Installs HAProxy to cleanup Cloud9 and the submarine's webApp urls | haproxy | Fast |
