<!-- <p align="center">
  <a href="http://usub.cs.usu.edu/">
    <img src="http://usub.cs.usu.edu/Images/Logo_Large.png"
          height="200"></a>
</p> -->

<p align="center">
  <a href="https://travis-ci.org/USU-Robosub/submarine">
    <img src="https://img.shields.io/travis/USU-Robosub/submarine/develop.svg"
      alt="Travis-CI Build Status"></a>
  <a href="https://coveralls.io/github/USU-Robosub/submarine?branch=develop">
    <img src="https://img.shields.io/coveralls/github/USU-Robosub/submarine/develop.svg"
      alt="Coveralls Coverage"></a>
</p>

This repository contains the software for the second generation of Poseidon, USU's latest autonomous submarine.
This submarine is the third generation of our submarine, built by the mechanical, electrical, and software engineers at Utah State University.
The quality of Poseidon sets it apart from previous generations of submarines. Poseidon contains cutting edge hardware, has an easy to maintain design, and cleaner code.

## Setup

The setup script is for linux based systems with `apt-get`. The script also requires `sudo` privileges to run some commands.

### Laptop / Desktop

#### Just the Source Code

Want to take a look at the project's source code? You won't be able to compile the tests or run the included applications. Run to following command.

```
git clone https://github.com/USU-Robosub/submarine.git
```

#### Development

Want to run all the submarine's code on your laptop? You will need some extra software. Use the following command to download the repository, and setup your development environment automatically.

```
wget -qO- https://raw.githubusercontent.com/USU-Robosub/submarine/develop/scripts/setup.sh | bash -s -- -rcja
```

If you already have the repository downloaded, use the following command.

```
bash submarine/scripts/setup.sh -cja
```

#### Development + Vision

Want to test the submarine's vision code? Use the following command to download the repository, and setup your development environment automatically. This will take much longer than a normal development install.

```
wget -qO- https://raw.githubusercontent.com/USU-Robosub/submarine/develop/scripts/setup.sh | bash -s -- -rcjav amd
```

If you already have the repository downloaded, use the following command.

```
bash submarine/scripts/setup.sh -cjav amd
```

*Note: Run the above command as is. Don't use a different path. New directories will be created for some of the dependancies and we don't want them to be created in the repository.*

### Submarine's On Board Computers

#### Main Raspberry PI

Use the following commands to setup a Raspberry PI to be used as the main computer.

```
wget -qO- https://raw.githubusercontent.com/USU-Robosub/submarine/develop/scripts/setup.sh | bash -s -- -rcjap9h
bash submarine/scripts/add_cloud9_to_pm2.sh
bash submarine/scripts/add_[submarine's name]_to_pm2.sh
```

#### Vision Raspberry PI

Use the following commands to setup a Raspberry PI to be used for vision processing.

```
wget -qO- https://raw.githubusercontent.com/USU-Robosub/submarine/develop/scripts/setup.sh | bash -s -- -rcp9hv pi
bash submarine/scripts/add_cloud9_to_pm2.sh
bash submarine/scripts/add_[submarine's name]_vision_to_pm2.sh
```
