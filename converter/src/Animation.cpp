#include "Animation.hpp"


bool Animation::Save(const std::string &path)
{

        std::string final_path =path+ this->name + ".anim";

        FileStream stream(final_path, "wb");
        if (!stream.IsOpen())
        {
            LogError("Cannot save %s", final_path.c_str());
            return false;
        }

       
    stream.Close();

    return true;

}

bool Animation::Load(const std::string &fileName)
{

    FileStream stream(fileName, "rb");
    if (!stream.IsOpen())
    {
        LogError("Cannot load %s", fileName.c_str());
        return false;
    }


    stream.Close();
    return true;
           
}


void Animation::Force()
{ 

    if (frames.size() == 0)
    {
        return;
    }



        for (u32 i = 0; i < frames.size(); i++)
        {
            Frame *b = frames[i];
            if (b->keys.numPositionKeys() > 0)
            {
                b->pos = true;
            } 

            if (b->keys.numRotationKeys() > 0)
            {
              b->rot = true;
            }
        }

    

}



 //****************************************************************************
void Animator::SaveAllFrames(const std::string &path)
{
    for (u32 i = 0; i < m_animations.size(); i++)
    {
        m_animations[i]->Save(path);
    }
}

Animation * Animator::LoadAnimation(const std::string &name)
{
    Animation *a = new Animation();
    if (a->Load(name))
    {
        
        m_animations.push_back(a);
        m_animations_map[a->name] = a;
        return a;
    }

    delete a;
    return nullptr;
}

 

Animation * Animator::AddAnimation(const std::string &name)
{
    Animation *a = new Animation();
    a->name = name; 

    for (size_t i = 0; i < entity->joints.size(); ++i) 
    {
        a->AddFrame(entity->joints[i]->name);
    }

    m_animations.push_back(a);
    m_animations_map[name] = a;
    return a;
}




//******************************************************************************

    aiNode *FindNode(const aiScene *scene, aiNode *node, char *name)
    {
        int i;
        if (!strcmp(name, node->mName.data))
            return node;
        for (i = 0; i < (int)node->mNumChildren; i++)
        {
            struct aiNode *found = FindNode(scene, node->mChildren[i], name);
            if (found)
                return found;
        }
        return NULL;
    }

    aiBone *FindBone(const aiScene *scene, const char *name)
    {
      
        for (int i = 0; i < (int)scene->mNumMeshes; i++)
        {
            aiMesh *paiMesh = scene->mMeshes[i];
            for (int x = 0; x < (int)paiMesh->mNumBones; x++)
            {
                aiBone *mBone = paiMesh->mBones[x];
                if (strcmp(name, mBone->mName.data) == 0)
                    return mBone;
            }
        }
        return NULL;
    }

    Surface *Entity::FindSurface(const char *name)
    {
        for (u32 i = 0; i < surfaces.size(); i++)
        {
            if (strcmp(surfaces[i]->name.c_str(), name) == 0)
                return surfaces[i];
        }
        return NULL;
    }

    void Entity::Scale(float sx, float sy, float sz)
    {
        Mat4 m = Mat4::Scale(sx, sy, sz);
        for (u32 i = 0; i < surfaces.size(); i++)
        {
            surfaces[i]->Transform(m);
        }
    }

    void Entity::Rotate( float eulerX,  float eulerY,  float eulerZ)
    {
        Quat rotation(ToRadians(eulerX), ToRadians(eulerY), ToRadians(eulerZ));
        Mat4 m(rotation);

        for (u32 i = 0; i < surfaces.size(); i++)
        {
            surfaces[i]->Transform(m);
        }
    }
    

    Mat4 aiMatrix4x4ToMat4(const aiMatrix4x4& m) 
    {
        Mat4 result;
        result.x[0] = m.a1;
        result.x[1] = m.b1;
        result.x[2] = m.c1;
        result.x[3] = m.d1;

        result.x[4] = m.a2;
        result.x[5] = m.b2;
        result.x[6] = m.c2;
        result.x[7] = m.d2;
        
        result.x[8] = m.a3;
        result.x[9] = m.b3;
        result.x[10] = m.c3;
        result.x[11] = m.d3;
        
        result.x[12] = m.a4;
        result.x[13] = m.b4;
        result.x[14] = m.c4;
        result.x[15] = m.d4;
        return result;

    }

   static void processMesh(const aiMesh* mesh, Entity *model, const aiMatrix4x4& transform) 
    {
        
        const char *name = mesh->mName.C_Str();
        Mat4 mat = aiMatrix4x4ToMat4(transform);

        Surface *surface = model->FindSurface(name);
        if (surface != NULL)
        {
            //Logger::Instance().Info("Transform Mesh: %s", name);
            surface->Transform(mat);
        } else 
        {
            Logger::Instance().Warning(" Mesh: %s not found", name);
        }

    }
 
   


