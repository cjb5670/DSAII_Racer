#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	////Change this to your name and email
	//m_sProgrammer = "Cameron Bajus - cjb5670@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUp(AXIS_Z * 10.0f, ZERO_V3, AXIS_Y);

	//init the camera
	m_pCamera = new MyCamera();
	

	//player
	//Entity Manager
	m_pEntityMngr = EntityManager::GetInstance();

	//Replace this file name with proper file name, also make the ID different. This ID is used once again in Update with SetModelMatrix
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
	m_pMyMeshMngr = MyMeshManager::GetInstance();
	m_pMyMeshMngr->SetCamera(m_pCamera);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Sets player position and rotation
	m_pEntityMngr->SetModelMatrix(glm::translate(m_v3PlayerPos) * glm::toMat4(m_qPlayerQuat) * glm::scale(vector3(0.012f, 0.012f, 0.012f)), "itemKiller");

	//updates the camera. Replace v3TempLocation with the location of the player, and qTempOrientation with it's orientation
	vector3 v3TempLocation = vector3();
	quaternion qTempOrientation = quaternion();

	m_pCamera->Update(m_v3PlayerPos, m_qPlayerQuat);
	if (CheckFinish(m_v3PlayerPos, vector3(0, 0, 10), 1.0f))
	{
		ResetTimer();
	}

	//Add objects to the Manager
	//for (int j = -50; j < 50; j += 2)
	//{
	//	for (int i = -50; i < 50; i += 2)
	//	{
	//		m_pMyMeshMngr->AddConeToRenderList(glm::translate(vector3(i, 0.0f, j)));
	//	}
	//}


	m_pMyMeshMngr->AddCubeToRenderList(glm::translate(vector3(0.0f, 0.0f, 10.0f)));

	//Update Entity Manager
	m_pEntityMngr->Update();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);

}
void Application::Display(void)
{
	//Clear the screen
	ClearScreen();

	//clear the render list
	m_pMeshMngr->ClearRenderList();

	//Render the list of MyMeshManager
	m_pMyMeshMngr->Render();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the MyMeshManager list
	m_pMyMeshMngr->ClearRenderList();
	
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

	//release the singleton
	MyMeshManager::ReleaseInstance();

	//release the camera
	SafeDelete(m_pCamera);

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
		posA.z + buffer > posB.z - buffer && posA.z - buffer < posB.z + buffer )
	{
		return true;
	}
	return false;
}