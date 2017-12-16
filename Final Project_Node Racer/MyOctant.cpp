#include "MyOctant.h"

Simplex::uint Simplex::MyOctant::m_uOctantCount;
Simplex::uint Simplex::MyOctant::m_uMaxLevel;
Simplex::uint Simplex::MyOctant::m_uIdealEntityCount;

// Call this the first time you're making an octant
Simplex::MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{

	// If this is the first Octant, this will be the root
	if (m_pRoot == nullptr) m_pRoot = this;
	else return;

	// Store necessary variables
	m_uOctantCount++;
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;

	m_pEntityMngr = MyEntityManager::GetInstance();
	m_pMeshMngr = MeshManager::GetInstance();

	// Bounds & Location
	for (uint i = 0; i < m_pEntityMngr->GetEntityCount(); ++i) {

		MyEntity* currentBlock = m_pEntityMngr->GetEntity(i);

		m_v3Center += currentBlock->GetRigidBody()->GetCenterGlobal();

		vector3 tempMin = currentBlock->GetRigidBody()->GetMinGlobal();
		vector3 tempMax = currentBlock->GetRigidBody()->GetMaxGlobal();

		// Find the min and max
		m_v3Min.x = glm::min(tempMin.x, m_v3Min.x);
		m_v3Min.y = glm::min(tempMin.y, m_v3Min.y);
		m_v3Min.z = glm::min(tempMin.z, m_v3Min.z);
		m_v3Max.x = glm::max(tempMax.x, m_v3Max.x);
		m_v3Max.y = glm::max(tempMax.y, m_v3Max.y);
		m_v3Max.z = glm::max(tempMax.z, m_v3Max.z);

	}

	// Assign the center
	m_v3Center /= m_pEntityMngr->GetEntityCount();
	

	// Build my wireframe
	ConstructTree(a_nMaxLevel);
}

// Constructor used for children
Simplex::MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{

	m_pEntityMngr = MyEntityManager::GetInstance();
	m_pMeshMngr = MeshManager::GetInstance();

	// Keep track of id and octant count
	m_uID = m_uOctantCount;
	++m_uOctantCount;

	// Set child's location and size and verts
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;

	float halfSize = m_fSize / 2;

	m_v3Min = vector3(m_v3Center.x - halfSize, m_v3Center.y - halfSize, m_v3Center.z - halfSize);
	m_v3Max = vector3(m_v3Center.x + halfSize, m_v3Center.y + halfSize, m_v3Center.z + halfSize);

}

Simplex::MyOctant::MyOctant(MyOctant const & other)
{
}

Simplex::MyOctant::~MyOctant(void)
{
	//SafeDelete(m_lChild[m_uID]);
}

void Simplex::MyOctant::Swap(MyOctant & other)
{
}

// Gets the Width
float Simplex::MyOctant::GetSize(void){return m_v3Max.x - m_v3Min.x;}

// Finds the center of this octant
Simplex::vector3 Simplex::MyOctant::GetCenterGlobal(void){return m_v3Center;}

// Finds the min point of this octant
Simplex::vector3 Simplex::MyOctant::GetMinGlobal(void){return m_v3Min;}

// Finds the max point of this octant
Simplex::vector3 Simplex::MyOctant::GetMaxGlobal(void){return m_v3Max;}

bool Simplex::MyOctant::IsColliding(uint a_uRBIndex)
{

	if (m_pEntityMngr->GetEntity(a_uRBIndex)->GetRigidBody()->GetCenterGlobal().x > GetMinGlobal().x &&
		m_pEntityMngr->GetEntity(a_uRBIndex)->GetRigidBody()->GetCenterGlobal().x < GetMaxGlobal().x &&
		m_pEntityMngr->GetEntity(a_uRBIndex)->GetRigidBody()->GetCenterGlobal().y > GetMinGlobal().y &&
		m_pEntityMngr->GetEntity(a_uRBIndex)->GetRigidBody()->GetCenterGlobal().y < GetMaxGlobal().y &&
		m_pEntityMngr->GetEntity(a_uRBIndex)->GetRigidBody()->GetCenterGlobal().z > GetMinGlobal().z &&
		m_pEntityMngr->GetEntity(a_uRBIndex)->GetRigidBody()->GetCenterGlobal().z < GetMaxGlobal().z) {
		
		return true;
	}

	return false;
}

