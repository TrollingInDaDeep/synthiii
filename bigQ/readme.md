How to install and setup a linux distro for running the Synth
Trying to use an old compaq netbook with Intel Atom N270 (1.6ghz)
Should be much faster than RPI 3b+


### Create a Linux USB stick ###

credits to: https://www.instructables.com/Install-any-linux-from-a-usb-the-easy-way/
Using tool: http://unetbootin.github.io/

1. Download unetbootin tool
2. Download Ubuntu ISO
3. Launch tool, select Ubuntu ISO to install on your desired USB stick


### Install Linux onto netbook from USB stick ###

1. Boot from USB stick
2. install ubuntu
3. don't lose your heart, use the correct ubuntu version for your CPU, use a stick that actually boots, don't rip your hair out


### Install Real Time patch ###

I don't know man just bugs me, it's probably not that hard but don't want to waste time on this RN
In case Real time input is shitty, I can come back to this


### Install what you need for the synthi ###

0. establish ssh / VNC connection
sudo apt-get install openssh-server
sudo systemctl enable ssh --now
sudo systemctl start ssh

sudo apt-get install tightvncserver
tightvncserver -geometry 600x400

1. get audio working (jack / alsa)
sudo apt-get install jackd jack qjackctl
Jack installation and config: https://wiki.ubuntuusers.de/JACK/
Make connections in jack via shell: https://askubuntu.com/questions/1153655/making-connections-in-jack-on-the-command-line
start jack server (GUI): qjackctl



2. install puredata
apt-get install puredata

3. setup GIT repo
sudo apt install git
https://jdblischak.github.io/2014-09-18-chicago/novice/git/05-sshkeys.html
ssh-keygen -o -t rsa -C "your@email.com"
add public key (cat ~/.ssh/id_rsa.pub) to github

git clone git@github.com:TrollingInDaDeep/synthiii.git

4. Other tools
sudo apt install vim

5. optimize audio settings
Jack settings
audio buffer / samplerate etc in puredata


