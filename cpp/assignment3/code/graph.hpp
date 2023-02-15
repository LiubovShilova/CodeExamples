#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <algorithm>
#include <iostream>
#include <list>
#include <stack>
#include <stdexcept>
#include <vector>
#include <unordered_set>

// an unweighted directed Graph whose vertex properties (e.g. int, Color,
// std::pair<int, Color>, etc.) can be passed as template parameter
// for simplicity vertices can only be added, but not removed

//PART OF THE FUNCTIONS (GET_VERTEX and NUM_VERTICES) ARE WRITTEN RIGHT WITH DEFINITION!!!

template <typename VertexProp>
class Graph {
public:

    class dfs_iterator;
    class const_dfs_iterator;
    class dfsu_iterator;
    class const_dfsu_iterator;

    // builds an empty graph
    Graph(){};

    // builds a graph with num_vertices unconnected vertices
    explicit Graph(std::size_t num_vertices);

    // adds a vertex and returns its index
    std::size_t add_vertex(VertexProp &&prop);

    // creates and edge between two vertices and returns if the creation was
    // successful (e.g. the edge could already exist)
    bool add_edge(const std::size_t v_idx1, const std::size_t v_idx2);

    // removes an edge between two vertices and returns if the removal was
    // successful
    bool remove_edge(const std::size_t v_idx1, const std::size_t v_idx2);

    // returns if there is an edge between v_idx1 and v_idx2
    bool has_edge(const std::size_t v_idx1, const std::size_t v_idx2) const;

    // returns if a directed path exists between two nodes
    // in case of v_idx1==v_idx2 return true iff v_ifx is part of a directed cycle
    bool path_exists(const std::size_t v_idx, const std::size_t v_idx2) const;

    // returns the properties for a given vertex index
    VertexProp &get_vertex(const std::size_t v_idx){
        if(v_idx>=adj_mat_.size()){
            throw std::out_of_range("get_vertex: indexes out of range, cannot check\n");
        }
        return vertex_props_[v_idx];
    };

    const VertexProp &get_vertex(const std::size_t v_idx) const{
        if(v_idx>=adj_mat_.size()){
            throw std::out_of_range("get_vertex: indexes out of range, cannot check\n");
        }
        return vertex_props_[v_idx];
    };

    // returns the number of vertices
    std::size_t num_vertices() const{
        return vertex_props_.size();
    };

    // returns the number of edges
    std::size_t num_edges() const;

    // returns the neighbors of a vertex
    std::list<std::size_t> get_neighbors(const std::size_t v_idx) const;

    // returns the neighbors of a vertex ignoring edge directions
    std::list<std::size_t> get_undirected_neighbors(
            const std::size_t v_idx) const;

    //returns the vector with the vertex indexes in order of DFS traversing
    std::vector<std::size_t> dfs(std::size_t vertex_idx){
        //check
        if(vertex_idx >= adj_mat_.size()){
            throw std::out_of_range("dfs_begin: indexes out of range\n");
        }

        //set of visited (to avoid infinite loops)
        std::unordered_set<std::size_t> visited;
        // vector of expanded nodes (the order we're looking for)
        std::vector<std::size_t> expanded;

        //because we need add/erase all the time - separate list, frontier
        std::stack<std::size_t> frontier;
        frontier.push(vertex_idx);
        expanded.push_back(vertex_idx);

        while(!frontier.empty())
        {
            std::size_t current = frontier.top();
            frontier.pop();
            if (visited.insert(current).second){
                expanded.push_back(current);
                for ( std::size_t i = 0; i < adj_mat_.size(); i++){
                    if (adj_mat_[current][i]) {
                        frontier.push(i); //tie breaker: expand from biggest index
                    }
                }
            }
        }
    };

