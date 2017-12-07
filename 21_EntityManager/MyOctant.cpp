#include "MyOctant.h"
using namespace Simplex;

uint MyOctant::m_uMyOctantCount;
uint MyOctant::m_uMaxLevel;
uint MyOctant::m_uIdealEntityCount;
uint MyOctant::m_uCurrentOctantID;

Simplex::MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	m_pEntityMngr = EntityManager::GetInstance();
	m_pMeshMngr = MeshManager::GetInstance();
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;

	Init();
}

Simplex::MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
	m_pEntityMngr = EntityManager::GetInstance();
	m_pMeshMngr = MeshManager::GetInstance();

	m_uID = m_uMyOctantCount;
	m_uMyOctantCount++;

	m_v3Max = a_v3Center + vector3(a_fSize);
	m_v3Min = a_v3Center - vector3(a_fSize);
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;
}

Simplex::MyOctant::MyOctant(vector3 a_v3Max, vector3 a_v3Min, float a_fSize)
{
	m_pEntityMngr = EntityManager::GetInstance();
	m_pMeshMngr = MeshManager::GetInstance();

	m_uID = m_uMyOctantCount;
	m_uMyOctantCount++;

	m_v3Max = a_v3Max;
	m_v3Min = a_v3Min;
	m_v3Center = (a_v3Max + a_v3Min) / 2.0f;
	m_fSize = a_fSize;
}

Simplex::MyOctant::MyOctant(MyOctant const & other)
{
	m_uID = other.m_uID;
	m_uLevel = other.m_uLevel;
	m_uChildren = other.m_uChildren;

	m_fSize = other.m_fSize;

	m_pMeshMngr = other.m_pMeshMngr;
	m_pEntityMngr = other.m_pEntityMngr;
	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;

	m_pParent = other.m_pParent;
	for(int i = 0; i < m_uChildren; i++)
		m_pChild[i] = other.m_pChild[i];

	m_EntityList = other.m_EntityList;

	m_pRoot = other.m_pRoot;
	m_lChild = other.m_lChild;
}

MyOctant & Simplex::MyOctant::operator=(MyOctant const & other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyOctant temp(other);
		Swap(temp);
	}
	return *this;
}

Simplex::MyOctant::~MyOctant(void)
{
	Release();
	
}

void Simplex::MyOctant::Swap(MyOctant & other)
{
	std::swap(m_uID, other.m_uID);
	std::swap(m_uLevel, other.m_uLevel);
	std::swap(m_uChildren, other.m_uChildren);
	
	std::swap(m_fSize, other.m_fSize);
	
	std::swap(m_pMeshMngr, other.m_pMeshMngr);
	std::swap(m_pEntityMngr, other.m_pEntityMngr);
	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);
	
	std::swap(m_pParent, other.m_pParent);
	std::swap(m_pChild, other.m_pChild);
	
	std::swap(m_EntityList, other.m_EntityList);
	
	std::swap(m_pRoot, other.m_pRoot);
	std::swap(m_lChild, other.m_lChild);
}

float Simplex::MyOctant::GetSize(void)
{
	return m_fSize;
}

vector3 Simplex::MyOctant::GetCenterGlobal(void)
{
	return m_v3Center;
}

vector3 Simplex::MyOctant::GetMinGlobal(void)
{
	return m_v3Min;
}

vector3 Simplex::MyOctant::GetMaxGlobal(void)
{
	return m_v3Max;
}

uint Simplex::MyOctant::GetOctantCount(void)
{
	return m_uMyOctantCount;
}

bool Simplex::MyOctant::IsColliding(uint a_uRBIndex)
{
	/* Supposed to check if box is actually colliding with octant via aabb collision detection, but this code tanks performance
	vector3 v3Emax, v3Emin;
	v3Emax = m_pEntityMngr->GetEntity(a_uRBIndex)->GetRigidBody()->GetMaxGlobal();
	v3Emin = m_pEntityMngr->GetEntity(a_uRBIndex)->GetRigidBody()->GetMinGlobal();
	
	if ((v3Emin.x <= m_v3Max.x && v3Emax.x >= m_v3Min.x) &&
		(v3Emin.x <= m_v3Max.x && v3Emax.x >= m_v3Min.x) &&
		(v3Emin.x <= m_v3Max.x && v3Emax.x >= m_v3Min.x)) {
		return true;
	}

	return false;
	*/
	//*
	if((m_pEntityMngr->GetEntity(a_uRBIndex)->GetPosition().x >= m_v3Min.x && m_pEntityMngr->GetEntity(a_uRBIndex)->GetPosition().x < m_v3Max.x) &&
		(m_pEntityMngr->GetEntity(a_uRBIndex)->GetPosition().y >= m_v3Min.y && m_pEntityMngr->GetEntity(a_uRBIndex)->GetPosition().y < m_v3Max.y) &&
		(m_pEntityMngr->GetEntity(a_uRBIndex)->GetPosition().z >= m_v3Min.z && m_pEntityMngr->GetEntity(a_uRBIndex)->GetPosition().z < m_v3Max.z))
		return true;
	return false;
	//*/
	
}

