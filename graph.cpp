//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

typedef int NodeId;
const NodeId INVALID_NODE_ID = -1;

class Node {
public:
	Node();
	Node(NodeId);
	Node(NodeId id, int nodeDepth, NodeId parentId, bool rootNote, bool explored, bool inFrontier);
	~Node();

	NodeId getId() const {
		return id;
	}

	int getNodeDepth() const {
		return nodeDepth;
	}

	NodeId getParentId() const {
		return parentId;
	}

	bool getRootNode() const {
		return rootNote;
	}

	bool getExplored() const {
		return explored;
	}

	bool getInFrontier() const {
		return inFrontier;
	}

	void setId(NodeId id) { this->id = id; }
	void setNodeDepth(int nodeDepth) { this->nodeDepth = nodeDepth; }
	void setParentId(NodeId parentId) { this->parentId = parentId; }
	void setRootNode(bool rootNote) { this->rootNote = rootNote; }
	void setExplored(bool explored) { this->explored = explored; }
	void setInFrontier(bool inFrontier) { this->inFrontier = inFrontier; }

private:
	NodeId id;
	int nodeDepth;
	NodeId parentId;
	bool rootNote;
	bool explored;
	bool inFrontier;
	
	// Game specific members
};

//*************************************************************************************************************
//*************************************************************************************************************

Node::Node() :
	id(INVALID_NODE_ID),
	nodeDepth(INVALID_NODE_DEPTH),
	parentId(INVALID_NODE_ID),
	rootNote(false),
	explored(false),
	inFrontier(false)
{
	
}

//*************************************************************************************************************
//*************************************************************************************************************

Node::Node(NodeId id) :
	id(id),
	nodeDepth(INVALID_NODE_DEPTH),
	parentId(INVALID_ID),
	rootNote(false),
	explored(false),
	inFrontier(false),
	isGateway(false)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Node::Node(NodeId id, int nodeDepth, NodeId parentId, bool rootNote, bool explored, bool inFrontier) :
	id(id),
	nodeDepth(nodeDepth),
	parentId(parentId),
	rootNote(rootNote),
	explored(explored),
	inFrontier(inFrontier)
{
	
}

//*************************************************************************************************************
//*************************************************************************************************************

Node::~Node() {

}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

typedef vector<NodeId> ChildrenList;
typedef map<NodeId, ChildrenList> GraphMap;
typedef map<NodeId, Node*> IdNodeMap;
typedef vector<NodeId> NodeStack;
typedef deque<NodeId> NodeQueue;
typedef set<NodeId> NodeSet;

class Graph {
public:
	Graph();
	Graph(int nodesCount, bool undirected, GraphMap graph);
	~Graph();

	int getNodesCount() const {
		return nodesCount;
	}

	bool getUndirectedget() const {
		return undirected;
	}

	GraphMap getGraph() const {
		return graph;
	}

	IdNodeMap getIdNodeMap() const {
		return idNodeMap;
	}

	Node* getNode(NodeId nodeId) const {
		return idNodeMap.at(nodeId);
	}

	void setNodesCount(int nodesCount) { this->nodesCount = nodesCount; }
	void setUndirected(bool undirected) { this->undirected = undirected; }
	void setGraph(GraphMap graph) { this->graph = graph; }
	void setIdNodeMap(IdNodeMap idNodeMap) { this->idNodeMap = idNodeMap; }

	void addEdge(NodeId parentId, NodeId childId);

	/// Adds edge wiht alredy created nodes
	void addEdge(Node* parent, Node* child);

	/// Used when new child should be added to the graph
	void createNode(NodeId nodeId, int nodeDepth, NodeId parentId);

	void clear();
	bool nodeCreated(NodeId nodeId) const;
	void deleteAllNodes();
	vector<NodeId> treeRootsIds() const;
	void dfs(NodeId treeRootNodeId);
	void bfs(NodeId treeRootNodeId);
	int getMaxNodeDepth() const;
	bool edgeExists(NodeId parent, NodeId child) const;
	vector<NodeId> backtrack(NodeId from, NodeId to) const;
	NodeId getFirstNodeId() const;
	int depthOfTree(NodeId nodeId) const;
	int treeDiameter(NodeId nodeId) const;
	void graphResetAlgParams();

private:
	int nodesCount;
	bool undirected;
	GraphMap graph;

