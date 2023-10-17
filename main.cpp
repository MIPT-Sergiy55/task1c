//
// Created by sergiy on 17.10.23.
//

#include <vector>
#include <iostream>
#include <set>
#include <map>
#include <bitset>
#include <ctime>
#include <chrono>
#include <random>
#include <iterator>

class Graph {
    static const int MAX_SIZE = 10000;
    static const int MAX_WORK_TIME = 10; // in seconds
    int size = 0;
    std::vector<std::vector<int>> edges;
    std::vector<std::bitset<MAX_SIZE>> matrix;
    std::bitset<MAX_SIZE> leafs;
public:
    Graph(int sz, std::vector<std::vector<int>>& e)
    {
        size = sz;
        edges = e;
        matrix = std::vector<std::bitset<MAX_SIZE>>(size, 0);
        for (int i = 0; i < e.size(); ++i)
        {
            matrix[i][i] = 1;
            for (int j = 0; j < e[i].size(); ++j)
            {
                matrix[i][edges[i][j]] = 1;
            }
        }
        leafs = std::bitset<MAX_SIZE>(0);
        for (int i = 0; i < size; ++i)
        {
            // also has disconnected vertexes
            leafs[i] = (edges[i].size() <= 1);
        }
    }

    void print()
    {
        std::cout << "size: " << size << '\n';
        std::cout << "\nedges: \n";
        for (int i = 0; i < edges.size(); ++i)
        {
            std::cout << i << ": ";
            for (int j: edges[i])
                std::cout << j << ", ";
            std::cout << '\n';
        }
        std::cout << "\nmatrix: \n";
        for (int i = 0; i < size; ++i)
        {
            for (int j = 0; j < size; ++j)
                std::cout << matrix[i][j] << ",";
            std::cout << '\n';
        }
    }

    std::vector<int> best_solution() {
        auto ans = std::vector<int>();
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        for (int i = 0; i < size; ++i)
        {
            ans.push_back(i);
        }
        for (int i = 0; i < 10000; ++i) {
            auto other = solve();
            if (other.size() < ans.size())
            {
                ans = other;
            }
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() >= MAX_WORK_TIME)
                break;
        }
        return ans;
    }
private:
    std::vector<int> solve() {
        std::bitset<MAX_SIZE> infected;
        std::bitset<MAX_SIZE> marked = leafs;
        for (int i = 0; i < size; ++i)
            infected[i] = 1;

        iteration(infected, marked);
        std::vector<int> result;
        for (int i = 0; i < size; ++i)
            if (infected[i])
                result.push_back(i);
        return result;
    }


    void iteration(std::bitset<MAX_SIZE>& infected, std::bitset<MAX_SIZE>& marked)
    {
        std::vector<int> cities(size);
        for (int i = 0; i < size; ++i)
            cities[i] = i;
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(cities.begin(), cities.end(), g);
        for (int k: cities) {
            if (marked[k])
                continue;
            if(try_to_remove(infected, k))
                infected[k] = false;
        }
    }

    bool try_to_remove(std::bitset<MAX_SIZE>& infected, int index)
    {
        return (infected & matrix[index]).count() >= 3;
        // >= 3 because index is infected and matrix[index][index] = 1
    }

};

int main()
{
    int m, n = 0;
    std::cin >> m;
    std::vector<std::pair<int, int>> input_edges(m);
    std::set<int> nodes;
    for (int i = 0; i < m; ++i)
    {
        std::cin >> input_edges[i].first >> input_edges[i].second;
        nodes.insert(input_edges[i].first);
        nodes.insert(input_edges[i].second);
    }
    std::map<int, int> city_name_to_number;
    std::vector<int> cities;
    for (auto node: nodes)
    {
        cities.push_back(node);
        city_name_to_number[node] = n++;
    }
    std::vector<std::vector<int>> edges(n);
    for (int i = 0; i < m; ++i)
    {
        edges[city_name_to_number[input_edges[i].first]].push_back(city_name_to_number[input_edges[i].second]);
        edges[city_name_to_number[input_edges[i].second]].push_back(city_name_to_number[input_edges[i].first]);
    }
    Graph graph(n, edges);
//    graph.print();
    auto ans = graph.best_solution();
    std::cout << ans.size() << '\n';
    for (int i: ans)
        std::cout << cities[i] << " ";
    std::cout << '\n';
    return 0;
}