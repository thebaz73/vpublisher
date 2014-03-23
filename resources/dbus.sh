#!/bin/bash
qdbusxml2cpp -a ../src/vpublishingservice_interface.h: ./vpubservice.xml
qdbusxml2cpp -i ../src/vpublishingservice_interface.h -a :../src/vpublishingservice_interface.cpp ./vpubservice.xml