	// Map used to store all nodes, used to check if node is already created
	// and for easy accesss when deleteing memory pointed by each node
	IdNodeMap idNodeMap;

	/// Game specific members
};

//*************************************************************************************************************
//*************************************************************************************************************

Graph::Graph() :
	nodesCount(0),
	undirected(true),
	graph()
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Graph::Graph(int nodesCount, bool undirected, GraphMap graph) :
	nodesCount(nodesCount),
	undirected(undirected),
	graph(graph)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Graph::~Graph() {
	deleteAllNodes();
	graph.clear();
}

//*************************************************************************************************************
//*************************************************************************************************************

void Graph::deleteAllNodes() {
	for (IdNodeMap::iterator it = idNodeMap.begin(); it != idNodeMap.end(); ++it) {
		Node* node = it->second;

		if (node) {
			delete node;
			node = NULL;
		}
	}

	idNodeMap.clear();
	nodesCount = 0;
}

//*************************************************************************************************************
//*************************************************************************************************************

vector<NodeId> Graph::treeRootsIds() const {
	vector<NodeId> res;

	for (IdNodeMap::const_iterator nodeIt = idNodeMap.begin(); nodeIt != idNodeMap.end(); ++nodeIt) {
		NodeId nodeId = nodeIt->first;

		bool isChildOfANode = false;

		for (GraphMap::const_iterator graphIt = graph.begin(); graphIt != graph.end(); ++graphIt) {
			ChildrenList childrenList = graphIt->second;
			if (find(childrenList.begin(), childrenList.end(), nodeId) != childrenList.end()) {
				isChildOfANode = true;
				break;
			}
		}

		if (!isChildOfANode) {
			res.push_back(nodeId);
		}
	}

	return res;
}

//*************************************************************************************************************
//*************************************************************************************************************

