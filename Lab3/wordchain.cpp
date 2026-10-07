#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::queue;
#include <algorithm>
using namespace std;

#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
using namespace std;
// Typ som används för ordlistan. Den definieras med en typedef här så att du enkelt kan ändra
// representationen av en ordlista utefter vad din implementation behöver. Funktionen
// "read_questions" skickar ordlistan till "find_shortest" och "find_longest" med hjälp av denna
// typen.

typedef vector<string> Dictionary;

// struct Node {
//     // vector <string > edges;
//     bool visited = false;
//     string previous;
// };
// vector <Node > graph;

struct Node {
    // vector <string > edges;
    bool visited = false;
    int previous ;
};
// vector <Node > graph;

// static vector<string> follow_parents(vector<Node> const& node_vector, const Dictionary &dict, Node const& end_node, const string &from, bool const reverse = false) {
//     Node current_node{end_node};
//     vector<string> chain{};

//     while (current_node.previous != from){
//         if (!reverse)
//             chain.insert(chain.begin(), current_node.previous);
//         else 
//             chain.push_back(current_node.previous);
//         current_node = node_vector.at(distance(dict.begin(), find(dict.begin(), dict.end(), current_node.previous)));
//     }
//     return chain;
// }

static vector<string> follow_parents(vector<Node> const& node_vector, const Dictionary &dict, Node const& end_node, const int from_idx, bool const reverse = false) {
    Node current_node{end_node};
    vector<string> chain{};

    while (current_node.previous != from_idx){
        if (!reverse)
            chain.insert(chain.begin(), dict.at(current_node.previous));
        else 
            chain.push_back(dict.at(current_node.previous));
        current_node = node_vector.at(current_node.previous);
    }
    return chain;
}

int word_diff(string const& word1, string const& word2) {
    int diff {};
    for (size_t i{}; i < word2.size(); ++i){
        if (word2.at(i) != word1.at(i)) ++diff;
        if (diff > 1) return diff;
    }
    return diff;
}

vector<int> get_neighbors(const Dictionary &dict, const string &from, const vector<Node> & node_vector){
    vector<int> neighbors{};
    
    for (size_t i{}; i < dict.size(); ++i){
        if (node_vector.at(i).visited == false and (1 == word_diff(dict[i], from))){
            neighbors.push_back(i);
        }  
    }
    return neighbors;
}

vector<string> bfs(const Dictionary &dict, string const& from, string const& to) {
    vector<Node> node_vector(dict.size(), Node{false, int{}});
    vector<string> chain { };
    queue<int> q;
    auto from_idx = distance(dict.begin(),find(dict.begin(), dict.end(), from));

    q.push(from_idx);
    while (!q.empty()) {
        int curr = q.front();
        q.pop();
  
        vector<int> neighbors {get_neighbors(dict, dict[curr], node_vector)};
        // if (neighbors.empty()) return chain;
        //titar på grannoderna
        if (!neighbors.empty()){
            for (const int neighbor: neighbors) {
                if (!node_vector.at(neighbor).visited) {
                    node_vector.at(neighbor).previous = curr;
                    node_vector.at(neighbor).visited = true;
                    q.push(neighbor);
                    if (dict[neighbor] == to) {
                        chain = follow_parents(node_vector, dict, node_vector.at(neighbor) , from_idx);
                        chain.push_back(dict[neighbor]);
                        return chain;
                    }
                }
            } 
        }   
    }
    return chain;
}


vector<string> bfs_longest(const Dictionary &dict, string const& from) {
    vector<Node> node_vector(dict.size(), Node{false, int{}}); // graph
    vector<string> chain{ };
    vector<vector<string>> all_paths{ };
    queue<int> q;
    bool foud_end { };
    auto from_idx = distance(dict.begin(),find(dict.begin(), dict.end(), from));
    
    q.push(from_idx);
    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        
        if (foud_end){
            // auto it = distance(dict.begin(), find(dict.begin(), dict.end(), curr));
            vector <string> tmp { follow_parents(node_vector, dict, node_vector.at(curr) , from_idx, true) };
            tmp.push_back(from);
            all_paths.push_back(tmp);
        }
        // vector<string> neighbors {get_neighbors(dict, curr)};
         
        vector<int> neighbors {get_neighbors(dict, dict[curr], node_vector)};
        // if (neighbors.empty()) return continue;
        if (!neighbors.empty()){
            for (const int neighbor: neighbors) {

                // auto it = distance(dict.begin(), find(dict.begin(), dict.end(), neighbor));

                if (!node_vector.at(neighbor).visited) {
                    node_vector.at(neighbor).previous = curr;
                    node_vector.at(neighbor).visited = true;
                    q.push(neighbor);
                }
            }  
        } else {
            foud_end = true;
        }
    }

    for (size_t i{1}; i < all_paths.size(); ++i )
        if (all_paths.at(i) > chain)
            chain = all_paths.at(i);
    
    return chain;
}




/**
 * Hitta den kortaste ordkedjan från 'first' till 'second' givet de ord som finns i
 * 'dict'. Returvärdet är den ordkedja som hittats, första elementet ska vara 'from' och sista
 * 'to'. Om ingen ordkedja hittas kan en tom vector returneras.
 */

vector<string> find_shortest(const Dictionary &dict, const string &from, const string &to) {
    vector<string> result{ };
    result = bfs(dict,from,  to);
    return result;
}

/**
 * Hitta den längsta kortaste ordkedjan som slutar i 'word' i ordlistan 'dict'. Returvärdet är den
 * ordkedja som hittats. Det sista elementet ska vara 'word'.
 */
vector<string> find_longest(const Dictionary &dict, const string &word) {
    vector<string> result(1, word);
    // cout << "TODO: Implement me!" << endl;
    result = bfs_longest(dict, word);
    return result;
}

/**
 * Läs in ordlistan och returnera den som en vector av orden. Funktionen läser även bort raden med
 * #-tecknet så att resterande kod inte behöver hantera det.
 */
Dictionary read_dictionary() {
    string line;
    vector<string> result;
    while (std::getline(std::cin, line)) {
        if (line == "#")
            break;

        result.push_back(line);
    }

    return Dictionary(result.begin(), result.end());
}

/**
 * Skriv ut en ordkedja på en rad.
 */
void print_chain(const vector<string> &chain) {
    if (chain.empty())
        return;

    vector<string>::const_iterator i = chain.begin();
    cout << *i;

    for (++i; i != chain.end(); ++i)
        cout << " -> " << *i;

    cout << endl;
}

/**
 * Skriv ut ": X ord" och sedan en ordkedja om det behövs. Om ordkedjan är tom, skriv "ingen lösning".
 */
void print_answer(const vector<string> &chain) {
    if (chain.empty()) {
        cout << "ingen lösning" << endl;
    } else {
        cout << chain.size() << " ord" << endl;
        print_chain(chain);
    }
}

/**
 * Läs in alla frågor. Anropar funktionerna "find_shortest" eller "find_longest" ovan när en fråga hittas.
 */
void read_questions(const Dictionary &dict) {
    string line;
    while (std::getline(std::cin, line)) {
        size_t space = line.find(' ');
        if (space != string::npos) {
            string first = line.substr(0, space);
            string second = line.substr(space + 1);
            vector<string> chain = find_shortest(dict, first, second);

            cout << first << " " << second << ": ";
            print_answer(chain);
        } else {
            vector<string> chain = find_longest(dict, line);

            cout << line << ": ";
            print_answer(chain);
        }
    }
}

int main() {
    Dictionary dict = read_dictionary();
    read_questions(dict);
    return 0;
}
