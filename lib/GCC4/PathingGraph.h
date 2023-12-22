#ifndef GCC4_PATHING_GRAPH_H_INCLUDED
#define GCC4_PATHING_GRAPH_H_INCLUDED

#include <vector>
#include <map>
#include <list>
#include <cassert>

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
        m_tolerance(tolerance),
        m_pos(pos)
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
    explicit PathingArc(float weight = PATHING_DEFAULT_ARC_WEIGHT) :
        m_weight(weight)
    {}

    float GetWeight() const { return m_weight; }
    void LinkNodes(PathingNode* pNodeA, PathingNode* pNodeB);
    PathingNode* GetNeighbor(PathingNode* pMe);
private:
    float m_weight;
    PathingNode* m_pNodes[2];
};

/** A complete path, used to determine where to go */
class PathPlan
{
    friend class AStar;
public:
    PathPlan() :
        m_index(m_path.end())
    {}

    void ResetPath() { m_index = m_path.begin(); }
    const Vec3& GetCurrentNodePosition() const {
        assert(m_index != m_path.end());
        return (*m_index)->GetPos();
    }
    bool CheckForNextNode(const Vec3& pos);
    bool CheckForEnd();
private:
    PathingNodeList m_path;
    PathingNodeList::iterator m_index;

    void AddNode(PathingNode* pNode);
};

/**
 * Helper for PathingGraph::FindPath().
 * Tracks heuristic/cost data for a given node
 */
class PathPlanNode
{
public:
    explicit PathPlanNode(
        PathingNode* pNode,
        PathPlanNode* pPrevNode,
        PathingNode* pGoalNode
    );
    PathPlanNode* GetPrev() const { return m_pPrev; }
    PathingNode* GetPathingNode() const { return m_pPathingNode; }
    bool IsClosed() const { return m_closed; }
    float GetGoal() const { return m_goal; }
    float GetHeuristic() const { return m_heuristic; }
    float GetFitness() const { return m_fitness; }

    void UpdatePrevNode(PathPlanNode* pPrev);
    void SetClosed(bool toClose = true) { m_closed = toClose; }
    bool IsBetterChoiceThan(PathPlanNode* pRight) { return (m_fitness < pRight->GetFitness()); }
private:
    PathPlanNode* m_pPrev; // node we just came from
    PathingNode* m_pPathingNode; // pointer to the pathing node from the pathing graph
    PathingNode* m_pGoalNode; // pointer to the goal node
    bool m_closed; // the node is closed if it's already been processed
    float m_goal; // cost of the entire path up to this point (g)
    float m_heuristic; // estimated cost of this node to the goal (h)
    float m_fitness; // estimated cost from start to toal through this node (f)

    void UpdateHeuristics();
};

/** Implements the A* algorithm */
class AStar
{
public:
    AStar();
    ~AStar();

    void Destroy();
    PathPlan* operator()(PathingNode* pStartNode, PathingNode* pGoalNode);

private:
    PathingNodeToPathPlanMap m_nodes;
    PathingNode* m_pStartNode;
    PathingNode* m_pGoalNode;
    PathPlanNodeList m_openSet;

    PathPlanNode* AddToOpenSet(PathingNode* pNode, PathPlanNode* pPrevNode);
    void AddToClosedSet(PathPlanNode* pNode);
    void InsertNode(PathPlanNode* pNode);
    void ReinsertNode(PathPlanNode* pNode);
    PathPlan* RebuildPath(PathPlanNode* pGoalNode);
};

/** Main interface into the pathing system */
class PathingGraph
{
public:
    PathingGraph() {}
    ~PathingGraph() { DestroyGraph(); }
    void DestroyGraph();

    PathingNode* FindClosestNode(const Vec3& pos);
    PathingNode* FindFurthestNode(const Vec3& pos);
    PathingNode* FindRandomNode();
    PathPlan* FindPath(const Vec3& startPoint, const Vec3& endPoint);
    PathPlan* FindPath(const Vec3& startPoint, PathingNode* pGoalNode);
    PathPlan* FindPath(PathingNode* pStartNode, const Vec3& endPoint);
    PathPlan* FindPath(PathingNode* pStartNode, PathingNode* pGoalNode);

    // debug functions
    void BuildTestGraph();

private:
    PathingNodeVec m_nodes;
    PathingArcList m_arcs;

    void LinkNodes(PathingNode* pNodeA, PathingNode* pNodeB);
};

PathingGraph* CreatePathingGraph();

#endif // GCC4_PATHING_GRAPH_H_INCLUDED

