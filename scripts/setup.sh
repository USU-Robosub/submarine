#!/bin/bash
set -e

while getopts "rcjav:p9" option; do
  case $option in
    r )
        clone_repo=true
    ;;
    c )
        install_cpp=true
    ;;
    j )
        install_javascript=true
    ;;
    a )
        install_arduino=true
    ;;
    v )
        install_vision=true
        vision_platform=$OPTARG
    ;;
    p )
        install_pm2=true
    ;;
    9 )
        install_cloud9=true
    ;;
  esac
done

# update package lists
sudo apt-get update

if [[ $clone_repo && $clone_repo == true ]]; then
    if [[ ! $(dpkg -s git) ]]; then
        # install git to clone repo
        sudo apt-get install git -y
    fi

    # clone repo into submarine
    git clone https://github.com/USU-Robosub/submarine.git
fi

if [[ $install_cpp && $install_cpp == true ]]; then
    if [[ ! $(dpkg -s cmake) ]]; then
        # install cmake to build c++ code
        sudo apt-get install cmake -y
    fi
fi

if [[ $install_javascript && $install_javascript == true ]]; then
    # install nodejs version manager
    if [ ! -d "$HOME/.nvm" ]
    then
        wget -qO- https://raw.githubusercontent.com/creationix/nvm/v0.33.11/install.sh | bash
        export NVM_DIR="$HOME/.nvm"
        [ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh" # This loads nvm
        [ -s "$NVM_DIR/bash_completion" ] && \. "$NVM_DIR/bash_completion" # This loads nvm bash_completion

        # import nvm function into this script
        . $HOME/.nvm/nvm.sh

        # install nodejs
        nvm install 8 --latest-npm
        nvm use 8
    fi
fi

if [[ $install_arduino && $install_arduino == true ]]; then
    if [[ ! $(dpkg -s python) ]]; then
        # install python 2.7
        sudo apt-get install python -y
    fi
    if [[ ! $(dpkg -S `which pip`) ]]; then
        # install pip for platformio
        sudo apt-get install python-pip -y
    fi

    # install platformio to compile for Arduino platform
    sudo -H pip install platformio # it only seems to work with "sudo -H", the normal command does not work
fi

if [[ $install_vision && $install_vision == true ]]; then
    if [ ! -d "streameye" ]; then
        # install streamEye to allow video streaming to webapp
        git clone https://github.com/ccrisan/streameye.git
        cd streameye
        make
        sudo make install
        cd ..
    fi

    # install openCV deps
    sudo apt-get install build-essential pkg-config -y
    if [[ $vision_platform == "pi" ]]; then
        sudo apt-get install libjpeg-dev libtiff5-dev libjasper-dev libpng12-dev -y
    fi
    sudo apt-get install libavcodec-dev libavformat-dev libswscale-dev libv4l-dev -y
    sudo apt-get install libxvidcore-dev libx264-dev -y
    if [[ $vision_platform == "pi" ]]; then
        sudo apt-get install libgtk2.0-dev libgtk-3-dev -y
        sudo apt-get install libcanberra-gtk* -y
    fi
    sudo apt-get install libatlas-base-dev gfortran -y

    # install openCV, will not include opencv_contrib
    wget -O opencv.zip https://github.com/opencv/opencv/archive/3.4.2.zip
    unzip opencv.zip
    cd opencv-3.4.2/
    mkdir build -p
    cd build
    if [[ $vision_platform == "pi" ]]; then
        echo "Building OpenCV for Raspberry PI"
        cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D ENABLE_NEON=ON -D ENABLE_VFPV3=ON -D BUILD_TESTS=OFF -D INSTALL_PYTHON_EXAMPLES=OFF -D BUILD_EXAMPLES=OFF ..

        # expand swap to allow multi-core
        sed -i '/CONF_SWAPSIZE=/c\CONF_SWAPSIZE=1024' /etc/dphys-swapfile
        # reload swap service
        sudo /etc/init.d/dphys-swapfile stop
        sudo /etc/init.d/dphys-swapfile start

        # run multi-core build
        make -j4
        sudo make install
        sudo ldconfig

        # reset swap to normal size
        sed -i '/CONF_SWAPSIZE=/c\CONF_SWAPSIZE=100' /etc/dphys-swapfile
        # reload swap service
        sudo /etc/init.d/dphys-swapfile stop
        sudo /etc/init.d/dphys-swapfile start
    elif [[ $vision_platform == "amd" ]]; then
        cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D BUILD_TESTS=OFF -D INSTALL_PYTHON_EXAMPLES=OFF -D BUILD_EXAMPLES=OFF ..

        # run multi-core build
        make -j4
        sudo make install
        sudo ldconfig
    else
        echo "Unkown platform \"$vision_platform\" for OpenCV build."
    fi
fi

if [[ $install_pm2 && $install_pm2==true ]]; then
    # install pm2 to manage running processes as services
    npm install pm2@latest -g

    # make pm2 start on boot
    pm2 startup | tail -1 | . /dev/stdin
fi

if [[ $install_cloud9 && $install_cloud9==true ]]; then
	# install cloud9 for remote development
    git clone git://github.com/c9/core.git cloud9
    cd cloud9
    scripts/install-sdk.sh
    cd ..
fi
