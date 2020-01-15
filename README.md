# My ARIA interface to the Pioneed P3DX robot

The pioneer P3DX robot is an old mobile robot widely used for research and student projects. 
Here there is an working interface with a nice GUI that allows the robot to be used in windows using the 
classical ARIA library forked from this github <a href="https://github.com/cinvesrob/Aria">repository</a> 
and modified to easily build with cmake. 
The interface should also working in linux by removing the agent client (which is based on the winsocket )
The agent visible in the code is a client application that allows an external application to send simple 
commands to the robot (not required for building nor for using the example).
This was used in a student project. 

This GUI is intended to be used for testing purposes and as a starting point for other learning projects.

This software is released under the terms of the <a href="https://www.gnu.org/licenses/gpl-3.0.en.html">GNU GPL</a> license. 


# Dependences

The external dependences for this applications are:
  - QT  v. >5.4, for GUI - https://www.qt.io/
  - <a href="http://www.cmake.org">CMake</a> to generate the solution for your system and build. 


# Building Instructions:

Use CMake to generate the solution for your system, see http://www.cmake.org

If the QT_DIR is not found automatically set it to:
 __YOUR_QT_INSTALLATION_FOLDER__/__YOUR_QT_VERSION/___YOUR_COMPILER_VERSION___/lib/cmake/Qt5/

# Contribute to the code:

New developers can contribute to the code by compiling in Windows or Linux.
QT GUI must be modified using QT creator/designer


---------------------------------------------------------------------
<sup> Software released under GNU GPL License. <br>
Author: Mauro Bellone, http://www.maurobellone.com <br> </sup>
