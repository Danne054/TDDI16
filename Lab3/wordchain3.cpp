#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

using namespace std;
// Typ som används för ordlistan. Den definieras med en typedef här så att du enkelt kan ändra
// representationen av en ordlista utefter vad din implementation behöver. Funktionen
// "read_questions" skickar ordlistan till "find_shortest" och "find_longest" med hjälp av denna
// typen.

typedef unordered_set<string> Dictionary;

static vector<string> follow_parents(unordered_map<string, string> const& graph, string const& end_node, const string &from, bool const reverse ) {
    vector<string> chain{ };
    
    string current_node { end_node };
    string previous_node{ graph.at(end_node) };
    chain.push_back(end_node);
    // chain.push_back(from);
    while (previous_node != from){
        
        if (!reverse)
            chain.insert(chain.begin(), previous_node);
        else 
            chain.push_back(previous_node);

        previous_node = graph.at(previous_node);
    }
       
    return chain;
}

vector<string> generate_word(const string &from){
    string word { from };
    vector<string> all_possible_neighbors{ };
    string alphabet {"abcdefghijklmnopqrstuvwxyz"};

    for (size_t j{}; j < from.size(); ++j){
        for (size_t i{static_cast<size_t>(from.at(j) -'a'+1)}; i  < alphabet.size() + (from.at(j) -'a'); ++i){
            word.at(j) = alphabet.at(i%alphabet.size());
            all_possible_neighbors.push_back(word);
        }
        word = from;
    }

    return all_possible_neighbors;
}   

vector<string> get_neighbors(const Dictionary &dict, const string &from, const unordered_map<string, bool> & visited){
    vector<string> all_possible_neighbors {generate_word(from)};
    vector<string> neighbors{};

    for (const string & w : all_possible_neighbors){
        auto it = visited.find(w);

        if (it != visited.end() && !it->second && dict.count(w) >= 1){
            neighbors.push_back(w);
        }
    }
    return neighbors;
}

void add_keys(unordered_map<string, string> &graph, unordered_map<string, bool> & visited, const Dictionary &dict, string const& from){
    graph.emplace(from, string{} );
    visited.emplace(from, true);
    for (const string& word : dict) {
        graph.emplace(word, string{} );
        visited.emplace(word, false);
    }
}

vector<string> bfs(const Dictionary &dict, string const& from, string const& to) {
    unordered_map<string, string> graph;
    unordered_map<string, bool> visited;

    add_keys(graph, visited, dict, from);

    vector<string> chain { };
    queue<string> q;


    // visited.at(from) = true;
    q.push(from);
    while (!q.empty()) {
        string curr = q.front();
        q.pop();
        
        vector<string> neighbors {get_neighbors(dict, curr, visited)};
        
        if (neighbors.empty()) return chain;
        
        //titar på grannoderna
        for (size_t i{}; i < neighbors.size(); ++i) {

            graph.at(neighbors.at(i)) = curr;
            visited.at(neighbors.at(i)) = true;
            q.push(neighbors.at(i));
            if (neighbors.at(i) == to) {
                chain = follow_parents(graph, neighbors.at(i) , from, false);
                chain.insert(chain.begin(), from);
                return chain;
            }

        }    
    }
    return chain;
}


vector<string> bfs_longest(const Dictionary &dict, string const& from) {
    unordered_map<string, string> graph;
    unordered_map<string, bool> visited;

    add_keys(graph, visited, dict, from);
    
    vector<vector<string>> all_paths{ };
    vector<string> chain { };
    
    // visited.at(from) = true;
    bool foud_end { };
    queue<string> q;
    q.push(from);

    while (!q.empty()) {
        foud_end = true;
        string curr = q.front();
        q.pop();

        vector<string> neighbors {get_neighbors(dict, curr, visited)};
        
        for (size_t i{}; i < neighbors.size(); ++i) {
            foud_end = false;
            graph.at(neighbors.at(i)) = curr;
            visited.at(neighbors.at(i)) = true;
            q.push(neighbors.at(i));

        }  
        
        if (foud_end){
            if (curr == from) return vector<string>{};
            vector <string> tmp { follow_parents(graph, curr , from, true) };
            tmp.push_back(from);
            all_paths.push_back(tmp);
        }
    }

    for (size_t i{0}; i < all_paths.size(); ++i )
        if (all_paths.at(i).size() >= chain.size())
            chain = all_paths.at(i);
    return chain;
}




/**
 * Hitta den kortaste ordkedjan från 'first' till 'second' givet de ord som finns i
 * 'dict'. Returvärdet är den ordkedja som hittats, första elementet ska vara 'from' och sista
 * 'to'. Om ingen ordkedja hittas kan en tom vector returneras.
 */

vector<string> find_shortest(const Dictionary &dict, const string &from, const string &to) {
    vector<string> result;
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