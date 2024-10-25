#pragma once
#include "pch.h"

#include <assimp/Importer.hpp> //OO version Header!
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/vector3.h>
#include <assimp/matrix3x3.h>
#include <assimp/matrix4x4.h>

inline Vec3 VectorInterpolate(const Vec3 &A, const Vec3 &B, const float S)
{
    Vec3 result;
    result.x = (A.x * S) + (B.x * (1.0f - S));
    result.y = (A.y * S) + (B.y * (1.0f - S));
    result.z = (A.z * S) + (B.z * (1.0f - S));
    return result;
}

inline Mat4 ConvertMatrix(const aiMatrix4x4 &from)
{
    Mat4 to;
    // the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
    to.c[0][0] = from.a1;
    to.c[1][0] = from.a2;
    to.c[2][0] = from.a3;
    to.c[3][0] = from.a4;
    to.c[0][1] = from.b1;
    to.c[1][1] = from.b2;
    to.c[2][1] = from.b3;
    to.c[3][1] = from.b4;
    to.c[0][2] = from.c1;
    to.c[1][2] = from.c2;
    to.c[2][2] = from.c3;
    to.c[3][2] = from.c4;
    to.c[0][3] = from.d1;
    to.c[1][3] = from.d2;
    to.c[2][3] = from.d3;
    to.c[3][3] = from.d4;
    return to;
}

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
    Vec3 pos;
    Vec3 normal;
    Vec2 uv;
    int bones[MAX_BONE_INFLUENCE];
    float weights[MAX_BONE_INFLUENCE];
    bool operator==(const Vertex &other) const
    {
        if (pos != other.pos || normal != other.normal || uv != other.uv)
            return false;
        for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
        {
            if (bones[i] != other.bones[i] || weights[i] != other.weights[i])
                return false;
        }
        return true;
    }
};

class Node;
class Joint;
class KeyFrame;
class Animator;
class Entity;

inline void SetVertexBoneData(Vertex &vertex, int boneID, float weight)
{
    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
    {
        if (vertex.bones[i] < 0)
        {
            vertex.weights[i] = weight;
            vertex.bones[i] = boneID;
            break;
        }
    }
}

struct PosKeyFrame
{
    Vec3 pos;
    float frame;

    PosKeyFrame(const Vec3 &p, float f)
    {
        pos.set(p.x, p.y, p.z);
        frame = f;
    }
};

struct RotKeyFrame
{
    Quat rot;
    float frame;
    RotKeyFrame(const Quat &r, float f)
    {
        rot.set(r.x, r.y, r.z, r.w);
        frame = f;
    }
};

struct Mat
{
    std::string name;
    std::string diffuse;
    std::string specular;
    std::string normal;

    Mat()
    {

        name = "";
        diffuse = "";
        specular = "";
        normal = "";
    }
};

struct Surface
{
    std::string name;
    std::vector<unsigned int> faces;
    std::vector<Vertex> vertices;
    int material;

    const Vec3 GetVertex(int id) { return vertices[id].pos; }
    void SetVertex(int id, const Vec3 &v) { vertices[id].pos = v; }
    void SetVertex(int id, float x, float y, float z) { vertices[id].pos.set(x, y, z); }

    void AddVertex(const Vertex &vertex)
    {

        vertices.push_back(vertex);
    }

    void AddVertex(const Vec3 &v)
    {
        Vertex vertex;
        vertex.pos = v;

        vertex.weights[0] = 0.0f;
        vertex.bones[0] = -1;

        vertex.weights[1] = 0.0f;
        vertex.bones[1] = -1;

        vertex.weights[2] = 1.0f;
        vertex.bones[2] = -1;

        vertex.weights[3] = 0.0f;
        vertex.bones[3] = -1;

        vertices.push_back(vertex);
    }

    void AddVertex(const Vec3 &v, const Vec2 &uv)
    {
        Vertex vertex;
        vertex.pos = v;
        vertex.uv = uv;

        vertex.weights[0] = 0.0f;
        vertex.bones[0] = -1;

        vertex.weights[1] = 0.0f;
        vertex.bones[1] = -1;

        vertex.weights[2] = 1.0f;
        vertex.bones[2] = -1;

        vertex.weights[3] = 0.0f;
        vertex.bones[3] = -1;

        vertices.push_back(vertex);
    }

