/*
 * fast < input.txt
 *
 * Compute and plot all line segments involving 4 points in input.txt
 */

#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include <unordered_map>
#include "point.h"
#include "window.h"

using namespace std;

// bool operator!= (Point const& lhs, Point const& rhs){
//     return lhs.x != rhs.x && lhs.y != rhs.y;
// }

bool in_line(vector<Point> const& Points, Point const& compare){
    for (Point const& p : Points)
        if( p.x == compare.x && p.y == compare.y) return true;
    return false;
}

int partition(vector<Point>& vec, int low, int high, Point const& origin) {

    // Selecting the last element as the pivot
    double pivot = origin.slopeTo(vec[high]);

    // Index of the element smaller than the pivot
    int i = low - 1;

    for (int j = low; j < high; j++) {

        // If the current element is smaller than
        // or equal to the pivot
        if (origin.slopeTo(vec[j]) <= pivot) {
            i++;
            swap(vec[i], vec[j]);
        }
    }

    // Place the pivot in its correct position
    swap(vec[i + 1], vec[high]);

    // Return the partition index
    return i + 1;
}

void quickSort(vector<Point>& vec, int low, int high, Point const& origin) {

    // Base case
    if (low < high) {

        // Partition the array
        int pi = partition(vec, low, high, origin);

        // Sort elements before the pivot
        quickSort(vec, low, pi - 1, origin);

        // Sort elements after the pivot
        quickSort(vec, pi + 1, high, origin);
    }
}

void find_line(int const N, vector<Point> const& points, double const tolerance,  WindowPtr const& window){
    vector<Point> used_points{}; 
    
    for (int o{0}; o < N; ++o){
        // cout << "----new o ----" << endl;
        // if (in_line(used_points, points.at(o))) continue;
        // cout << "test1" << endl;
        // PolarSorter slope_for_point{points.at(o)};
        vector<Point> slope_sorted{points};
        
        quickSort(slope_sorted, 0, slope_sorted.size()-1, points.at(o));
        
        unordered_map<double, vector<Point>> points_on_line{};
        double temp_key{ };
        if (!slope_sorted.empty()){
            temp_key = (points.at(o)).slopeTo(slope_sorted.at(0));
            // cout << "test3" << endl;
            points_on_line.emplace(temp_key, vector<Point>{slope_sorted.at(o), slope_sorted.at(0)});
        }    
        // cout << "test4" << endl;
        
        // count()
        // find()
        for (size_t i{1}; i < slope_sorted.size(); ++i){
            if (sameSlope(points.at(o).slopeTo(slope_sorted.at(i-1)), points.at(o).slopeTo(slope_sorted.at(i)), tolerance)){
                vector <Point> tmp_points_vector{ points_on_line.find(temp_key)->second };
                tmp_points_vector.push_back(slope_sorted.at(i));
                points_on_line.erase(temp_key);
                points_on_line.emplace(temp_key, tmp_points_vector);
                // used_points.push_back(slope_sorted.at(i-1));
                // used_points.push_back(slope_sorted.at(i));
            }
            else{
                temp_key = points.at(o).slopeTo(slope_sorted.at(i));
                points_on_line.emplace(temp_key, vector<Point>{points.at(o), slope_sorted.at(i)});
            }
        }
        // cout << "test4" << endl;
        

        for (const std::pair<double, vector<Point>> P : points_on_line)
            if(P.second.size() >= 4){
                window->draw_line(P.second);

                // cout << "--------" << endl;
            }
        // cout << "test5" << endl;
        // cout << "-----------------" << endl;
    }
}


int main(int argc, const char* argv[]) {
    WindowPtr window = create_window(argc, argv);
    if (!window)
        return 1;

    // The array of points
    vector<Point> points;

    // Read tolerance from cin
    double tolerance{};
    cin >> tolerance;

    // Read points from cin
    int N{};
    cin >> N;

    for (int i{0}; i < N; ++i) {
        double x{}, y{};
        cin >> x >> y;
        points.push_back(Point{x, y});
    }
    // kör vårt program
    auto begin = chrono::high_resolution_clock::now();
    find_line(N, points, tolerance, window);

    // draw points to screen all at once
    window->draw_points(points);



    /////////////////////////////////////////////////////////////////////////////
    // Draw any lines that you find in 'points' using the function 'window->draw_line'.
    /////////////////////////////////////////////////////////////////////////////

    auto end = chrono::high_resolution_clock::now();
    cout << "Computing line segments took "
         << std::chrono::duration_cast<chrono::milliseconds>(end - begin).count()
         << " milliseconds." << endl;

    // wait for user to terminate program
    window->run();

    return 0;
}
