#ifndef GCCSCENE_H_INCLUDED
#define GCCSCENE_H_INCLUDED

#include <map>
#include <memory>

#include <ISceneNode.h>
#include <ComponentTypes.h>

typedef std::map<ActorId,std::shared_ptr<ISceneNode>> SceneActorMap;

// forward declarations
class CameraNode;
class SkyNode;
class LightNode;
class LightManager;

class Scene
{
public:
protected:
    std::shared_ptr<SceneNode> m_Root;
};

#endif // GCCSCENE_H_INCLUDED