void Simplex::MyOctant::Display(uint a_nIndex, vector3 a_v3Color)
{

	if (m_uID == a_nIndex) {
		// Draw the Wireframe
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(GetSize(), GetSize(), GetSize()), a_v3Color);
	}
	else if (m_uChildren != 0) {
		for (uint i = 0; i < 8; ++i) {
			m_pChild[i]->Display(a_nIndex, a_v3Color);
		}
	}
}

void Simplex::MyOctant::Display(vector3 a_v3Color)
{
	// Draw the Wireframe
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(GetSize(), GetSize(), GetSize()), a_v3Color);
	
	if (m_uChildren != 0) {
		for (uint i = 0; i < 8; ++i) {
			m_pChild[i]->Display(a_v3Color);
		}
	}
}

void Simplex::MyOctant::DisplayLeafs(vector3 a_v3Color)
{
}

void Simplex::MyOctant::ClearEntityList(void)
{
}


// Subdivides the current octant into 8 children
void Simplex::MyOctant::Subdivide(void)
{

	float halfDistance = GetSize() / 2;
	float childHalfDistance = halfDistance / 2;

	m_pChild[0] = new MyOctant(GetCenterGlobal() + vector3(childHalfDistance, childHalfDistance, childHalfDistance), halfDistance);
	m_pChild[1] = new MyOctant(GetCenterGlobal() + vector3(childHalfDistance, -childHalfDistance, childHalfDistance), halfDistance);
	m_pChild[2] = new MyOctant(GetCenterGlobal() + vector3(-childHalfDistance, childHalfDistance, childHalfDistance), halfDistance);
	m_pChild[3] = new MyOctant(GetCenterGlobal() + vector3(-childHalfDistance, -childHalfDistance, childHalfDistance), halfDistance);
				  								 
	m_pChild[4] = new MyOctant(GetCenterGlobal() + vector3(childHalfDistance, childHalfDistance, -childHalfDistance), halfDistance);
	m_pChild[5] = new MyOctant(GetCenterGlobal() + vector3(childHalfDistance, -childHalfDistance, -childHalfDistance), halfDistance);
	m_pChild[6] = new MyOctant(GetCenterGlobal() + vector3(-childHalfDistance, childHalfDistance, -childHalfDistance), halfDistance);
	m_pChild[7] = new MyOctant(GetCenterGlobal() + vector3(-childHalfDistance, -childHalfDistance, -childHalfDistance), halfDistance);

	// Remember that there are 8 children
	m_uChildren = 8;
}

void Simplex::MyOctant::AddObject(String uniqueID)
{

}

Simplex::MyOctant * Simplex::MyOctant::GetChild(uint a_nChild)
{
	return nullptr;
}

Simplex::MyOctant * Simplex::MyOctant::GetParent(void)
{
	return nullptr;
}

bool Simplex::MyOctant::IsLeaf(void)
{
	return false;
}

bool Simplex::MyOctant::ContainsMoreThan(uint a_nEntities)
{
	return false;
}

void Simplex::MyOctant::KillBranches(void)
{
}

void Simplex::MyOctant::ConstructTree(uint a_nMaxLevel)
{
	// Stop making babies
	if (a_nMaxLevel == 0) return;

	// Make babies
	Subdivide();

	// Make a node for all children
	for (uint i = 0; i < m_uChildren; ++i) {
		m_pChild[i]->ConstructTree(a_nMaxLevel - 1);
	}

	AssignIDtoEntity();

	for (uint i = 0; i < m_pEntityMngr->GetEntityCount(); ++i) {

		MyEntity* currentBlock = m_pEntityMngr->GetEntity(i);

	}

}

void Simplex::MyOctant::AssignIDtoEntity(void)
{

	// If there are children
	if (m_uChildren > 0) {
		for (uint i = 0; i < m_uChildren; ++i) {
			m_pChild[i]->AssignIDtoEntity();
		}
	}
	else {
		for (uint i = 0; i < m_pEntityMngr->GetEntityCount(); ++i) {
			
			if(IsColliding(i)) {
				m_EntityList.push_back(i);
				m_pEntityMngr->GetEntity(i)->AddDimension(m_uID);
			}
		}
	}

}

Simplex::uint Simplex::MyOctant::GetOctantCount(void)
{
	return uint();
}

void Simplex::MyOctant::Release(void)
{
}

void Simplex::MyOctant::Init(void)
{
}

void Simplex::MyOctant::ConstructList(void)
{
}