static void processNode(const aiNode* node, const aiScene* scene, Entity *model) 
{
    aiMatrix4x4 nodeTransformation = node->mTransformation;

    for (unsigned int i = 0; i < node->mNumMeshes; ++i) 
    {
        const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, model, nodeTransformation);
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i) 
    {
        processNode(node->mChildren[i], scene, model);  // Recursão para filhos
    }
}


static std::string toLowerCase(const std::string& str) 
{
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return lowerStr;
}

    bool Entity::ImportStatic(const std::string &name, bool otimize,float scale)
    {
        const aiScene *g_scene = NULL;
        Assimp::Importer importer;
        isStatic = true;

        int flags = aiProcess_Triangulate;
        // flags |= aiProcess_JoinIdenticalVertices;
        // flags |= aiProcess_GenSmoothNormals;
        // flags |= aiProcess_GenUVCoords;
        // flags |= aiProcess_LimitBoneWeights;
        // flags|= aiProcess_SortByPType;
         flags |= aiProcess_FindInvalidData;
         flags |= aiProcess_RemoveRedundantMaterials;
	      flags|=aiProcess_OptimizeGraph;

         if(otimize)
         {
		 //flags|=aiProcess_OptimizeMeshes;
         }
	

        g_scene = importer.ReadFile(name.c_str(), flags);
        if (!g_scene)
        {
            Logger::Instance().Error("Fail load load mesh : %s, %s", name.c_str(), importer.GetErrorString());
            return false;
        }

        for (u32 i = 0; i < g_scene->mNumMaterials; i++)
        {
            aiMaterial *mat = g_scene->mMaterials[i];
            Mat *material = AddMaterial();

            std::string materialName = mat->GetName().C_Str();
            material->name =toLowerCase(materialName);
            Logger::Instance().Info("Load material %s", material->name.c_str());

            for (unsigned int i = 0; i < mat->GetTextureCount(aiTextureType_DIFFUSE); i++)
            {
                aiString str;
                mat->GetTexture(aiTextureType_DIFFUSE, i, &str);
                std::string name = str.C_Str();
                material->diffuse =  toLowerCase(name);
                Logger::Instance().Info("Material texture %s diffuse", material->diffuse.c_str());
            }
             for (unsigned int i = 0; i < mat->GetTextureCount(aiTextureType_SPECULAR); i++)
             {
                aiString str;
                mat->GetTexture(aiTextureType_SPECULAR, i, &str);
                std::string name = str.C_Str();
                material->specular =  toLowerCase(name);
                Logger::Instance().Info("Material texture %s specular", material->specular.c_str());
             }
               for (unsigned int i = 0; i < mat->GetTextureCount(aiTextureType_NORMALS); i++)
             {
                aiString str;
                mat->GetTexture(aiTextureType_NORMALS, i, &str);
                std::string name = str.C_Str();
                material->normal =  toLowerCase(name);

                Logger::Instance().Info("Material texture %s normal", material->normal.c_str());

             }
        }

        for (unsigned int i = 0; i < g_scene->mNumMeshes; i++)
        {
            aiMesh *mesh = g_scene->mMeshes[i];

            Surface *surface = AddSurface();
            
            std::string meshName = mesh->mName.C_Str();
            surface->name = meshName;
           // meshName.substr(0, 15);
            surface->material = mesh->mMaterialIndex;

           // Logger::Instance().Info("Load mesh %s material %d",  surface->name.c_str(), surface->material);
            for (unsigned int j = 0; j < mesh->mNumVertices; j++)
            {
                Vec3 pos;
                pos.x = mesh->mVertices[j].x ;
                pos.y = mesh->mVertices[j].y ;
                pos.z = mesh->mVertices[j].z ;
                Vec2 uv;
                if (mesh->HasTextureCoords(0))
                {
                    uv.x = mesh->mTextureCoords[0][j].x;
                    uv.y = mesh->mTextureCoords[0][j].y;
                }

                Vec3 normal;
                if (mesh->HasNormals())
                {
                    normal.x = mesh->mNormals[j].x;
                    normal.y = mesh->mNormals[j].y;
                    normal.z = mesh->mNormals[j].z;
                }

                surface->AddVertex(pos, uv, normal);
            }

            for (u32 j = 0; j < mesh->mNumFaces; j++)
            {
                aiFace *face = mesh->mFaces + j;
                int f0 = face->mIndices[0];
                int f1 = face->mIndices[1];
                int f2 = face->mIndices[2];
                surface->AddFace(f0, f1, f2);
            }
        }

         processNode(g_scene->mRootNode, g_scene, this);

        return true;
    }
    bool Entity::Import(const std::string &name)
    {
        const aiScene *g_scene = NULL;
        Assimp::Importer importer;
        isStatic = false;

        int flags = aiProcess_Triangulate;
        flags |= aiProcess_JoinIdenticalVertices;
        flags |= aiProcess_GenSmoothNormals;
        flags |= aiProcess_GenUVCoords;
        flags |= aiProcess_LimitBoneWeights;

        g_scene = importer.ReadFile(name.c_str(), flags);
        if (!g_scene)
        {
            Logger::Instance().Error("Fail load load mesh : %s, %s", name.c_str(), importer.GetErrorString());
            return false;
        }

        for (u32 i = 0; i < g_scene->mNumMaterials; i++)
        {
            aiMaterial *mat = g_scene->mMaterials[i];
            Logger::Instance().Info("Load material %s", mat->GetName().C_Str());
            Mat *material = AddMaterial();
            material->name = mat->GetName().C_Str();
            material->diffuse = "";

            for (unsigned int i = 0; i < mat->GetTextureCount(aiTextureType_DIFFUSE); i++)
            {
                aiString str;
                mat->GetTexture(aiTextureType_DIFFUSE, i, &str);
                material->diffuse = str.C_Str();
                Logger::Instance().Info("Material texture %s", str.C_Str());
                
            }
        }

        for (unsigned int i = 0; i < g_scene->mNumMeshes; i++)
        {
            Logger::Instance().Info("Load mesh %s", g_scene->mMeshes[i]->mName.C_Str());
            aiMesh *mesh = g_scene->mMeshes[i];

            Surface *surface = AddSurface();
            surface->name = g_scene->mMeshes[i]->mName.C_Str();
            surface->material = mesh->mMaterialIndex;

            for (unsigned int j = 0; j < mesh->mNumVertices; j++)
            {
                Vec3 pos;
                pos.x = mesh->mVertices[j].x;
                pos.y = mesh->mVertices[j].y;
                pos.z = mesh->mVertices[j].z;
                Vec2 uv;
                if (mesh->HasTextureCoords(0))
                {
                    uv.x = mesh->mTextureCoords[0][j].x;
                    uv.y = mesh->mTextureCoords[0][j].y;
                }

                Vec3 normal;
                if (mesh->HasNormals())
                {
                    normal.x = mesh->mNormals[j].x;
                    normal.y = mesh->mNormals[j].y;
                    normal.z = mesh->mNormals[j].z;
                }

                surface->AddVertex(pos, uv, normal);
            }

            for (u32 j = 0; j < mesh->mNumFaces; j++)
            {
                aiFace *face = mesh->mFaces + j;
                int f0 = face->mIndices[0];
                int f1 = face->mIndices[1];
                int f2 = face->mIndices[2];
                surface->AddFace(f0, f1, f2);
            }
        }

        if (g_scene->mRootNode)
        {
            ReadNodeHeirarchy(g_scene->mRootNode);

            

            for (unsigned int i = 0; i < g_scene->mNumMeshes; i++)
            {
                aiMesh *iMesh = g_scene->mMeshes[i];
                Surface *surface = surfaces[i];
                for (u32 boneIndex = 0; boneIndex < iMesh->mNumBones; ++boneIndex)
                {
                    aiBone *iBone = iMesh->mBones[boneIndex];

                    int jointIndex = GetJointIndex(iBone->mName.data);

                    Joint *joint = joints[jointIndex];

                    if (strcmp(iBone->mName.data, joint->name.c_str()) != 0)
                    {
                        Logger::Instance().Error("Bone %s != Joint %s", iBone->mName.data, joint->name.c_str());
                        DEBUG_BREAK_IF(0);
                    }

                    joint->offset = ConvertMatrix(iBone->mOffsetMatrix);

                    //   Logger::Instance().Info("Mesh %s - Bone %s  Joint %s",surface->name.c_str(),   iBone->mName.data,joint->name.c_str());

                    for (u32 weightIndex = 0; weightIndex < iBone->mNumWeights; ++weightIndex)

                    {
                        aiVertexWeight vw = iBone->mWeights[weightIndex];
                        int vertexID = vw.mVertexId;
                        float weight = vw.mWeight;

                        DEBUG_BREAK_IF(vertexID >= (int)surface->vertices.size());

                        SetVertexBoneData(surface->vertices[vertexID], jointIndex, weight);
                    }
                }
                
            }

            if (g_scene->HasAnimations())
            {

                const aiAnimation *anim = g_scene->mAnimations[0];

                double Duration = anim->mDuration;
                double TicksPerSecond = anim->mTicksPerSecond;
                if (TicksPerSecond == 0)
                {
                    TicksPerSecond = 30.0f;
                }

                std::string name = anim->mName.C_Str();
                if (name.empty())
                {
                    name = "idle";
                }

                if (name =="Armature|mixamo.com|Layer0")
                {
                    name = "default";
                }


                Animation *animation = animator->AddAnimation(name);
                animation->duration = Duration;
                animation->fps = TicksPerSecond;
                animation->n_frames = (int)(Duration);// * TicksPerSecond);

               

                Logger::Instance().Info("Animation (%s) Duration %f FramesPerSecond %f ",name.c_str(), Duration, TicksPerSecond);

                for (u32 i = 0; i < anim->mNumChannels; i++)
                {
                    aiNodeAnim *pNodeAnim = anim->mChannels[i];
                    //aiNode *node = FindNode(g_scene, g_scene->mRootNode, pNodeAnim->mNodeName.data);

                    // Logger::Instance().Info("Node %s",pNodeAnim->mNodeName.data);


                     Frame *frame = animation->GetFrame(pNodeAnim->mNodeName.data);
                     DEBUG_BREAK_IF(!frame);

                     frame->pos = (pNodeAnim->mNumPositionKeys > 0);
                     frame->rot = (pNodeAnim->mNumRotationKeys > 0);


                 

                    for (uint i = 0; i < pNodeAnim->mNumPositionKeys; i++)
                    {
                        aiVector3D Translation = pNodeAnim->mPositionKeys[i].mValue;

                        float time = pNodeAnim->mPositionKeys[i].mTime;

                       frame->keys.AddPositionKeyFrame(time, Vec3(Translation.x, Translation.y, Translation.z));

                   //     LogInfo("Position %f %f %f  time %f", Translation.x, Translation.y, Translation.z, time); 

                    }

                    for (uint i = 0; i < pNodeAnim->mNumRotationKeys; i++)
                    {
                        aiQuaternion Rotation = pNodeAnim->mRotationKeys[i].mValue;

                        float time = pNodeAnim->mRotationKeys[i].mTime;

                        frame->keys.AddRotationKeyFrame(time, Quat(Rotation.x, Rotation.y, Rotation.z, Rotation.w));
                    }
                    
                    frame->pos = (pNodeAnim->mNumPositionKeys > 0);
                    frame->rot = (pNodeAnim->mNumRotationKeys > 0);

                }
                animation->Force();
          
            }
        }

        return true;
    }

    bool Entity::ImportAnimation(const char *fileName, const std::string &name)
    {
        const aiScene *g_scene = NULL;
        Assimp::Importer importer;
        int flags = aiProcess_Triangulate;

        g_scene = importer.ReadFile(fileName, flags);
        if (!g_scene)
        {
            Logger::Instance().Error("Fail load load mesh : %s, %s", fileName, importer.GetErrorString());
            return false;
        }

        if (g_scene->HasAnimations())
        {
            const aiAnimation *anim = g_scene->mAnimations[0];

                double Duration = anim->mDuration;
                double TicksPerSecond = anim->mTicksPerSecond;
                if (TicksPerSecond == 0)
                {
                    TicksPerSecond = 30.0f;
                }

   
              


                 Animation *animation = animator->AddAnimation(name);
                 animation->duration = Duration;
                 animation->fps = TicksPerSecond;
                 animation->n_frames = (int)(Duration);// * TicksPerSecond);

                 

               

                Logger::Instance().Info("Animation (%s) Duration %f FramesPerSecond %f ",name.c_str(), Duration, TicksPerSecond);

                for (u32 i = 0; i < anim->mNumChannels; i++)
                {
                    aiNodeAnim *pNodeAnim = anim->mChannels[i];
                   // aiNode *node = FindNode(g_scene, g_scene->mRootNode, pNodeAnim->mNodeName.data);

                    // Logger::Instance().Info("Node %s",pNodeAnim->mNodeName.data);


                     Frame *frame = animation->GetFrame(pNodeAnim->mNodeName.data);
                     DEBUG_BREAK_IF(!frame);

                    
                    if (frame->name=="origin")
                    {
                        LogInfo("Frame %s   %d %d",frame->name.c_str(),pNodeAnim->mNumPositionKeys,pNodeAnim->mNumRotationKeys);
                    }


                   
         
                 

                    for (uint i = 0; i < pNodeAnim->mNumPositionKeys; i++)
                    {
                        aiVector3D Translation = pNodeAnim->mPositionKeys[i].mValue;

                        float time = pNodeAnim->mPositionKeys[i].mTime;

                        frame->keys.AddPositionKeyFrame(time, Vec3(Translation.x, Translation.y, Translation.z));

                   //     LogInfo("Position %f %f %f  time %f", Translation.x, Translation.y, Translation.z, time); 

                    }

                    for (uint i = 0; i < pNodeAnim->mNumRotationKeys; i++)
                    {
                        aiQuaternion Rotation = pNodeAnim->mRotationKeys[i].mValue;

                        float time = pNodeAnim->mRotationKeys[i].mTime;

                        frame->keys.AddRotationKeyFrame(time, Quat(Rotation.x, Rotation.y, Rotation.z, Rotation.w));
                    }
                    frame->pos = (pNodeAnim->mNumPositionKeys > 0);
                    frame->rot = (pNodeAnim->mNumRotationKeys > 0);

                }
                animation->Force();

             


        }

        return true;
    }



    void Entity::ReadNodeHeirarchy(const aiNode *pNode)
    {
        if (!pNode)
            return;
        std::string NodeName(pNode->mName.data);
        std::string ParentName = "none";

        if (strcmp(NodeName.c_str(), "Camera") == 0)
            return;
        if (strcmp(NodeName.c_str(), "Light") == 0)
            return;

        if (pNode->mParent != NULL)
        {
            ParentName = std::string(pNode->mParent->mName.data);
        }

        Joint *node = new Joint();
        node->name = NodeName;
        node->parentName = ParentName;

        node->LocalWorld = ConvertMatrix(pNode->mTransformation);

        aiVector3D Position;
		aiQuaternion Rotation;
        aiVector3D Scaling;

        

	    pNode->mTransformation.Decompose(Scaling, Rotation, Position);


        node->position= Vec3(Position.x, Position.y, Position.z);
        node->orientation = Quat(Rotation.x, Rotation.y, Rotation.z, Rotation.w);
        node->scale = Vec3(Scaling.x, Scaling.y, Scaling.z);

        node->parent = this;

        joints.push_back(node);

        for (u32 i = 0; i < joints.size(); i++)
        {
            if (joints[i]->name == node->parentName)
            {
                node->parent = joints[i];
                break;
            }
        }
        node->UpdateAbsolutePosition();
        node->Update();

        for (int i = 0; i < (int)pNode->mNumChildren; i++)
        {
            ReadNodeHeirarchy(pNode->mChildren[i]);
        }
    }