    std::vector<std::size_t> dfsu(std::size_t vertex_idx){
        //check
        if(vertex_idx >= adj_mat_.size()){
            throw std::out_of_range("dfsu_begin: indexes out of range\n");
        }
        //set of visited (to avoid infinite loops)
        std::unordered_set<std::size_t> visited;
        // vector of expanded nodes (the order we're looking for)
        std::vector<std::size_t> expanded;

        //because we need add/erase all the time - separate list, frontier
        std::stack<std::size_t> frontier;
        frontier.push(vertex_idx);
        expanded.push_back(vertex_idx);

        //DFS
        while(!frontier.empty())
        {
            std::size_t current = frontier.top();
            frontier.pop();
            if (visited.insert(current).second){
                expanded.push_back(current);
                std::list neighbors = get_undirected_neighbors(current);
                for (std::size_t neighbor:neighbors){
                    frontier.push(neighbor); //tie breaker: expand from biggest index
                }
            }
        }
    };

    dfs_iterator dfs_begin(std::size_t vertex_idx){
        std::vector<std::size_t> expanded = dfs(vertex_idx);
        std::vector<std::size_t>::iterator it = begin(expanded);
        return it;
    };

    dfs_iterator dfs_end(std::size_t vertex_idx){
        std::vector<std::size_t> expanded = dfs(vertex_idx);
        std::vector<std::size_t>::iterator it = end(expanded);
        return it;
    };

    const_dfs_iterator dfs_begin(std::size_t vertex_idx) const{
        std::vector<std::size_t> expanded = dfs(vertex_idx);
        auto it = cbegin(expanded);
        return it;
    };

    const_dfs_iterator dfs_end(std::size_t vertex_idx) const{
        std::vector<std::size_t> expanded = dfs(vertex_idx);
        auto it = cend(expanded);
        return it;
    };

    dfsu_iterator dfsu_begin(std::size_t vertex_idx){
        std::vector<std::size_t> expanded = dfsu(vertex_idx);
        std::vector<std::size_t>::iterator it = begin(expanded);
        return it;
    };

    dfsu_iterator dfsu_end(std::size_t vertex_idx){
        std::vector<std::size_t> expanded = dfsu(vertex_idx);
        std::vector<std::size_t>::iterator it = end(expanded);
        return it;
    };

    const_dfsu_iterator dfsu_begin(
            std::size_t vertex_idx) const{
        std::vector<std::size_t> expanded = dfsu(vertex_idx);
        auto it = cbegin(expanded);
        return it;
    };

    const_dfsu_iterator dfsu_end(
            std::size_t vertex_idx) const{
        std::vector<std::size_t> expanded = dfsu(vertex_idx);
        auto it = cend(expanded);
        return it;
    };

private:
    // stores the neighbors of each vertex
    std::vector<std::vector<bool>> adj_mat_;
    // stores the vertex properties
    std::vector<VertexProp> vertex_props_;
};

template<typename VertexProp>
Graph<VertexProp>::Graph(std::size_t num_vertices) {
    //empty matrix
    for(unsigned int i = 0; i < num_vertices; i++ ){
        //int number = static_cast<int>(num_vertices);
        std::vector<bool> raw (num_vertices,false);
        adj_mat_.push_back(raw);

        //vertex: initialize with random values. TODO:
        VertexProp a; //or make a NULL, but hten won't work with string?
        vertex_props_.push_back(a);
    }
}


template<typename VertexProp>
std::size_t Graph<VertexProp>::add_vertex(VertexProp &&prop) {
    {
        std::vector<bool> raw (adj_mat_.size()+1,false);

        for(std::size_t i=0; i < adj_mat_.size(); i++){
            adj_mat_[i].push_back(false);
        }
        adj_mat_.push_back(raw);

        vertex_props_.push_back(prop);

        return (vertex_props_.size()-1);
    };

}

template<typename VertexProp>
bool Graph<VertexProp>::add_edge(const std::size_t v_idx1, const std::size_t v_idx2) {
    {

        if(v_idx1>= adj_mat_.size()||v_idx2>=adj_mat_.size()) {
            throw std::out_of_range("add_edge: indexes out of range\n");
        }
        else if(adj_mat_[v_idx1][v_idx2]){
            std::cerr<<"add_edge: the vertexes are already connected\n";
            return false;
        }
        else if (v_idx1 == v_idx2) {
            throw std::logic_error("add_edge: cannot connect a vertex with itself\n");

        }
        else{
            adj_mat_[v_idx1][v_idx2] = true;
            return true;
        }
    }
}