    void AddVertex(const Vec3 &v, const Vec2 &uv, const Vec3 &normal)
    {
        Vertex vertex;
        vertex.pos = v;
        vertex.uv = uv;
        vertex.normal = normal;

        vertex.weights[0] = 0.0f;
        vertex.bones[0] = -1;

        vertex.weights[1] = 0.0f;
        vertex.bones[1] = -1;

        vertex.weights[2] = 1.0f;
        vertex.bones[2] = -1;

        vertex.weights[3] = 0.0f;
        vertex.bones[3] = -1;

        vertices.push_back(vertex);
    }

    void AddFace(int a, int b, int c)
    {
        faces.push_back(a);
        faces.push_back(b);
        faces.push_back(c);
    }

    void AddIndex(int a)
    {
        faces.push_back(a);
    }

    void Transform(const Mat4 &mat)
    {
        for (int i = 0; i < vertices.size(); i++)
        {
            // Mat4::Transform(mat, vertices[i].pos);
            vertices[i].pos = Mat4::Transform(mat, vertices[i].pos);
        }
    }
};

struct Node
{

    Vec3 position;
    Vec3 scale;
    Quat orientation;

    Mat4 AbsoluteTransformation;
    Mat4 LocalWorld;

    Node *parent;

    std::string name;
    std::string parentName;

    Node()
    {
        name = "";
        parent = nullptr;
        position.set(0, 0, 0);
        scale.set(1, 1, 1);
        orientation.identity();
        AbsoluteTransformation.identity();
    }
    virtual ~Node()
    {
    }

    const Mat4 GetRelativeTransformation()
    {

        LocalWorld = Mat4::Scale(scale.x, scale.y, scale.z);
        LocalWorld.rotate(orientation);
        LocalWorld.translate(position.x, position.y, position.z);

        if (parent != nullptr)
        {
            Mat4 m_absTrans;
            Mat4::fastMult43(m_absTrans, parent->AbsoluteTransformation, LocalWorld);
            return m_absTrans;
        }
        else
        {
            return LocalWorld;
        }
    }

    void UpdateAbsolutePosition()
    {

        AbsoluteTransformation = GetRelativeTransformation();
    }

    virtual void Update()
    {
        UpdateAbsolutePosition();
    }

    virtual void Release()
    {
    }
};

struct Joint : Node
{

    Mat4 offset;

    Joint() : Node()
    {
    }

    ~Joint()
    {
    }

    void setLocalPosition(const Vec3 &p)
    {
        position = p;
    }

    void setLocalRotation(const Quat &q)
    {
        orientation = q;
    }

    Vec3 getLocalPosition()
    {
        return position;
    }

    Quat getLocalRotation()
    {
        return orientation;
    }
    Vec3 getWorldPosition()
    {

        return Mat4::Transform(AbsoluteTransformation, position);
    }
};
struct KeyFrame
{

    std::vector<PosKeyFrame> positionKeyFrames;
    std::vector<RotKeyFrame> rotationKeyFrames;

    KeyFrame()
    {
    }

    KeyFrame(KeyFrame *t)
    {
        if ((!t) || (t->numPositionKeys() == 0) || (t->numRotationKeys() == 0))
        {

            LogError("Null animation pointer or nor frames");
            return;
        }

        for (u32 i = 0; i < t->numPositionKeys(); i++)
        {
            positionKeyFrames.push_back(t->positionKeyFrames[i]);
        }

        for (u32 i = 0; i < t->numRotationKeys(); i++)
        {
            rotationKeyFrames.push_back(t->rotationKeyFrames[i]);
        }
    }

    ~KeyFrame()
    {
    }

    void AddPositionKeyFrame(float frame, const Vec3 &pos)
    {
        positionKeyFrames.push_back(PosKeyFrame(pos, frame));
    }

    void AddRotationKeyFrame(float frame, const Quat &rot)
    {

        rotationKeyFrames.push_back(RotKeyFrame(rot, frame));
    }

    int GetPositionIndex(float animationTime)
    {
        for (u32 index = 0; index < positionKeyFrames.size() - 1; ++index)
        {
            if (animationTime < positionKeyFrames[index + 1].frame)
                return index;
        }
        // SDL_Log("KeyFrame time %f  %ld",animationTime,positionKeyFrames.size());
        DEBUG_BREAK_IF(true);
    }