bool  Entity::Save(const std::string &name)
{


        FileStream stream(name, "wb");
        if (!stream.IsOpen())
        {
            LogError("Cannot save %s", name.c_str());
            return false;
        }
        if (surfaces.size()==0)
        {

            stream.Close();
            return false;
        }

 

        stream.Close();
        return true;
        

}

bool  Entity::LoadStatic(const std::string &name)
{

    FileStream stream(name, "rb");
    if (!stream.IsOpen())
    {
        LogError("Cannot load %s", name.c_str());
        return false;
    }


    stream.Close(); 

    return true;
}

struct VertexHash 
{
    std::size_t operator()(const Vertex &v) const 
    {
        return std::hash<float>()(v.pos.x) ^ std::hash<float>()(v.pos.y) ^ std::hash<float>()(v.pos.z) ^ std::hash<float>()(v.normal.x) ^ std::hash<float>()(v.normal.y) ^ std::hash<float>()(v.normal.z) ^ std::hash<float>()(v.uv.x) ^ std::hash<float>()(v.uv.y);
    }
};

static void CleanSurface(Surface *surface) 
{
     std::unordered_map<Vertex, int, VertexHash> uniqueVerticesMap;   
    std::vector<Vertex> newVertices;  
    std::vector<unsigned int> newFaces; 

    for (u32 i = 0; i < surface->faces.size(); i++)
    {
        Vertex v = surface->vertices[surface->faces[i]];
        if (uniqueVerticesMap.find(v) == uniqueVerticesMap.end())
        {
            uniqueVerticesMap[v] = newVertices.size();
            newVertices.push_back(v);
        }

        newFaces.push_back(uniqueVerticesMap[v]);
       
    }
    surface->vertices = std::move(newVertices);
    
    surface->faces = std::move(newFaces);
 

}


