////////////////////////////////////////////////////
// // Template code for  CS 174C
////////////////////////////////////////////////////

#ifdef WIN32
#include <windows.h>
#endif


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <shared/defs.h>

#include "shared/opengl.h"

#include <string.h>
#include <util/util.h>
#include <GLModel/GLModel.h>
#include "anim.h"
#include "animTcl.h"
#include "myScene.h"
#include "SampleParticle.h"
#include "SampleGravitySimulator.h"
#include "Hermite.h"
#include "Terrain.h"
#include "Tank.h"
#include "Missile.h"
#include "TankMissileSimulator.h"
//#include <util/jama/tnt_stopwatch.h>
//#include <util/jama/jama_lu.h>

// register a sample variable with the shell.
// Available types are:
// - TCL_LINK_INT 
// - TCL_LINK_FLOAT

int g_testVariable = 10;

SETVAR myScriptVariables[] = {
	"testVariable", TCL_LINK_INT, (char *) &g_testVariable,
	"",0,(char *) NULL
};


//---------------------------------------------------------------------------------
//			Hooks that are called at appropriate places within anim.cpp
//---------------------------------------------------------------------------------

// start or end interaction
void myMouse(int button, int state, int x, int y)
{

	// let the global resource manager know about the new state of the mouse 
	// button
	GlobalResourceManager::use()->setMouseButtonInfo( button, state );

	if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
	{
		animTcl::OutputMessage(
			"My mouse received a mouse button press event\n");

	}
	if( button == GLUT_LEFT_BUTTON && state == GLUT_UP )
	{
		animTcl::OutputMessage(
			"My mouse received a mouse button release event\n") ;
	}
}	// myMouse

// interaction (mouse motion)
void myMotion(int x, int y)
{

	GLMouseButtonInfo updatedMouseButtonInfo = 
		GlobalResourceManager::use()->getMouseButtonInfo();

	if( updatedMouseButtonInfo.button == GLUT_LEFT_BUTTON )
	{
		animTcl::OutputMessage(
			"My mouse motion callback received a mousemotion event\n") ;
	}

}	// myMotion


void MakeScene(void)
{

	/* 
	
	This is where you instantiate all objects, systems, and simulators and 
	register them with the global resource manager

	*/

	/* SAMPLE SCENE */

	bool success;
	
	// register a system
	//SampleParticle* sphere1 = new SampleParticle( "sphere1" );

	//success = GlobalResourceManager::use()->addSystem( sphere1, true );

	//// make sure it was registered successfully
	//assert( success );

	// register a simulator
	//SampleGravitySimulator* gravitySimulator = 
	//	new SampleGravitySimulator( "gravity" );

	//success = GlobalResourceManager::use()->addSimulator( gravitySimulator );

	//// make sure it was registered successfully
	//assert( success );

	/* END SAMPLE SCENE */

	// the following code shows you how to retrieve a system that was registered 
	// with the resource manager. 

	//BaseSystem* sampleSystemRetrieval;

	//// retrieve the system
	//sampleSystemRetrieval = 
	//	GlobalResourceManager::use()->getSystem( "sphere1" );

	//// make sure you got it
	//assert( sampleSystemRetrieval );

	//BaseSimulator* sampleSimulatorRetrieval;

	//// retrieve the simulator
	//sampleSimulatorRetrieval = 
	//	GlobalResourceManager::use()->getSimulator( "gravity" );

	//// make sure you got it
	//assert( sampleSimulatorRetrieval );

}	// MakeScene

// OpenGL initialization
void myOpenGLInit(void)
{
	animTcl::OutputMessage("Initialization routine was called.");

}	// myOpenGLInit

void myIdleCB(void)
{
	
	return;

}	// myIdleCB

void myKey(unsigned char key, int x, int y)
{
	 animTcl::OutputMessage("My key callback received a key press event\n");
	return;

}	// myKey

static int testGlobalCommand(ClientData clientData, Tcl_Interp *interp, int argc, myCONST_SPEC char **argv)
{
	 animTcl::OutputMessage("This is a test command!");
    animTcl::OutputResult("100") ;
	return TCL_OK;

}	// testGlobalCommand

static int Part1(ClientData clientData, Tcl_Interp* interp, int argc, myCONST_SPEC char** argv)
{
	animTcl::OutputMessage("Adding new hermite spline!");
	bool success;

	Hermite* spline = new Hermite("hermite");
	if(spline)
		success = GlobalResourceManager::use()->addSystem(spline, true);

	assert(success);
	return TCL_OK;

}
static int Part2(ClientData clientData, Tcl_Interp* interp, int argc, myCONST_SPEC char** argv)
{
	animTcl::OutputMessage("Part2!");
	bool success;
	//Terrain
	animTcl::OutputMessage("Adding new Terrain to the Scene!");

	Terrain* terrain = new Terrain("terrain", 1, 1, 1);
	if (terrain)
		success = GlobalResourceManager::use()->addSystem(terrain, true);

	assert(success);
	//Set terrain position
	double *state = new double[6];
	
	state[0] = 0;
	state[2] = -10;
	state[1] = 0.0;
	state[3] = state[4] = 5;
	state[5] = .1;
	terrain->setState(state);

	
	
	Tank *tank = new Tank("tankpath");
	if(tank)
		success = GlobalResourceManager::use()->addSystem(tank, true);
	assert(success);
	tank->readModel("data/tank.obj");
	
	state[0] = 5;
	state[2] = 4;

	state[3] = state[4] = state[5] = 2;
	tank->setState(state);

	delete[] state;
	state = 0;
	Missile *miss = new Missile("missile");
	if(miss)
		success = GlobalResourceManager::use()->addSystem(miss, true);
	assert(success);
	miss->readModel("data/missile.obj");

	TankMissileSimulator* sim = new TankMissileSimulator("Sim", tank, miss);
	if(sim)
		success = GlobalResourceManager::use()->addSimulator(sim);
	assert(success);


	return TCL_OK;
}	

void mySetScriptCommands(Tcl_Interp *interp)
{

	// here you can register additional generic (they do not belong to any object) 
	// commands with the shell

	Tcl_CreateCommand(interp, "test", testGlobalCommand, (ClientData) NULL,
					  (Tcl_CmdDeleteProc *)	NULL);
	Tcl_CreateCommand(interp, "part1", Part1, (ClientData)NULL,
		(Tcl_CmdDeleteProc*)NULL);
	Tcl_CreateCommand(interp, "part2", Part2, (ClientData)NULL,
		(Tcl_CmdDeleteProc*)NULL);


}	// mySetScriptCommands
