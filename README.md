# Diploma-Thesis-Project

Hi there! In this repository you will find a software written in C++. This software was part of my diploma-thesis for graduating from the secondary technical college in Salzburg.

First of all, I have to admit that the software is far away from being clean and reader-friendly. However, I am about to refactor every single line of code in a new repository.
As I am working full time while I am about to launch my first online shop, I do not really have much time left for refactoring at the moment.

## Abstract
This software is part of a compact measuring and testing device for optical storage media. Our team developed the so-called «MMP-CrackTest» in cooperation with Sony DADC.

The «MMP-CrackTest» is used to determine whether a optical storage media bears a specific nominal value of force, or not. Furthermore, media are loaded with a maximum value of force. While loaded with a maximum value of force the optical media may break, whereas loaded with a nominal value of force the optical media should not break.

These specific values of force (nominal, maximum) were determined empirically.

This C++ software covers:
* communicating with hardware devices over I2C,
* file handling for saving parameters and logging,
* a server which communicates via TCP sockets.