void Entity::CleanSurfaces() 
{
    for (u32 i = 0; i < surfaces.size(); i++)
    {
        CleanSurface(surfaces[i]);
    }
}

void Entity::CombineSurfaces()
{
    std::unordered_map<int,Surface*> combinedSurfaces;

    printf("Total %ld surfaces\n", surfaces.size());

    for (u32 i = 0; i < surfaces.size(); i++)
    {
        int material = surfaces[i]->material;

        if (combinedSurfaces.find(material) == combinedSurfaces.end())
        {
            Surface *surface = new Surface();
            surface->material = material;
            surface->name = surfaces[i]->name;

            combinedSurfaces[material] = surface;
        }

        Surface *surface = combinedSurfaces[material];
        int indexOffset = surface->vertices.size();

        for (u32 j = 0; j < surfaces[i]->vertices.size(); j++)
        {
            surface->AddVertex(surfaces[i]->vertices[j].pos,  surfaces[i]->vertices[j].uv,surfaces[i]->vertices[j].normal);
        }
        for (u32 j = 0; j < surfaces[i]->faces.size(); j++)
        {
            surface->faces.push_back(surfaces[i]->faces[j] + indexOffset);
        }

    }
    for (u32 i = 0; i < surfaces.size(); i++)
    {

        delete surfaces[i];
    } 
    surfaces.clear();
    for (auto it = combinedSurfaces.begin(); it != combinedSurfaces.end(); it++)
    {
        surfaces.push_back(it->second);
    }
    printf("Total %ld surfaces\n", surfaces.size());
}

