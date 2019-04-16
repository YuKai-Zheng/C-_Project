#! /bin/bash
cd ~/Documents/hgGame/MobileClient_1099_DEMO/code/game/MixProject/tools;
./DelPng.sh;
java -jar DeleteResource.jar;
java -jar GeneratePVR.jar;
cd ~/Documents/hgGame/MobileClient_1099_DEMO/code/game/MixProject/tools/removePNG_cfg;
ant -buildfile removePNG.xml;