void Simplex::MyOctant::Display(uint a_nIndex, vector3 a_v3Color)
{
	//m_pMeshMngr->AddCubeToRenderList(glm::translate(m_v3Center)* glm::scale(vector3(m_fSize)), a_v3Color, 2);
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color);
}

void Simplex::MyOctant::Display(vector3 a_v3Color)
{
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color);
	if (m_uChildren != 0) {
		for (int i = 0; i < 8; i++)
			m_pChild[i]->Display(C_YELLOW);
	}
}

void Simplex::MyOctant::DisplayLeafs(vector3 a_v3Color)
{
	if (m_uID == m_uCurrentOctantID) {
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color);
	}
	if (!IsLeaf()) {
		for (int i = 0; i < 8; i++)
			m_pChild[i]->DisplayLeafs();
	}
}

void Simplex::MyOctant::ClearEntityList(void)
{
	m_EntityList.clear();
}

void Simplex::MyOctant::Subdivide(void)
{
	/*
		   3-----0
		  /     /|
		 2-----1 |
		 | 7   | 4
		 |/    |/
		 6-----5
	*/

	m_pChild[0] = new MyOctant(m_v3Max, m_v3Center, m_fSize / 2.0f);
	m_pChild[1] = new MyOctant(vector3(m_v3Max.x, m_v3Max.y, m_v3Center.z), vector3(m_v3Center.x, m_v3Center.y, m_v3Min.z), m_fSize / 2.0f);
	m_pChild[2] = new MyOctant(vector3(m_v3Center.x, m_v3Max.y, m_v3Center.z), vector3(m_v3Min.x, m_v3Center.y, m_v3Min.z), m_fSize / 2.0f);
	m_pChild[3] = new MyOctant(vector3(m_v3Center.x, m_v3Max.y, m_v3Max.z), vector3(m_v3Min.x, m_v3Center.y, m_v3Center.z), m_fSize / 2.0f);
	m_pChild[4] = new MyOctant(vector3(m_v3Max.x, m_v3Center.y, m_v3Max.z), vector3(m_v3Center.x, m_v3Min.y, m_v3Center.z), m_fSize / 2.0f);
	m_pChild[5] = new MyOctant(vector3(m_v3Max.x, m_v3Center.y, m_v3Center.z), vector3(m_v3Center.x, m_v3Min.y, m_v3Min.z), m_fSize / 2.0f);
	m_pChild[6] = new MyOctant(m_v3Center, m_v3Min, m_fSize / 2.0f);
	m_pChild[7] = new MyOctant(vector3(m_v3Center.x, m_v3Center.y, m_v3Max.z), vector3(m_v3Min.x, m_v3Min.y, m_v3Center.z), m_fSize / 2.0f);

	/* Wrong but makes a cool pattern
	m_pChild[0] = new MyOctant((m_v3Center + m_v3Max) / 2.0f, m_fSize / 2.0f);
	m_pChild[1] = new MyOctant((m_v3Center + vector3(m_v3Max.x, m_v3Max.y, m_v3Min.z)) / 2.0f, m_fSize / 2.0f);
	m_pChild[2] = new MyOctant((m_v3Center + vector3(m_v3Min.x, m_v3Max.y, m_v3Min.z)) / 2.0f, m_fSize / 2.0f);
	m_pChild[3] = new MyOctant((m_v3Center + vector3(m_v3Min.x, m_v3Max.y, m_v3Max.z)) / 2.0f, m_fSize / 2.0f);
	m_pChild[4] = new MyOctant((m_v3Center + vector3(m_v3Max.x, m_v3Min.y, m_v3Max.z)) / 2.0f, m_fSize / 2.0f);
	m_pChild[5] = new MyOctant((m_v3Center + vector3(m_v3Max.x, m_v3Min.y, m_v3Min.z)) / 2.0f, m_fSize / 2.0f);
	m_pChild[6] = new MyOctant((m_v3Center + m_v3Min) / 2.0f, m_fSize / 2.0f);
	m_pChild[7] = new MyOctant((m_v3Center + vector3(m_v3Min.x, m_v3Min.y, m_v3Max.z)) / 2.0f, m_fSize / 2.0f);
	*/

	/* just wrong
	m_pChild[0] = new MyOctant(m_v3Max - vector3(m_fSize / 4.0f), m_fSize / 2.0f);
	m_pChild[1] = new MyOctant(m_v3Max - vector3(m_fSize / 4.0f,m_fSize / 4.0f, 3.0f * m_fSize / 4.0f), m_fSize / 2.0f);
	m_pChild[2] = new MyOctant(m_v3Max - vector3(3.0f * m_fSize / 4.0f, m_fSize / 4.0f, 3.0f * m_fSize / 4.0f), m_fSize / 2.0f);
	m_pChild[3] = new MyOctant(m_v3Max - vector3(3.0f * m_fSize / 4.0f, m_fSize / 4.0f, m_fSize / 4.0f), m_fSize / 2.0f);
							   
	m_pChild[4] = new MyOctant(m_v3Max - vector3(m_fSize / 4.0f, 3.0f * m_fSize / 4.0f, m_fSize / 4.0f), m_fSize / 2.0f);
	m_pChild[5] = new MyOctant(m_v3Max - vector3(m_fSize / 4.0f, 3.0f * m_fSize / 4.0f, 3.0f * m_fSize / 4.0f), m_fSize / 2.0f);
	m_pChild[6] = new MyOctant(m_v3Max - vector3(3.0f * m_fSize / 4.0f, 3.0f * m_fSize / 4.0f, 3.0f * m_fSize / 4.0f), m_fSize / 2.0f);
	m_pChild[7] = new MyOctant(m_v3Max - vector3(3.0f * m_fSize / 4.0f, 3.0f * m_fSize / 4.0f, m_fSize / 4.0f), m_fSize / 2.0f);
	*/
	for (int i = 0; i < 8; i++) {
		m_pChild[i]->m_pParent = this;
		m_uChildren++;
	}

}

