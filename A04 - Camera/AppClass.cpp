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
	m_pCamera->SetPositionTargetAndUp(
			vector3(0.0f, 3.0f, 20.0f), //Where my eyes are
			vector3(0.0f, 3.0f, 19.0f), //where what I'm looking at is
			AXIS_Y);					//what is up

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

	//updates the camera. Replace v3TempLocation with the location of the player, and qTempOrientation with it's orientation
	vector3 v3TempLocation = vector3();
	quaternion qTempOrientation = quaternion();

	m_pCamera->Update(v3TempLocation, qTempOrientation);

	//Add objects to the Manager
	for (int j = -50; j < 50; j += 2)
	{
		for (int i = -50; i < 50; i += 2)
		{
			m_pMyMeshMngr->AddConeToRenderList(glm::translate(vector3(i, 0.0f, j)));
		}
	}
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

	//Get a timer
	static uint uClock = m_pSystem->GenClock();
	fTimer = (int)m_pSystem->GetTimeSinceStart(uClock);
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release the singleton
	MyMeshManager::ReleaseInstance();

	//release the camera
	SafeDelete(m_pCamera);

	//release GUI
	ShutdownGUI();
}
