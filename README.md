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

The above procedure will start 'lxmon' after any reboot of the system.

'lxmon' writes its output to the console (only visible if running in terminal), 
a daily logfile 'lxmon_YYYMMDD.log' and a settings file 'lxmon.ini'.


