Readme
======

Brainstorming
-------------
* For at lave vores obstacle-avoidance program *rigtig* godt, skal vi bruge yderligere framadrettet sensorer. Dette kan gøres ved at lave en vægtning af sensorerene. F.eks.

```c
double s_l = bn_wnw * 0.2 + tw_nnw * 0.4 + bn_nw * 0.4;
```
* Side-sensorene kunne have indflydelse på den fremadrettede sensor, hvis de har tilstrækkelig lav vægtning. F.eks.

 ```c
 double s_f = bn_n * 0.8 + bn_nw * 0.1 + bn_ne * 0.1
 ```
* Alle sensordata burde normaliseres, [0; 1.6] er lidt for magic-number-agtigt.
* Hvis alle sensorer nedarvede fra én overklasse, kunne vi lave en instans, som have vægtning indbygget. Denne kunne f.eks. hedde `CompoundSensor` eller `SensorGroup`.
* Hvis robotten bliver nød til at bakke, skal vi også sørge for at den drejer en smule, så den ikke sidder fast.

SSH til robot-maskine
---------------------
```
ssh diku@192.168.43.63
```

```
sshfs diku@192.168.43.63:/home/diku /Your/Path -C -oallow_other
```

Links
-----
* [libplayerc++ dokumentation](http://playerstage.sourceforge.net/doc/Player-cvs/player/group__player__clientlib__cplusplus.html)
* [Scorpion dokumentation](http://image.diku.dk/mediawiki/images/e/e7/Usermanualdoc_HandedIn.pdf)
* [OpenCv Object Tracking](http://opencv-srf.blogspot.de/2010/09/object-detection-using-color-seperation.html)