template<typename VertexProp>
bool Graph<VertexProp>::remove_edge(const std::size_t v_idx1, const std::size_t v_idx2) {
    {

        if(v_idx1 >= adj_mat_.size() || v_idx2 >= adj_mat_.size()){
            throw std::out_of_range("remove_edge: indexes out of range\n");
        }
        else if (v_idx1 == v_idx2) {
            throw std::logic_error("add_edge: cannot connect a vertex with itself\n");
        }
        else if(!adj_mat_[v_idx1][v_idx2]){
            std::cerr<<"remove_edge: the vertexes are alredy disconnected\n";
            return false;
        }
        else{
            adj_mat_[v_idx1][v_idx2] = false;
            return true;
        }
    }
}


template<typename VertexProp>
bool Graph<VertexProp>::has_edge(const std::size_t v_idx1, const std::size_t v_idx2) const{
    if((v_idx1>=adj_mat_.size())||(v_idx2>=adj_mat_.size())){
        throw std::out_of_range("has_edge: indexes out of range, cannot check\n");
    }
    return adj_mat_[v_idx1][v_idx2];
};

template<typename VertexProp>
bool Graph<VertexProp>::path_exists(const std::size_t v_idx, const std::size_t v_idx2) const {
    {
        //check
        if(v_idx >= adj_mat_.size() || v_idx2 >= adj_mat_.size()){
            throw std::logic_error("path_exists: indexes out of range, cannot check\n");
        }

        if (v_idx==v_idx2) return true;

        //BFS + keeping visited set for speed
        std::unordered_set<std::size_t> visited;

        std::list<std::size_t> frontier; //because we need add/erase all the time
        frontier.push_back(v_idx);

        while(!frontier.empty())
        {
            std::size_t current = frontier.front();
            frontier.pop_front();
            if (visited.insert(current).second){
                if (current == v_idx2) return true;
                for ( std::size_t i = 0; i < adj_mat_.size(); i++){
                    if (adj_mat_[current][i]) {
                        frontier.push_back(i);
                    }

                }
            }
        }
        return false;
    }
}

template<typename VertexProp>
std::size_t Graph<VertexProp>::num_edges() const {

    std::size_t n_edges = 0;
    for (const std::vector<bool>& raw:adj_mat_){
        for(const bool& el:raw){
            if (el) n_edges++;
        }
    }
    return n_edges;

}

template<typename VertexProp>
std::list<std::size_t> Graph<VertexProp>::get_neighbors(const std::size_t v_idx) const {
    {
        if(v_idx>=adj_mat_.size()){
            throw std::logic_error("path_exists: indexes out of range, cannot check\n");
        }

        std::list<std::size_t> neighbors;
        for (std::size_t i = 0; i<adj_mat_.size(); i++){
            //edges from v_idx
            if (adj_mat_[v_idx][i]) neighbors.push_back(i);
        }

        return  neighbors;
    }
}

template<typename VertexProp>
std::list<std::size_t> Graph<VertexProp>::get_undirected_neighbors(const std::size_t v_idx) const {

    if(v_idx>=adj_mat_.size()){
        std::cerr<<"cannot get neighbours of non-existent vertex\n";
    }
    std::list<std::size_t> neighbors;
    std::unordered_set<int> s;
    for (std::size_t i = 0; i<adj_mat_.size(); i++){
        //edges from v_idx
        if (adj_mat_[v_idx][i]) neighbors.push_back(vertex_props_[i]);

        //edges to v_idx
        if ((adj_mat_[i][v_idx])&&(s.insert(i).second)) neighbors.push_back(vertex_props_[i]);
    }
    return  neighbors;
}

#endif  // GRAPH_HPP
