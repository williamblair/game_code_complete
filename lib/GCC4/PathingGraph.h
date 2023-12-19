#ifndef GCC4_PATHING_GRAPH_H_INCLUDED
#define GCC4_PATHING_GRAPH_H_INCLUDED

#include "MemoryPool.h"
#include "GCCMath.h"

class PathingArc;
class PathingNode;
class PathPlanNode;
class AStar;

typedef std::list<PathingArc*> PathingArcList;
typedef std::list<PathingNode*> PathingNodeList;
typedef std::vector<PathingNode*> PathingNodeVec;
typedef std::map<PathingNode*,PathPlanNode*> PathingNodeToPathPlanMap;
typedef std::list<PathPlanNode*> PathPlanNodeList;

#define PATHING_DEFAULT_NODE_TOLERANCE (5.0f)
#define PATHING_DEFAULT_ARC_WEIGHT (1.0f)

/** A single node in the pathing graph */
class PathingNode
{
public:
    explicit PathingNode(
            const Vec3& pos,
            float tolerance = PATHING_DEFAULT_NODE_TOLERANCE) :
        m_pos(pos),
        m_tolerance(tolerance)
    {}

    const Vec3& GetPos() const { return m_pos; }
    float GetTolerance() const { return m_tolerance; }

    void AddArc(PathingArc* pArc);
    void GetNeighbors(PathingNodeList& outNeighbors);
    float GetCostFromNode(PathingNode* pFromNode);
    
private:
    float m_tolerance;
    Vec3 m_pos;
    PathingArcList m_arcs;

    PathingArc* FindArc(PathingNode* pLinkedNode);
    GCC_MEMORYPOOL_DECLARATION(0);
};

/** Arc that links two nodes, allowing travel between them */
class PathingArc
{
public:
private:
    float m_weight;
    PathingNode* m_pNodes[2];
};

#endif // GCC4_PATHING_GRAPH_H_INCLUDED

