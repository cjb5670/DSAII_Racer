#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	////Change this to your name and email
	//m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUp(
		vector3(0.0f, 0.0f, 13.0f), //Position
		vector3(0.0f, 0.0f, 12.0f),	//Target
		AXIS_Y);					//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)
	
	//Entity Manager
	m_pEntityMngr = MyEntityManager::GetInstance();

	//init the camera
	m_pCamera = new Camera();
	
	//creeper
	m_pEntityMngr->AddEntity("Minecraft\\Creeper.obj", "Creeper");
	m_pEntityMngr->SetAxisVisibility(true, "Creeper"); //set visibility of the entity's axis

	//steve
	m_pEntityMngr->AddEntity("Minecraft\\Steve.obj", "Steve");
	m_pEntityMngr->SetAxisVisibility(true, "Steve"); //set visibility of the entity's axis

	//add an entity
	m_pEntityMngr->AddEntity("Minecraft\\Cow.obj", "Cow");
	//set the model matrix and visibility of the last entity added
	m_pEntityMngr->SetModelMatrix(glm::translate(vector3(2.0f,-1.5f,-1.0f)));
	m_pEntityMngr->SetAxisVisibility(true);

	//add an entity
	m_pEntityMngr->AddEntity("Minecraft\\Zombie.obj", "Zombie");
	//set the model matrix and visibility of the last entity added
	m_pEntityMngr->SetModelMatrix(glm::translate(vector3(0.0f, -2.5f, 0.0f)));
	m_pEntityMngr->SetAxisVisibility(true);

	//add an entity
	m_pEntityMngr->AddEntity("Minecraft\\Pig.obj", "Pig");
	//set the model matrix and visibility of the last entity added
	m_pEntityMngr->SetModelMatrix(glm::translate(vector3(-2.0f,-1.0f,-1.0f)));
	m_pEntityMngr->SetAxisVisibility(true);

	//Our Models
	m_pEntityMngr->AddEntity("Racer\\bbill\\itemKiller.obj", "itemKiller");
	//add an entity
	m_pEntityMngr->AddEntity("Racer\\bowser\\HandTrack.obj", "HandTrack");
	//set the model matrix
	m_pEntityMngr->SetModelMatrix(glm::translate(vector3(0.0f, -20.0f, 0.0f)));


	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUp(
		vector3(0.0f, 5.0f, -10.0f),				//Position
		m_v3PlayerPos + vector3(0.0f, 4.5f, 0.0f),	//Target
		AXIS_Y);									//Up

	//Get the singleton
	m_pMeshMngr = MeshManager::GetInstance();
	m_pMyMeshMngr = MyMeshManager::GetInstance();
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Sets player position and rotation
	m_pEntityMngr->SetModelMatrix(glm::translate(m_v3PlayerPos) * glm::toMat4(m_qPlayerQuat) * glm::scale(vector3(0.012f, 0.012f, 0.012f)), "itemKiller");

	//updates the camera
	m_pCameraMngr->SetPositionTargetAndUp(
		m_v3PlayerPos + (m_qPlayerQuat * vector3(0.0f, 5.0f, -10.0f)),		//Position
		m_v3PlayerPos + vector3(0.0f, 4.5f, 0.0f),							//Target
		AXIS_Y);															//Up
	
	if (CheckFinish(m_v3PlayerPos, vector3(0, 0, 10), 1.0f))
	{
		ResetTimer();
	}

	m_pMeshMngr->AddCubeToRenderList(glm::translate(vector3(0.0f, 0.0f, 10.0f)), C_WHITE, 1);


	//Set model matrix to the creeper
	matrix4 mCreeper = glm::translate(m_v3Creeper) * ToMatrix4(m_qCreeper) * ToMatrix4(m_qArcBall);
	m_pEntityMngr->SetModelMatrix(mCreeper, "Creeper");
	

	//Set model matrix to Steve
	matrix4 mSteve = glm::translate(vector3(2.5f, 0.0f, 0.0f)) * glm::rotate(IDENTITY_M4, -55.0f, AXIS_Z);
	m_pEntityMngr->SetModelMatrix(mSteve, "Steve");


	//Move the last entity added slowly to the right
	matrix4 lastMatrix = m_pEntityMngr->GetModelMatrix();// get the model matrix of the last added
	lastMatrix *= glm::translate(IDENTITY_M4, vector3(0.01f, 0.0f, 0.0f)); //translate it
	m_pEntityMngr->SetModelMatrix(lastMatrix); //return it to its owner

	//Update Entity Manager
	m_pEntityMngr->Update();
		
	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();

	//Initialize a timer
	InitTimer();
	fTimerResettable = fTimer - fTimerOffset;
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release the entity manager
	m_pEntityMngr->ReleaseInstance();

	//release GUI
	ShutdownGUI();
}

// Initializes the timer
void Application::InitTimer()
{
	//Make a timer
	static uint uClock = m_pSystem->GenClock();
	fTimer = (int)m_pSystem->GetTimeSinceStart(uClock);
}
// Resets the timer
void Application::ResetTimer()
{
	fTimerOffset = fTimer;
	fTimerResettable = 0; // solves a 1 frame bug
}

bool Application::CheckFinish(vector3 posA, vector3 posB, float buffer)
{
	if (posA.x + buffer > posB.x - buffer && posA.x - buffer < posB.x + buffer &&
		posA.y + buffer > posB.y - buffer && posA.y - buffer < posB.y + buffer &&
		posA.z + buffer > posB.z - buffer && posA.z - buffer < posB.z + buffer)
	{
		return true;
	}
	return false;
}