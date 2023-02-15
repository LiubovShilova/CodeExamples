#include <algorithm>
#include <iostream>
#include <string>
#include <list>


#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "graph.hpp"



TEST_CASE("add_edge valid"){
    //non-empty graphs
    unsigned long int vertex_number = 5;
    Graph<std::string> g(vertex_number);
    Graph<int> g2(vertex_number);
    Graph<std::pair<int, std::string>> g3(vertex_number);
    REQUIRE(g.num_edges()==0);
    REQUIRE(g2.num_edges()==0);
    REQUIRE(g3.num_edges()==0);
    SECTION("adding edges to present vertexes"){
        unsigned long edges_num = 0;
        for (unsigned int i = 0; i<vertex_number; i++){
            for (unsigned j = 0; j<vertex_number; j++){
                if(i!=j){
                    g.add_edge(i,j);
                    g2.add_edge(i,j);
                    g3.add_edge(i,j);
                    edges_num++;
                    REQUIRE(g.add_edge(i,j)==false);
                    REQUIRE(g2.add_edge(i,j)==false);
                    REQUIRE(g3.add_edge(i,j)==false);
                    REQUIRE(g.num_edges() == edges_num);
                    REQUIRE(g2.num_edges() == edges_num);
                    REQUIRE(g3.num_edges() == edges_num);
                }
            }
        }
    }
}
TEST_CASE("add_edge invalid"){
    //initialization of empty graph of different types => cannot add the edge
    SECTION("adding edges in empty graph") {
        Graph<std::string> gr;
        Graph<std::string> gr_1(0);
        Graph<int> gr2;
        Graph<int> gr2_1(0);
        Graph<std::pair<int, std::string>> gr3;
        Graph<std::pair<int, std::string>> gr3_1(0);
        REQUIRE(gr.num_edges() == 0);
        REQUIRE_THROWS(gr.add_edge(0, 0));
        REQUIRE(gr_1.num_edges() == 0);
        REQUIRE_THROWS(gr_1.add_edge(0, 0));
        REQUIRE(gr2.num_edges() == 0);
        REQUIRE_THROWS(gr2.add_edge(0, 0));
        REQUIRE(gr2_1.num_edges() == 0);
        REQUIRE_THROWS(gr2_1.add_edge(0, 0));
        REQUIRE(gr3.num_edges() == 0);
        REQUIRE_THROWS(gr3.add_edge(0, 0));
        REQUIRE(gr3_1.num_edges() == 0);
        REQUIRE_THROWS(gr3_1.add_edge(0, 0));
    }

        //non-empty graphs
    SECTION("adding edges in non-empty graph") {
        Graph<std::string> g(5);
        Graph<int> g2(5);
        Graph<std::pair<int, std::string>> g3(5);
        REQUIRE(g.num_edges() == 0);
        REQUIRE(g2.num_edges() == 0);
        REQUIRE(g3.num_edges() == 0);
        REQUIRE_THROWS(g.add_edge(0, 5));
        REQUIRE_THROWS(g.add_edge(0, 5));
        REQUIRE_THROWS(g.add_edge(0, 5));
    }
}

