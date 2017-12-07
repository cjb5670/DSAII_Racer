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
	m_pEntityMngr = EntityManager::GetInstance();

	//init the camera
	m_pCamera = new Camera();
	
	//player
	m_pEntityMngr->AddEntity("Racer\\bbill\\itemKiller.obj", "itemKiller");
	m_pEntityMngr->SetAxisVisibility(true, "itemKiller"); //set visibility of the entity's axis

	//map
	m_pEntityMngr->AddEntity("Racer\\bowser\\flatMap.obj", "flatMap");
	m_pEntityMngr->SetModelMatrix(glm::translate(vector3()));
	m_pEntityMngr->SetAxisVisibility(true);

	//gate
	m_pEntityMngr->AddEntity("Racer\\bowser\\gate.obj", "gate");
	m_pEntityMngr->SetModelMatrix(glm::translate(vector3(0.0f, -1.0f, 0.0f)));
	m_pEntityMngr->SetAxisVisibility(true);

	//wall1
	m_pEntityMngr->AddEntity("Racer\\walls\\bigwall.obj", "bigwall1");
	//set the model matrix and visibility of the last entity added
	m_pEntityMngr->SetModelMatrix(glm::translate(vector3(-1.0f, -1.5f, -35.0f)));
	m_pEntityMngr->SetAxisVisibility(true);


	//wall2
	m_pEntityMngr->AddEntity("Racer\\walls\\bigwall.obj", "bigwall2");
	//set the model matrix and visibility of the last entity added
	m_pEntityMngr->SetModelMatrix(glm::translate(vector3(-1.0f, -1.5f, -11.0f)));
	m_pEntityMngr->SetAxisVisibility(true);

	//wall3
	m_pEntityMngr->AddEntity("Racer\\walls\\bigwall.obj", "bigwall3");
	//set the model matrix and visibility of the last entity added
	m_pEntityMngr->SetModelMatrix(glm::translate(vector3(-37.0f, -1.5f, -4.0f)) * glm::rotate(IDENTITY_M4, 90.0f, AXIS_Y));
	m_pEntityMngr->SetAxisVisibility(true);

	//wall4
	m_pEntityMngr->AddEntity("Racer\\walls\\bigwall.obj", "bigwall4");
	//set the model matrix and visibility of the last entity added
	m_pEntityMngr->SetModelMatrix(glm::translate(vector3(-13.5f, -1.5f, -4.0f)) * glm::rotate(IDENTITY_M4, 90.0f, AXIS_Y));
	m_pEntityMngr->SetAxisVisibility(true);

	//center
	m_pEntityMngr->AddEntity("Racer\\walls\\center.obj", "center");
	//set the model matrix and visibility of the last entity added
	m_pEntityMngr->SetModelMatrix(glm::translate(vector3()));
	m_pEntityMngr->SetAxisVisibility(true);


	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUp(
		vector3(0.0f, 5.0f, -10.0f),				//Position
		m_v3PlayerPos + vector3(0.0f, 4.5f, 0.0f),	//Target
		AXIS_Y);									//Up

	//Get the singleton
	m_pMeshMngr = MeshManager::GetInstance();
	m_pMyMeshMngr = MyMeshManager::GetInstance();

	m_pRoot = new MyOctant(3, 5);
}
void Application::Update(void)
{
	SafeDelete(m_pRoot);
	m_pRoot = new MyOctant(3, 5);

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
		m_v3PlayerPos + (m_qPlayerQuat * vector3(0.0f, 0.9f, -1.0f)),		//Position
		m_v3PlayerPos + vector3(0.0f, 0.7f, 0.0f),							//Target
		AXIS_Y);															//Up
	
	if (CheckFinish(m_v3PlayerPos, vector3(-3, 0, 7.5), 1.0f))
	{
		ResetTimer();
	}

	// Marker for finish line
	//m_pMeshMngr->AddCubeToRenderList(glm::translate(vector3(-3.0f, 0.0f, 7.5f)), C_WHITE, 1);


	//Set model matrix to the creeper
	matrix4 mBill = glm::translate(m_v3PlayerPos + vector3(0, 0.58f, 0)) * ToMatrix4(m_qPlayerQuat) * glm::scale(vector3(0.006f));
	m_pEntityMngr->SetModelMatrix(mBill, "itemKiller");

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

	//draw octree
	if (displayOctree)
		m_pRoot->Display(C_YELLOW);
	
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