#pragma once

#include <vector>
#include <utility>
#include <set>

enum class CellState {
    ALIVE,
    DEAD,
    OFF_GRID
};

struct Cell {
    CellState state;
    Cell(CellState state_in = CellState::DEAD) : state{ state_in }{};
};

class Grid {
    using Coord = std::pair<int, int>;
    using CoordVec = std::vector<Coord>;
    using CoordSet = std::set<Coord>;
private:
    const size_t width = 10; 
    const size_t height = 7;
    CoordSet alive_cells{};
    // dead cells around alive cells that may possibly be made alive
    CoordSet alive_candidates{};

    enum RelativeDir {
        NORTHWEST,
        NORTH,
        NORTHEAST,
        EAST,
        SOUTHEAST,
        SOUTH,
        SOUTHWEST,
        WEST
    };
    
    Cell off_grid_cell{ CellState::OFF_GRID };
    // initialize cell_grid with width*height count of dead cells.
    std::vector<Cell> cell_grid{width*height, {CellState::DEAD}};

    Cell& getCell(const Coord& in_coord);
    const Cell& getCell(const Coord& in_coord) const;

    Coord getAdjacentCoord(const Coord& in_coord, RelativeDir dir);
    Cell& getAdjacentCell(const Coord& in_coord, RelativeDir dir);
    int getAdjacentAliveCount(const Coord& in_coord);
    
    CellState next_state(const Coord & in_coord);
public:
    Grid() = default;
    // Constructor to initialize a grid with a given vector of pairs of alive coords
    Grid(const CoordVec &in_coords);

    // inline function to get cellstate from coord
    CellState getState(const Coord &in_coord) const { 
        return getCell(in_coord).state;
    }
    void addAlive(const Coord &in_coord);
    void killCell(const Coord& in_coord);
    void update();
    void printState();
};

//template<typename T> 
//class Fake2DVec : public std::vector<T> {
//    using CoordPair = std::pair<std::vector<T>::value_type, std::vector<T>::value_type>;
//public:
//    T& operator[](const CoordPair& in_coord) {
//        return std::vector<T>[in_coord.first * 1280 + in_coord.second];
//    }
//};