    int GetRotationIndex(float animationTime)
    {
        for (u32 index = 0; index < rotationKeyFrames.size() - 1; ++index)
        {
            if (animationTime < rotationKeyFrames[index + 1].frame)
                return index;
        }
        // SDL_Log("KeyFrame time %f  %ld",animationTime,rotationKeyFrames.size());
        DEBUG_BREAK_IF(true);
    }

    float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
    {
        float scaleFactor = 0.0f;
        float midWayLength = animationTime - lastTimeStamp;
        float framesDiff = nextTimeStamp - lastTimeStamp;
        scaleFactor = midWayLength / framesDiff;
        return scaleFactor;
    }

    Quat AnimateRotation(float movetime)
    {
        if (rotationKeyFrames.size() == 1)
        {
            return rotationKeyFrames[0].rot;
        }
        int currentIndex = GetRotationIndex(movetime);
        int nextIndex = currentIndex + 1;

        float factor = GetScaleFactor(rotationKeyFrames[currentIndex].frame, rotationKeyFrames[nextIndex].frame, movetime);

        //    SDL_Log(" %f %f %f",rotationKeyFrames[currentIndex].frame, rotationKeyFrames[nextIndex].frame,movetime);

        return Quat::Slerp(rotationKeyFrames[currentIndex].rot, rotationKeyFrames[nextIndex].rot, factor);
    }
    Vec3 AnimatePosition(float movetime)
    {
        if (positionKeyFrames.size() == 1)
        {
            return positionKeyFrames[0].pos;
        }
        int currentIndex = GetPositionIndex(movetime);
        int nextIndex = currentIndex + 1;

        float factor = GetScaleFactor(positionKeyFrames[currentIndex].frame, positionKeyFrames[nextIndex].frame, movetime);
        return Vec3::Lerp(positionKeyFrames[currentIndex].pos, positionKeyFrames[nextIndex].pos, factor);
    }

    u32 numRotationKeys() const { return rotationKeyFrames.size(); }
    u32 numPositionKeys() const { return positionKeyFrames.size(); }

    void setPositionKey(int frame, const Vec3 &p)
    {
        positionKeyFrames[frame].pos = p;
    }

    void setRotationKey(int frame, const Quat &q)
    {
        rotationKeyFrames[frame].rot = q;
    }
};

struct Frame
{
    std::string name;
    Vec3 position;
    Quat orientation;
    KeyFrame keys;
    Vec3 src_pos, dest_pos;
    Quat src_rot, dest_rot;
    bool pos;
    bool rot;
    bool IgnorePosition;
    bool IgnoreRotation;
    Frame()
    {
        pos = false;
        rot = false;
        IgnorePosition = false;
        IgnoreRotation = false;
    }
};

struct Animation
{
    enum
    {
        LOOP = 0,
        PINGPONG = 1,
        ONESHOT = 2
    };
    enum
    {
        Stoped = 0,
        Looping = 1,
        Playing = 2
    };
    std::string name;
    std::vector<Frame *> frames;
    std::map<std::string, Frame *> framesMap;
    u64 n_frames;
    u64 state;
    u64 method;
    float currentTime;
    float duration;
    float fps;
    int mode;
    bool isEnd;

    Animation()
    {
        state = Stoped;
        method = 0;
        currentTime = 0.0f;
        fps = 30.0f;
        mode = LOOP;
        isEnd = false;
    }

    void Force();

    bool Save(const std::string &name);
    bool Load(const std::string &name);

    Frame *AddFrame(std::string name)
    {
        Frame *frame = new Frame();
        frame->name = name;
        frames.push_back(frame);
        framesMap[name] = frame;
        return frame;
    }

    ~Animation()
    {
        for (u32 i = 0; i < frames.size(); i++)
        {
            delete frames[i];
        }
        frames.clear();
    }

    Frame *GetFrame(std::string name)
    {
        if (framesMap.find(name) == framesMap.end())
        {
            return NULL;
        }
        return framesMap[name];
    }

    Frame *GetFrame(int index)
    {
        if (index < 0 || index >= (int)frames.size())
        {
            return NULL;
        }
        return frames[index];
    }

    void Update(float elapsed);
};

struct Animator
{