TEST_CASE("remove_edge valid"){
    unsigned long int vertex_number = 5;
    Graph<std::string> g(vertex_number);
    Graph<int> g2(vertex_number);
    Graph<std::pair<int, std::string>> g3(vertex_number);
    unsigned long edges_num = 0;
    for (unsigned int i = 0; i<vertex_number; i++){
        for (unsigned j = 0; j<vertex_number; j++){
            if(i!=j){
                g.add_edge(i,j);
                g2.add_edge(i,j);
                g3.add_edge(i,j);
                edges_num++;
                REQUIRE(g.add_edge(i,j)==false);
                REQUIRE(g2.add_edge(i,j)==false);
                REQUIRE(g3.add_edge(i,j)==false);
                REQUIRE(g.num_edges() == edges_num);
                REQUIRE(g2.num_edges() == edges_num);
                REQUIRE(g3.num_edges() == edges_num);
            }
        }
    }
    for (unsigned int i = 0; i<vertex_number; i++){
        for (unsigned j = 0; j<vertex_number; j++){
            if(i!=j){
                g.remove_edge(i,j);
                g2.remove_edge(i,j);
                g3.remove_edge(i,j);
                edges_num--;
                REQUIRE(g.remove_edge(i,j)==false);
                REQUIRE(g2.remove_edge(i,j)==false);
                REQUIRE(g3.remove_edge(i,j)==false);
                REQUIRE(g.num_edges() == edges_num);
                REQUIRE(g2.num_edges() == edges_num);
                REQUIRE(g3.num_edges() == edges_num);
            }
        }
    }



}
TEST_CASE("remove_edge invalid"){
    SECTION("removing edges from an empty graph") {
        Graph<std::string> gr;
        Graph<std::string> gr_1(0);
        Graph<int> gr2;
        Graph<int> gr2_1(0);
        Graph<std::pair<int, std::string>> gr3;
        Graph<std::pair<int, std::string>> gr3_1(0);
        for (unsigned int i=0; i<4; i++){
            REQUIRE_THROWS(gr.remove_edge(i, 0) );
            REQUIRE_THROWS(gr.remove_edge(0, i) );
            REQUIRE_THROWS(gr.remove_edge(i, i) );
            REQUIRE_THROWS(gr_1.remove_edge(i, 0) );
            REQUIRE_THROWS(gr_1.remove_edge(i, i) );
            REQUIRE_THROWS(gr_1.remove_edge(0, i) );
            REQUIRE_THROWS(gr2.remove_edge(i, 0) );
            REQUIRE_THROWS(gr2.remove_edge(i, i) );
            REQUIRE_THROWS(gr2.remove_edge(0, i) );
            REQUIRE_THROWS(gr2_1.remove_edge(i, 0) );
            REQUIRE_THROWS(gr3.remove_edge(i, 0) );
            REQUIRE_THROWS(gr3_1.remove_edge(i, 0) );
        }
    }

    SECTION("removing edges from a non-empty graph") {
        unsigned long int number = 5;
        Graph<std::string> g(number);
        Graph<int> g2(number);
        Graph<std::pair<int, std::string>> g3(number);
        //the next value outside the vertex index range
        REQUIRE_THROWS(g.remove_edge(number,0));
        REQUIRE_THROWS(g2.remove_edge(number,0));
        REQUIRE_THROWS(g3.remove_edge(number,0));
        //no self loops
        REQUIRE_THROWS(g.remove_edge(0,0));
        REQUIRE_THROWS(g2.remove_edge(0,0));
        REQUIRE_THROWS(g3.remove_edge(0,0));
        //the edges are directed
        unsigned int a = 1;
        unsigned int b = 2;
        if(g.add_edge(a,b)){
            REQUIRE(g.remove_edge(a,b)==true);
            REQUIRE(g.remove_edge(b,a)==false);
        }
    }
}

TEST_CASE("add_vertex valid"){
    //empty
    Graph<std::string> gr;
    Graph<int> gr2;
    Graph<std::pair<int, std::string>> gr3;
    REQUIRE(gr.add_vertex("blue") == 0);
    REQUIRE(gr2.add_vertex(1) == 0);
    std::pair<int, std::string> p {1, "blue"};
    REQUIRE(gr3.add_vertex(std::move(p)) == 0);
    //non-empty
    unsigned long int vertex_number = 5;
    Graph<std::string> g(vertex_number);
    Graph<int> g2(vertex_number);
    Graph<std::pair<int, std::string>> g3(vertex_number);
    REQUIRE(g.add_vertex("blue") == vertex_number);
    REQUIRE(g.num_vertices()==vertex_number+1);
    REQUIRE(g2.add_vertex(1) == vertex_number);
    REQUIRE(g3.add_vertex(std::move(p)) == vertex_number);
}

TEST_CASE("add_vertex invalid"){
    //the function has no arguments, so I guess the checks above are all that we need
    //It can happen that there is no memory lft to add a new vertex,
    // but then the above checks should also catch it
}

TEST_CASE("get_neighbors valid") {
    //empty
    Graph<std::string> g(3);
    Graph<int> g2(3);
    Graph<std::pair<int, std::string>> g3(3);
    g.add_edge(0,1);
    g.add_edge(0,2);
    std::list<std::size_t> children {1,2};
    REQUIRE(g.get_neighbors(0)==children);
    REQUIRE(g.get_neighbors(1).empty());
}
TEST_CASE("get_neighbors invalid") {
    //empty
    Graph<std::string> gr;
    Graph<int> gr2;
    Graph<std::pair<int, std::string>> gr3;
    REQUIRE_THROWS(gr.get_neighbors(0));
    REQUIRE_THROWS(gr2.get_neighbors(0));
    REQUIRE_THROWS(gr3.get_neighbors(0));
    //not empty
    Graph<std::string> g(3);
    Graph<int> g2(3);
    Graph<std::pair<int, std::string>> g3(3);
    REQUIRE(g.get_neighbors(0).empty());
    REQUIRE(g2.get_neighbors(0).empty());
    REQUIRE(g3.get_neighbors(0).empty());
}

TEST_CASE("get_undirected_neighbors valid") {
    Graph<std::string> g(3);
    Graph<int> g2(3);
    Graph<std::pair<int, std::string>> g3(3);
    g.add_edge(0,1);
    g.add_edge(1,0);
    std::list<std::size_t> neigh1 {1};
    std::list<std::size_t> neigh2 {0};
    REQUIRE(g.get_neighbors(0)==neigh1);
    REQUIRE(g.get_neighbors(1)==neigh2);
}

TEST_CASE("get_undirected_neighbors invalid") {
    Graph<std::string> g(3);
    Graph<int> g2(3);
    Graph<std::pair<int, std::string>> g3(3);
    //out of range
    REQUIRE_THROWS(g.get_neighbors(3));
    REQUIRE_THROWS(g2.get_neighbors(3));
    REQUIRE_THROWS(g3.get_neighbors(3));
}