MyOctant * Simplex::MyOctant::GetChild(uint a_nChild)
{
	return m_pChild[a_nChild];
}

MyOctant * Simplex::MyOctant::GetParent(void)
{
	return m_pParent;
}

bool Simplex::MyOctant::IsLeaf(void)
{
	return m_uChildren == 0;
}

bool Simplex::MyOctant::ContainsMoreThan(uint a_nEntities)
{
	return a_nEntities > m_EntityList.size();
}

void Simplex::MyOctant::KillBranches(void)
{
	MyOctant* ToDie;
	if (m_uChildren != 0) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				ToDie = m_pChild[i]->GetChild(j);
				SafeDelete(ToDie);
			}
			ToDie = m_pChild[i];
			SafeDelete(ToDie);
		}
	}
}

void Simplex::MyOctant::ConstructTree(uint a_nMaxLevel)
{
	if (a_nMaxLevel <= 1) {
		return;
	}
	//This makes it so that the octree won't subdivide if there isn't enough entities in the octant to subdivide
	AssignIDtoEntity();
	if (m_EntityList.size() > m_uIdealEntityCount) {
		m_EntityList.clear();
		ClearAllEntityIDs();
		Subdivide();
		for (int i = 0; i < 8; i++) {
			m_pChild[i]->ConstructTree(a_nMaxLevel - 1);
		}
		AssignIDtoEntity();
	}
	/* Old Code, don't throw out in case something went catastrophically wrong and I didn't catch it
	Subdivide();
	for (int i = 0; i < 8; i++) {
		m_pChild[i]->ConstructTree(a_nMaxLevel - 1);
	}
	AssignIDtoEntity();
	*/
}

void Simplex::MyOctant::AssignIDtoEntity(void)
{
	if (m_uChildren != 0) {
		for (int i = 0; i < 8; i++) {
			m_pChild[i]->AssignIDtoEntity();
		}
	}
	else {
		for (uint i = 0; i < m_pEntityMngr->GetEntityCount(); i++) {
			if (IsColliding(i)) {
				m_pEntityMngr->GetEntity(i)->AddDimension(m_uID);
				m_EntityList.push_back(i);
			}
		}
	}
}

