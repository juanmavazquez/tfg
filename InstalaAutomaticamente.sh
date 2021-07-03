#Instalacion de Mosquitto:

sudo apt-get install -y mosquitto mosquitto-clients libmosquitto-dev

#Instalacion de las dependecias para Web-scaping:

sudo apt-get install -y python-bs4
pip install beautifulsoup4
sudo apt-get install -y python-lxml python-html5lib 

#Instalacion de Node-Red

bash <(curl -sL https://raw.githubusercontent.com/node-red/linux-installers/master/deb/update-nodejs-and-nodered)

#Instalacion de Home Assistant

sudo useradd -rm homeassistant -G dialout
cd /srv
sudo mkdir homeassistant
sudo chown homeassistant:homeassistant homeassistant
sudo -u homeassistant -H -s
cd /srv/homeassistant
python3 -m venv .
source bin/activate
python3 -m pip install wheel
pip3 install homeassistant