    Animator(Entity *parent)
    {
        entity = parent;
    }

    ~Animator()
    {
        for (u32 i = 0; i < m_animations.size(); i++)
        {
            delete m_animations[i];
        }
    }

    void Update(float elapsed);

    void SaveAllFrames(const std::string &path);

    Animation *LoadAnimation(const std::string &name);

    Animation *AddAnimation(const std::string &name);

    Animation *GetAnimation(const std::string &name)
    {
        if (m_animations_map.find(name) == m_animations_map.end())
        {
            return NULL;
        }
        return m_animations_map[name];
    }

    Animation *GetAnimation(int index)
    {
        if (index < 0 || index >= (int)m_animations.size())
        {
            return NULL;
        }
        return m_animations[index];
    }

    u32 numAnimations() const { return m_animations.size(); }

private:
    std::vector<Animation *> m_animations;
    std::map<std::string, Animation *> m_animations_map;
    Entity *entity;
};

struct Entity : Node
{
    std::vector<Joint *> joints;
    std::vector<Surface *> surfaces;
    std::vector<Mat *> materials;
    std::vector<Mat4> bones;

    Animator *animator;

    bool isStatic;

    Entity()
    {

        animator = new Animator(this);

        bones.reserve(200);

        for (int i = 0; i < 200; i++)
        {
            bones.push_back(Mat4::Identity());
        }
    }
    ~Entity()
    {
        Release();
    }

    void UpdateAnimation(float dt)
    {

        //   SDL_Log("CurrentTime : %f Duration : %f TicksPerSecond : %f DeltaTime : %f",m_CurrentTime,m_Duration,m_TicksPerSecond,m_DeltaTime);
        animator->Update(dt);
        for (u32 i = 0; i < joints.size(); i++)
        {

            Joint *b = joints[i];
            b->Update();
            Mat4::fastMult43(bones[i], b->AbsoluteTransformation, b->offset);
        }
    }

    Mat *AddMaterial()
    {
        Mat *material = new Mat();
        materials.push_back(material);
        return material;
    }

    void AddJoint(Joint *joint)
    {
        joints.push_back(joint);
    }

    Surface *AddSurface()
    {
        Surface *surface = new Surface();
        surfaces.push_back(surface);
        return surface;
    }

    Joint *GetJoint(int id) { return joints[id]; }
    Joint *FindJoint(const char *name)
    {
        for (u32 i = 0; i < joints.size(); i++)
        {
            if (strcmp(joints[i]->name.c_str(), name) == 0)
            {
                return joints[i];
            }
        }
        return nullptr;
    }
    int GetJointIndex(const char *name)
    {
        for (u32 i = 0; i < joints.size(); i++)
        {
            if (strcmp(joints[i]->name.c_str(), name) == 0)
            {
                return i;
            }
        }
        return -1;
    }

    void Update() override
    {
        Node::Update();
    }
    void Release() override
    {
        Node::Release();

        if (animator != nullptr)
        {
            delete animator;
            animator = nullptr;
        }

        for (u32 i = 0; i < materials.size(); i++)
        {

            delete materials[i];
        }
        for (u32 i = 0; i < joints.size(); i++)
        {
            joints[i]->Release();
            delete joints[i];
        }

        for (u32 i = 0; i < surfaces.size(); i++)
        {

            delete surfaces[i];
        }
    }

    Surface *GetSurface(int id) { return surfaces[id]; }
    Surface *FindSurface(const char *name);

    void Scale(float sx, float sy, float sz);
    void Rotate(float eulerX, float eulerY, float eulerZ);

    bool ImportStatic(const std::string &name, bool otimize, float scale = 1.0f);
    bool Import(const std::string &name);
    bool ImportAnimation(const char *fileName, const std::string &name);
    void ReadNodeHeirarchy(const aiNode *pNode);

    bool Save(const std::string &name);
    bool SaveStatic(const std::string &name);

    bool Load(const std::string &name);
    bool LoadStatic(const std::string &name);

    void CombineSurfaces();
    void CleanSurfaces();
};

class MeshSaver
{
public:
    MeshSaver();
    ~MeshSaver();
    bool SaveMesh(const std::string &name, const Entity *entity);

private:
    std::stack<u64> Stack;
    void beginChunk(const std::string &Tag);
    void endChunk();

    FileStream stream;
};