void Graph::dfs(NodeId treeRootNodeId) {
	getNode(treeRootNodeId)->setNodeDepth(TREE_ROOT_NODE_DEPTH);

	NodeStack frontier;

	frontier.push_back(treeRootNodeId);
	idNodeMap[treeRootNodeId]->setInFrontier(true);

	while (!frontier.empty()) {
		NodeId state = frontier.back();
		frontier.pop_back();
		idNodeMap[state]->setInFrontier(false);

		idNodeMap[state]->setExplored(true);

		ChildrenList* children = &graph[state];
		for (size_t childIdx = 0; childIdx < children->size(); ++childIdx) {
			NodeId childId = children->at(childIdx);

			bool nodeExplored = idNodeMap[childId]->getExplored();
			bool nodeInFrontier = idNodeMap[childId]->getInFrontier();
			if (!nodeExplored && !nodeInFrontier) {
				frontier.push_back(childId);

				int parentDepth = idNodeMap[state]->getNodeDepth();
				idNodeMap[childId]->setNodeDepth(parentDepth + 1);
				idNodeMap[childId]->setParentId(state);
			}
		}
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

void Graph::bfs(NodeId treeRootNodeId) {
	getNode(treeRootNodeId)->setNodeDepth(TREE_ROOT_NODE_DEPTH);

	NodeQueue frontier;

	frontier.push_back(treeRootNodeId);
	idNodeMap[treeRootNodeId]->setInFrontier(true);

	while (!frontier.empty()) {
		NodeId state = frontier.front();
		frontier.pop_front();
		idNodeMap[state]->setInFrontier(false);

		idNodeMap[state]->setExplored(true);

		ChildrenList* children = &graph[state];
		for (size_t childIdx = 0; childIdx < children->size(); ++childIdx) {
			NodeId childId = children->at(childIdx);

			bool nodeExplored = idNodeMap[childId]->getExplored();
			bool nodeInFrontier = idNodeMap[childId]->getInFrontier();
			if (!nodeExplored && !nodeInFrontier) {
				frontier.push_back(childId);

				int parentDepth = idNodeMap[state]->getNodeDepth();
				idNodeMap[childId]->setNodeDepth(parentDepth + 1);
				idNodeMap[childId]->setParentId(state);
			}
		}
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

int Graph::getMaxNodeDepth() const {
	int maxNodeDepth = INVALID_NODE_DEPTH;

	for (IdNodeMap::const_iterator nodeIt = idNodeMap.begin(); nodeIt != idNodeMap.end(); ++nodeIt) {
		int nodeDepth = nodeIt->second->getNodeDepth();
		if (nodeDepth > maxNodeDepth) {
			maxNodeDepth = nodeDepth;
		}
	}

	return maxNodeDepth;
}

//*************************************************************************************************************
//*************************************************************************************************************

bool Graph::edgeExists(NodeId parent, NodeId child) const {
	bool res = false;

	if (nodeCreated(parent) && nodeCreated(child)) {
		ChildrenList children = graph.at(parent); // TODO: copying do not copy use * for children
		res = find(children.begin(), children.end(), child) != children.end();
	}

	return res;
}

//*************************************************************************************************************
//*************************************************************************************************************

vector<NodeId> Graph::backtrack(NodeId from, NodeId to) const {
	vector<NodeId> path;

	while (from != to) {
		path.push_back(from);
		from = idNodeMap.at(from)->getParentId();
	}

	path.push_back(to);

	reverse(path.begin(), path.end());

	return path;
}

//*************************************************************************************************************
//*************************************************************************************************************

NodeId Graph::getFirstNodeId() const {
	return idNodeMap.begin()->first;
}

//*************************************************************************************************************
//*************************************************************************************************************

int Graph::depthOfTree(NodeId nodeId) const {
	if (idNodeMap.at(nodeId)->getRootNode()) {
		return 0;
	}

	int maxdepth = 0;

	for (ChildrenList::const_iterator nodeIt = graph.at(nodeId).begin(); nodeIt != graph.at(nodeId).end(); ++nodeIt) {
		maxdepth = max(maxdepth, depthOfTree(*nodeIt));
	}

	return maxdepth + 1;
}

//*************************************************************************************************************
//*************************************************************************************************************

int Graph::treeDiameter(NodeId nodeId) const {
	if (idNodeMap.at(nodeId)->getRootNode()) {
		return 0;
	}

	int max1 = 0, max2 = 0;
	for (ChildrenList::const_iterator nodeIt = graph.at(nodeId).begin(); nodeIt != graph.at(nodeId).end(); ++nodeIt) {
		int h = depthOfTree(*nodeIt);
		if (h > max1) {
			max2 = max1;
			max1 = h;
		}
		else if (h > max2) {
			max2 = h;
		}
	}

	int maxChildDia = 0;
	for (ChildrenList::const_iterator nodeIt = graph.at(nodeId).begin(); nodeIt != graph.at(nodeId).end(); ++nodeIt) {
		maxChildDia = max(maxChildDia, treeDiameter(*nodeIt));
	}

	return max(maxChildDia, max1 + max2 + 1);
}

//*************************************************************************************************************
//*************************************************************************************************************

void Graph::graphResetAlgParams() {
	for (IdNodeMap::iterator nodeIt = idNodeMap.begin(); nodeIt != idNodeMap.end(); ++nodeIt) {
		nodeIt->second->setExplored(false);
		nodeIt->second->setInFrontier(false);
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

void Graph::addEdge(NodeId parentId, NodeId childId) {
	graph[parentId].push_back(childId);

	if (undirected) {
		graph[childId].push_back(parentId);
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

void Graph::addEdge(Node* parent, Node* child) {
	Node* nodes[PAIR] = { parent, child };

	for (int nodeIdx = 0; nodeIdx < PAIR; ++nodeIdx) {
		Node* node = nodes[nodeIdx];
		NodeId nodeId = node->getId();

		// May be an addNode(Node*) would be nice
		if (!nodeCreated(nodeId)) {
			idNodeMap[nodeId] = node;
			graph[nodeId];
		}
	}

	addEdge(parent->getId(), child->getId());
	child->setParentId(parent->getId());
}

//*************************************************************************************************************
//*************************************************************************************************************

void Graph::createNode(NodeId nodeId, int nodeDepth, NodeId parentId) {
	if (!nodeCreated(nodeId)) {
		Node* node = new Node(nodeId, nodeDepth, parentId, false, false, false);
		idNodeMap[nodeId] = node;
		graph[nodeId];
		++nodesCount;
	}
}

//*************************************************************************************************************
//*************************************************************************************************************

void Graph::clear() {
	deleteAllNodes();
	graph.clear();
}

//*************************************************************************************************************
//*************************************************************************************************************

bool Graph::nodeCreated(NodeId nodeId) const {
	return idNodeMap.end() != idNodeMap.find(nodeId);
}

//*************************************************************************************************************
//*************************************************************************************************************
