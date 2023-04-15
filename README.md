## Warning
Do not use 'lxmon', if you don't use LXDE on a raspberry pi.

Do not use 'lxmon', if you are not sure what to achieve with it.
## Background
It just so happens to my raspberry 4, that it does not boot into an lxsession with a 
probability of approx. 10%.
So far, I could not find a root cause for it.
Not good for a headless operated system.
The program 'lxmon' is intended to provide a workaround for such a situation.
## Introduction
The program 'lxmon' checks after its start, whether a 'lxsession' is active or not.
If an active lxsession was found, the program simply ends.
If no active lxsession was found, it checks again for another 10 times with 20s pause in between.
If there is still no active lxsession after such 10 iterations, the system is forced to reboot.
After a maximum of 10 reboots, 'lxmon' gives up and the system is finally shut down.

To make 'lxmon' work, the followings steps are needed:
1. copy 'lxmon' into a folder of your choice, as best into a user home folder
2. edit crontab
   * crontab -e
   * @reboot sleep 45 && /path_to_lxmon/lxmon &

The above procedure will start 'lxmon' 45s after any reboot of the system.

'lxmon' writes its output to the console (only visible if running in terminal), 
a daily logfile 'lxmon_YYYMMDD.log' and a settings file 'lxmon.ini'.
## Build yourself
'lxmon' uses Qt features, therefore parts of the Qt framework are needed.
Choose at least one a) or b) of the following build enviromnents. 

a) cross compile environment
 * follow https://github.com/abhiTronix/raspberry-pi-cross-compilers/blob/master/QT_build_instructions.md
 * download & unzip 'lxmon' repository
 * build & run

b) raspberry
 * sudo apt-get install qt5-default
 * sudo apt-get install qtcreator
 * sudo apt-get install libqt5serialport5
 * sudo apt-get install libqt5serialport5-dev
 * configure QtCreator
 * download & unzip 'lxmon' repository
 * simplify lxmon.pro file
 QT -= gui
 CONFIG += c++17 console
 CONFIG -= app_bundle
 SOURCES += main.cpp
 HEADERS += logger.h
 * uncheck checkbox Project/Run Setting/ 'Run in terminal' to be able to use debugger
 * build & run
 
