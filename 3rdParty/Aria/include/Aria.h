/*
Adept MobileRobots Robotics Interface for Applications (ARIA)
Copyright (C) 2004-2005 ActivMedia Robotics LLC
Copyright (C) 2006-2010 MobileRobots Inc.
Copyright (C) 2011-2015 Adept Technology, Inc.
Copyright (C) 2016 Omron Adept Technologies, Inc.

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License, or
     (at your option) any later version.

     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with this program; if not, write to the Free Software
     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

If you wish to redistribute ARIA under different terms, contact 
Adept MobileRobots for information about a commercial version of ARIA at 
robots@mobilerobots.com or 
Adept MobileRobots, 10 Columbia Drive, Amherst, NH 03031; +1-603-881-7960
*/
#ifndef ARIA_H
#define ARIA_H

#include "ariaOSDef.h"
#include "ariaTypedefs.h"
#include "ArSerialConnection.h"
#include "ArTcpConnection.h"
#include "ArSimpleConnector.h"
#include "ArLogFileConnection.h"
#include "ArLog.h"
#include "ArRobotPacket.h"
#include "ArRobotPacketSender.h"
#include "ArRobotPacketReceiver.h"
#include "ArRobotConfigPacketReader.h"
#include "ArRobotTypes.h"
#include "ariaUtil.h"
#include "ArArgumentBuilder.h"
#include "ArArgumentParser.h"
#include "ArFileParser.h"
#include "ArConfig.h"
#include "ArConfigArg.h"
#include "ArConfigGroup.h"
#include "ArRobot.h"
#include "ArCommands.h"
#include "ArJoyHandler.h"
#include "ArSyncTask.h"
#include "ArTaskState.h"
#include "ariaInternal.h"
#include "ArSonarDevice.h"
#include "ArPriorityResolver.h"
#include "ArAction.h"
#include "ArActionGroup.h"
#include "ArActionGroups.h"
#include "ArActionDeceleratingLimiter.h"
#include "ArActionLimiterForwards.h"
#include "ArActionLimiterBackwards.h"
#include "ArActionLimiterTableSensor.h"
#include "ArActionBumpers.h"
#include "ArActionIRs.h"
#include "ArActionStallRecover.h"
#include "ArActionAvoidFront.h"
#include "ArActionAvoidSide.h"
#include "ArActionConstantVelocity.h"
#include "ArActionInput.h"
#include "ArActionRobotJoydrive.h"
#include "ArActionJoydrive.h"
#include "ArActionKeydrive.h"
#include "ArActionTriangleDriveTo.h"
#include "ArActionTurn.h"
#include "ArActionRatioInput.h"
#include "ArActionStop.h"
#include "ArActionGoto.h"
#include "ArModule.h"
#include "ArModuleLoader.h"
#include "ArRecurrentTask.h"
#include "ArInterpolation.h"
#include "ArGripper.h"
#include "ArSonyPTZ.h"
#include "ArRVisionPTZ.h"
#include "ArAMPTU.h"
#include "ArP2Arm.h"
#include "ArACTS.h"
#include "ArSick.h"
#include "ArLaserLogger.h"
#include "ArIrrfDevice.h"
#include "ArKeyHandler.h"
#include "ArDPPTU.h"
#include "ArVCC4.h"
#include "ArMode.h"
#include "ArModes.h"
#include "ArNetServer.h"
#include "ArSignalHandler.h"
#include "ArAnalogGyro.h"
#include "ArMapInterface.h"
#include "ArMapObject.h"
#include "ArMap.h"
#include "ArLineFinder.h"
#include "ArBumpers.h"
#include "ArIRs.h"
#include "ArDrawingData.h"
#include "ArForbiddenRangeDevice.h"
#include "ArTCM2.h"
#if !defined(WIN32) && !defined(SWIGWIN)
#include "ArVersalogicIO.h"
#include "ArMTXIO.h"
#endif
#include "ArActionGotoStraight.h"
#include "ArDataLogger.h"
#include "ArRobotJoyHandler.h"
#include "ArRatioInputKeydrive.h"
#include "ArRatioInputJoydrive.h"
#include "ArRatioInputRobotJoydrive.h"
#include "ArActionMovementParameters.h"
#include "ArSoundPlayer.h"
#include "ArSoundsQueue.h"
#include "ArCameraCollection.h"
#include "ArCameraCommands.h"
#include "ArStringInfoGroup.h"
#include "ArSonarAutoDisabler.h"
#include "ArActionDriveDistance.h"
#include "ArLaserReflectorDevice.h"
#include "ArRobotConfig.h"
#include "ArTCMCompassRobot.h"
#include "ArTCMCompassDirect.h"
#include "ArLaserFilter.h"
#include "ArUrg.h"
#include "ArSpeech.h"
#include "ArGPS.h"
#include "ArTrimbleGPS.h"
#include "ArNovatelGPS.h"
#include "ArGPSCoords.h"
#include "ArLaser.h"
#include "ArRobotConnector.h"
#include "ArLaserConnector.h"
#include "ArSonarConnector.h"
#include "ArBatteryConnector.h"
#include "ArLCDConnector.h"
#include "ArSonarMTX.h"
#include "ArBatteryMTX.h"
#include "ArLCDMTX.h"
#include "ArSimulatedLaser.h"
#include "ArExitErrorSource.h"
#include "ArActionLimiterRot.h"
#include "ArRobotBatteryPacketReader.h"
#include "ArLMS1XX.h"
#include "ArUrg_2_0.h"
#include "ArActionMovementParametersDebugging.h"
#include "ArZippable.h"
#include "ArS3Series.h"
#include "ArSZSeries.h"
#include "ArRobotPacketReaderThread.h"
#include "ArHasFileName.h"

#if 0
#ifdef __cplusplus
extern "C" {
#endif

	extern __declspec(dllexport) Aria* Aria_ctor() { return new Aria(); }

	extern __declspec(dllexport) void Aria_inint(Aria* aria_ptr) { return aria_ptr->init(); }

	extern __declspec(dllexport) void Aria_dtor(Aria* aria_ptr) { delete aria_ptr; }


#ifdef __cplusplus
}
#endif
#endif
#endif // ARIA_H