void Simplex::MyOctant::ClearAllEntityIDs(void)
{
	for (uint i = 0; i < m_pEntityMngr->GetEntityCount(); i++) {
		m_pEntityMngr->GetEntity(i)->ClearDimensionSet();
		
	}
}

uint Simplex::MyOctant::GetMyOctantCount(void)
{
	return m_uMyOctantCount;
}

uint Simplex::MyOctant::GetCurrentOctantID(void)
{
	return m_uCurrentOctantID;
}

void Simplex::MyOctant::SetCurrentOctantID(uint a_uID)
{
	m_uCurrentOctantID = a_uID;
}

void Simplex::MyOctant::Release(void)
{
	m_pMeshMngr = nullptr;
	m_pEntityMngr = nullptr;
	delete(m_pParent);
	m_pParent = nullptr;
	if (m_uChildren != 0) {
		for (int i = 0; i < m_lChild.size(); i++) {
			delete(m_pChild[i]);
			m_pChild[i] = nullptr;
			delete(m_lChild[i]);
			m_lChild[i] = nullptr;
			m_uChildren = 0;
		}
	}
	m_pRoot = nullptr;
	m_uMyOctantCount = 0;
	m_uCurrentOctantID = -1;
}

void Simplex::MyOctant::Init(void)
{
	m_uID = m_uMyOctantCount;
	m_uMyOctantCount++;

	//really big/small values to find min and max
	m_v3Max = vector3(std::numeric_limits<float>::min());
	m_v3Min = vector3(std::numeric_limits<float>::max());

	//finds min and max points by looking at the farthest entities in the scene
	for (int i = 0; i < m_pEntityMngr->GetEntityCount(); i++) {
		if (m_pEntityMngr->GetEntity(i)->GetPosition().x > m_v3Max.x)
			m_v3Max.x = m_pEntityMngr->GetEntity(i)->GetPosition().x;
		else if (m_pEntityMngr->GetEntity(i)->GetPosition().x < m_v3Min.x)
			m_v3Min.x = m_pEntityMngr->GetEntity(i)->GetPosition().x;

		if (m_pEntityMngr->GetEntity(i)->GetPosition().y > m_v3Max.y)
			m_v3Max.y = m_pEntityMngr->GetEntity(i)->GetPosition().y;
		else if (m_pEntityMngr->GetEntity(i)->GetPosition().y < m_v3Min.y)
			m_v3Min.y = m_pEntityMngr->GetEntity(i)->GetPosition().y;

		if (m_pEntityMngr->GetEntity(i)->GetPosition().z > m_v3Max.z)
			m_v3Max.z = m_pEntityMngr->GetEntity(i)->GetPosition().z;
		else if (m_pEntityMngr->GetEntity(i)->GetPosition().z < m_v3Min.z)
			m_v3Min.z = m_pEntityMngr->GetEntity(i)->GetPosition().z;
	}
	//many checks cuz crunch time
	if (m_v3Max.y > m_v3Max.x)
		m_v3Max.x = m_v3Max.y;
	if (m_v3Max.z > m_v3Max.x)
		m_v3Max.x = m_v3Max.z;
	if (m_v3Max.x > m_v3Max.y)
		m_v3Max.y = m_v3Max.x;
	if (m_v3Max.z > m_v3Max.y)
		m_v3Max.y = m_v3Max.z;
	if (m_v3Max.x > m_v3Max.z)
		m_v3Max.z = m_v3Max.x;
	if (m_v3Max.y > m_v3Max.z)
		m_v3Max.z = m_v3Max.y;

	if (m_v3Min.y < m_v3Min.x)
		m_v3Min.x = m_v3Min.y;
	if (m_v3Min.z < m_v3Min.x)
		m_v3Min.x = m_v3Min.z;
	if (m_v3Min.x < m_v3Min.y)
		m_v3Min.y = m_v3Min.x;
	if (m_v3Min.z < m_v3Min.y)
		m_v3Min.y = m_v3Min.z;
	if (m_v3Min.x < m_v3Min.z)
		m_v3Min.z = m_v3Min.x;
	if (m_v3Min.y < m_v3Min.z)
		m_v3Min.z = m_v3Min.y;

	m_v3Center = (m_v3Max + m_v3Min) / 2.0f;
	m_fSize = abs(m_v3Max.x) + abs(m_v3Min.x);
	m_pRoot = this;

	ConstructTree(m_uMaxLevel);
}

void Simplex::MyOctant::ConstructList(void)
{
}