bool  Entity::SaveStatic(const std::string &name)
{

    MeshSaver saver;

    return saver.SaveMesh(name, this);

    
        

}

bool Entity::Load(const std::string &name)
{

    FileStream stream(name, "rb");
    if (!stream.IsOpen())
    {
        LogError("Cannot load %s", name.c_str());
        return false;
    }


    stream.Close();

    return true;


}

MeshSaver::MeshSaver()
{

   // CurPos = 0;
   // memset(Stack, 0, sizeof(Stack));
}


MeshSaver::~MeshSaver()
{
}

bool MeshSaver::SaveMesh(const std::string &name, const Entity *entity)
{
    if (!stream.Create(name, true))
    {
        return false;
    }
    stream.WriteChar('H');
    stream.WriteChar('3');
    stream.WriteChar('D');
    stream.WriteChar('S');
    stream.WriteShort(5);
    for (unsigned int i = 0; i < entity->materials.size(); i++)
    {
        beginChunk("MAT#");
        beginChunk("TEX#");
        Mat *mat = entity->materials[i];
        if (mat->diffuse.size() > 0)
        {

            stream.WriteString(mat->diffuse.c_str());
        }
        if (mat->specular.size() > 0)
        {
            stream.WriteString(mat->specular.c_str());
        }
        if (mat->normal.size() > 0)
        {
            stream.WriteString(mat->normal.c_str());
        }
        endChunk();
        endChunk();
    }
    beginChunk("MESH");

        for (unsigned int i = 0; i < entity->surfaces.size(); i++)
        {
            beginChunk("SURF");
            Surface *surface = entity->surfaces[i];
            stream.WriteShort(surface->material);

            beginChunk("VRTS");
                for (unsigned int j = 0; j < surface->vertices.size(); j++)
                {
                    stream.WriteFloat(surface->vertices[j].pos.x);
                    stream.WriteFloat(surface->vertices[j].pos.y);
                    stream.WriteFloat(surface->vertices[j].pos.z);

                    stream.WriteFloat(surface->vertices[j].normal.x);
                    stream.WriteFloat(surface->vertices[j].normal.y);
                    stream.WriteFloat(surface->vertices[j].normal.z);

                    stream.WriteFloat(surface->vertices[j].uv.x);
                    stream.WriteFloat(surface->vertices[j].uv.y);
                    
                }
            endChunk();//end VRTS


        beginChunk("TRIS");
        for (unsigned int j = 0; j < (surface->faces.size()) / 3; j++) {
            stream.WriteInt(surface->faces[j * 3 + 0]);
            stream.WriteInt(surface->faces[j * 3 + 1]);
            stream.WriteInt(surface->faces[j * 3 + 2]);
        }
        endChunk(); // end TRIS
                        


            endChunk();//end SURF
        }

    endChunk();//end MESH
    stream.Close();
    return true;
}

void MeshSaver::beginChunk(const std::string &Tag)
{
    
    stream.WriteStringSize(Tag.c_str(), 4);
    Stack.push(stream.Cursor());
    stream.WriteInt(0);
}

void MeshSaver::endChunk()
{
    int Pos = stream.Cursor();
    
    int Start = Stack.top();
    Stack.pop();
    
    
    int Size = Pos - (Start + 4);
    
    stream.Seek(Start);
    
    stream.WriteInt(Size);
    
    stream.Seek(Pos);

   // std::cout << "Pos: " << Pos << ", Start: " << Start << ", Size: " << Size << std::endl;

    
}
