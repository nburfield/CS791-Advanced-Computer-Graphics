/*
  The mesh.cpp was refrenced From: http://ogldev.atspace.co.uk/

  Additions have been made to it from: 
                                      Nolan Burfield

  This Class Description Can be found in mesh.h
*/

//Headers
#include <mesh2.h>

//Each Mesh Entry Constructor
BasicMesh::MeshEntry::MeshEntry()
   {
	NumIndices  = 0;
	MaterialIndex = INVALID_MATERIAL;
   }

//Mesh Entry Destructor
BasicMesh::MeshEntry::~MeshEntry()
   {
	glDeleteBuffers(1, &VB);
	glDeleteBuffers(1, &IB);
   }

//Sets up the buffers and adds in the data
void BasicMesh::MeshEntry::Init(const std::vector<BasicVertex>& Vertices,
                           const std::vector<unsigned int>& Indices)
   {
	NumIndices = Indices.size();

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BasicVertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NumIndices, &Indices[0], GL_STATIC_DRAW);
   }

//Nothing to do for a mesh constructor
BasicMesh::BasicMesh()
   {
   }

//Destructor of mesh calls clear
BasicMesh::~BasicMesh()
   {
	Clear();
   }

//Clearing the mesh deletes all the textures
void BasicMesh::Clear()
   {
	for (unsigned int i = 0 ; i < m_Textures.size() ; i++)
	   {
		delete m_Textures[i];
	   }
   }

//The load mesh is the main function to read in all the 
//Data from a file and store the necesasary contents
bool BasicMesh::LoadMesh(const char* Filename)
   {
	// Release the previously loaded mesh (if it exists)
	Clear();

	bool Ret = false;
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(Filename, aiProcess_Triangulate/* | aiProcess_GenSmoothNormals | aiProcess_FlipUVs*/);

	if(pScene) 
	   {
		Ret = InitFromScene(pScene, Filename);
	   }
	else
	   {
		std::cerr << "Model file did not load there was a problem" << std::endl;
	   }

	return Ret;
   }

//Inits each mesh of the scene
bool BasicMesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
   {  
	m_Entries.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);

	// Initialize the meshes in the scene one by one
	for (unsigned int i = 0 ; i < m_Entries.size() ; i++)
	   {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh);
	   }

	return InitMaterials(pScene, Filename);
   }

//Inits the individual meshes
void BasicMesh::InitMesh(unsigned int Index, const aiMesh* paiMesh)
   {
	m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;

	std::vector<BasicVertex> Vertices;
	std::vector<unsigned int> Indices;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++)
	   {
		const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal   = paiMesh->HasNormals() ? &(paiMesh->mNormals[i]) : &Zero3D;
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		BasicVertex v(glm::vec3(pPos->x, pPos->y, pPos->z), glm::vec2(pTexCoord->x, pTexCoord->y), glm::vec3(pNormal->x, pNormal->y, pNormal->z));
		 //v.m_pos[0] = pPos->x; 
		 //v.m_pos[1] = pPos->y;  
		 //v.m_pos[2] = pPos->z;
		 //v.m_tex[0] = pTexCoord->x;
		 //v.m_tex[1] = pTexCoord->y;
		 //v.m_normal[0] = pNormal->x; 
		 //v.m_normal[1] = pNormal->y; 
		 //v.m_normal[2] = pNormal->z;



    
		Vertices.push_back(v);
	   }

	for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++)
	   {
		const aiFace& Face = paiMesh->mFaces[i];

	   	//Add the indicies faces
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	   }

	m_Entries[Index].Init(Vertices, Indices);
   }

//Calls the texture class wit the proper file path to read in the image
bool BasicMesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
   {
	// Extract the directory part from the file name
	std::string::size_type SlashIndex = Filename.find_last_of("/");
	std::string Dir;

	if (SlashIndex == std::string::npos)
	   {
		Dir = ".";
	   }
	else if (SlashIndex == 0)
	   {
		Dir = "/";
	   }
	else 
	   {
		Dir = Filename.substr(0, SlashIndex);
	   }

	bool Ret = true;

	// Initialize the materials
	for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++)
	   {
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		m_Textures[i] = NULL;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		   {
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			   {
				std::string FullPath = Dir + "/" + Path.data;
				m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());
				if (!m_Textures[i]->Load())
				   {
					printf("Error loading texture '%s'\n", FullPath.c_str());
					delete m_Textures[i];
					m_Textures[i] = NULL;
					Ret = false;
				   }
				else
				   {
					printf("Loaded texture '%s'\n", FullPath.c_str());
				   }
			   }
		   }

		// Load a white texture in case the model does not include its own texture
		if (!m_Textures[i])
		   {
			m_Textures[i] = new Texture(GL_TEXTURE_2D, "../data/default.jpg");
			Ret = m_Textures[i]->Load();
		   }
	   }

	return Ret;
   }

//Called to render the object to the screen
void BasicMesh::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  for (unsigned int i = 0 ; i < m_Entries.size() ; i++)
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BasicVertex), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(BasicVertex), (const GLvoid*)(offsetof(BasicVertex, m_tex)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(BasicVertex), (const GLvoid*)(offsetof(BasicVertex, m_normal)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);

    const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

    if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex])
    {
      m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
    }

    glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);
  }

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}