TEST_CASE("get_vertex valid"){
    Graph<std::string> gr;
    Graph<int> gr2;
    Graph<std::pair<int, std::string>> gr3;
    std::size_t idx = gr.add_vertex("blue");
    REQUIRE(gr.get_vertex(idx)=="blue");
    std::size_t idx2 = gr2.add_vertex(777);
    REQUIRE(gr2.get_vertex(idx2) == 777);
    std::pair<int, std::string> p {1, "blue"};
    std::size_t idx3 = gr3.add_vertex(std::move(p));
    REQUIRE(gr3.get_vertex(idx3) == p);
}

TEST_CASE("get_vertex invalid"){
    Graph<std::string> gr;
    Graph<int> gr2;
    Graph<std::pair<int, std::string>> gr3;
    std::size_t idx = gr.add_vertex("blue");
    std::size_t idx2 = gr2.add_vertex(777);
    std::pair<int, std::string> p {1, "blue"};
    std::size_t idx3 = gr3.add_vertex(std::move(p));
    REQUIRE_THROWS(gr.get_vertex(1));//out of range
    REQUIRE_THROWS(gr2.get_vertex(1));
    REQUIRE_THROWS(gr3.get_vertex(1));
}

TEST_CASE("has_edge valid"){
    unsigned long int vertex_number = 5; //Pick value>3 for valid checking
    Graph<std::string> g(vertex_number);
    Graph<int> g2(vertex_number);
    Graph<std::pair<int, std::string>> g3(vertex_number);

    g.add_edge(1,vertex_number-1);
    g2.add_edge(1,vertex_number-1);
    g3.add_edge(1,vertex_number-1);
    //there are edges here
    REQUIRE(g.has_edge(1,vertex_number-1)==true);
    REQUIRE(g2.has_edge(1,vertex_number-1)==true);
    REQUIRE(g3.has_edge(1,vertex_number-1)==true);
    //no edges present
    REQUIRE(g.has_edge(1,vertex_number-2)==false);
    REQUIRE(g2.has_edge(1,vertex_number-2)==false);
    REQUIRE(g3.has_edge(1,vertex_number-2)==false);
}
TEST_CASE("has_edge invalid"){
    unsigned long int vertex_number = 5; //Pick value>3 for valid checking
    Graph<std::string> g(vertex_number);
    Graph<int> g2(vertex_number);
    Graph<std::pair<int, std::string>> g3(vertex_number);
    g.add_edge(1,vertex_number-1);
    g2.add_edge(1,vertex_number-1);
    g3.add_edge(1,vertex_number-1);
    //out of range:
    REQUIRE_THROWS(g.has_edge(1,vertex_number));
    REQUIRE_THROWS(g2.has_edge(1,vertex_number));
    REQUIRE_THROWS(g3.has_edge(1,vertex_number));
    //no self-loops
    REQUIRE(g.has_edge(vertex_number-1,vertex_number-1)==false);
    REQUIRE(g2.has_edge(vertex_number-1,vertex_number-1)==false);
    REQUIRE(g3.has_edge(vertex_number-1,vertex_number-1)==false);
}

TEST_CASE("num_edges valid"){
    //checks are included in previous test cases (add/remove/is_edge)
}
TEST_CASE("num_edges invalid"){
    //checks are included in previous test cases (add/remove/is_edge)
}

TEST_CASE("num_vertices valid"){
    //checks are included in previous test cases (add vertex)
}
TEST_CASE("num_vertices invalid"){
    //checks are included in previous test cases (add vertex)
}

TEST_CASE("path_exists valid"){
    unsigned long int vertex_number = 5; //Pick value>4 for valid checking
    Graph<std::string> gr(vertex_number);

    //Path:a-b-c
    std::size_t a = 1;
    std::size_t b = 5;
    std::size_t c = 3;

    gr.add_edge(a,b);
    gr.add_edge(a,4);//random edges present in graph
    gr.add_edge(b,c);
    REQUIRE(gr.path_exists(a,c)==true);
    REQUIRE(gr.path_exists(c,a)==false);
}

TEST_CASE("path_exists invalid"){
    //empty vector
    Graph<std::string> g;
    REQUIRE_THROWS(g.path_exists(0,0));
    REQUIRE_THROWS(g.path_exists(0,1));

    unsigned long int vertex_number = 5; //Pick value>3 for valid checking
    Graph<std::string> gr(vertex_number);

    //Path:a-b-c
    std::size_t a = 1;
    std::size_t b = 5;
    std::size_t c = 3;

    gr.add_edge(a,b);
    gr.add_edge(a,4);//random edges present in graph
    gr.add_edge(b,c);
    REQUIRE_THROWS(gr.path_exists(a,vertex_number));
    REQUIRE_THROWS(gr.path_exists(vertex_number, a));
}

