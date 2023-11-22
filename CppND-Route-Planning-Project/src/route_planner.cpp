#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
  	start_node = &m_Model.FindClosestNode(start_x, start_y); // Store Start Node
    end_node = &m_Model.FindClosestNode(end_x, end_y); // Store End Node
 
}


// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    
    return node -> distance(*end_node); // Calculate the distance between End Node and the node which is used

}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {

    current_node -> FindNeighbors();

    for (auto node : current_node -> neighbors){

        node -> parent = current_node; // set the parent
        node -> h_value = CalculateHValue(node); //calculate the h value for each neighbor
        node -> g_value = current_node -> g_value + node -> distance(*current_node); // calculate the g vale for each neighbor
        open_list.push_back(node); // add neighbor to the list
        node -> visited = true; // set the visited value as true
    }
}


// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

bool CompareValues(RouteModel::Node *x, RouteModel::Node *y){
  return (x -> g_value + x -> h_value) < (y -> g_value + y -> h_value); // Compare the f values (g+h)
}

RouteModel::Node *RoutePlanner::NextNode() {

    std::sort(open_list.begin(), open_list.end(), CompareValues); // sort the list in increasing order

    RouteModel::Node* ptr = open_list[0]; // Pointer to the node in the list with the lowest sum.
    open_list.erase(open_list.begin()); // Remove that node

    return ptr;
}


// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.
// Tips:
// - This method should take the current (final) node as an argument and iteratively follow the 
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    // TODO: Implement your solution here.

    while(current_node -> parent){ //iterate over parents till the first node
        path_found.push_back(*current_node); // add the current not to the vector
        distance += current_node -> distance(*current_node->parent); // Sum the distance
        current_node = current_node->parent; // Move to the next parent node
    }
    path_found.push_back(*current_node); // Add the first node 
    std::reverse(path_found.begin(), path_found.end()); // reverse the vector for correct order (Start Node ----> End node)


    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}


// TODO 7: Write the A* Search algorithm here.
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {
    RouteModel::Node *current_node = nullptr;

    // TODO: Implement your solution here.

    start_node -> visited = true; // Start from the start node

    open_list.push_back(start_node); // Add start node to the list

    while(true){ // Iterate over all nodes
        
        current_node = NextNode(); // go to the next node

        if(current_node == end_node){ // if the current node is the end node 

            m_Model.path = ConstructFinalPath(current_node); // Store the path
            break;
        }
        else{
            AddNeighbors(current_node); // if the current node iis not the end node continue adding neighbor nodes
        }
    }

}