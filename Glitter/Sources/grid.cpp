#include "grid.h"

#include <iostream>
#include <set>

Grid::Grid(const CoordVec& in_coords) {
    for (const auto& coord : in_coords) {
        addAlive(coord);
    }
}

Cell& Grid::getCell(const Coord& in_coord)
{
    if (in_coord.first < 0 || in_coord.second < 0
        || in_coord.first >= width || in_coord.second >= height) {
        return off_grid_cell;
    }
    return cell_grid.at(in_coord.second * width + in_coord.first); 
}

const Cell& Grid::getCell(const Coord& in_coord) const
{
    if (in_coord.first < 0 || in_coord.second < 0
        || in_coord.first >= width || in_coord.second >= height) {
        return off_grid_cell;
    }
    return cell_grid.at(in_coord.second * width + in_coord.first); 
}

Grid::Coord Grid::getAdjacentCoord(const Coord& in_coord, RelativeDir dir) {
    Coord relative_coord = in_coord;
    switch (dir) {
    case NORTHWEST: 
        relative_coord.first -= 1;
        relative_coord.second += 1;
        break;

    case NORTH:
        relative_coord.second += 1;
        break;

    case NORTHEAST:
        relative_coord.first += 1;
        relative_coord.second += 1;
        break;

    case EAST:
        relative_coord.first += 1;
        break;

    case SOUTHEAST:
        relative_coord.first += 1;
        relative_coord.second -= 1;
        break;

    case SOUTH:
        relative_coord.second -= 1;
        break;

    case SOUTHWEST:
        relative_coord.first -= 1;
        relative_coord.second -= 1;
        break;

    case WEST:
        relative_coord.first -= 1;
        break;
    }
    return relative_coord;
}

// Function that will accept a coordinate and a relative direction
// and will return the cell at the relative location to the input 
// coordinate. 
// Returns cell with OFF_GRID if the relative cell is off the grid.
Cell &Grid::getAdjacentCell(const Coord &in_coords, RelativeDir dir) {
    Coord relative_coord = getAdjacentCoord(in_coords, dir);
    return getCell(relative_coord);
}

// Checks every adjacent cell for alive cells
int Grid::getAdjacentAliveCount(const Coord &in_coords)
{
    int alive_count = 0;
    for (int dir = NORTHWEST; dir <= WEST; dir++) {
        CellState current_state = getAdjacentCell(in_coords, static_cast<RelativeDir>(dir)).state;
        if (current_state == CellState::ALIVE) {
            alive_count++;
        }
    }
    return alive_count;
}

// Using the rules of Conway's Game of Life, the cell at the given 
// coordinates will either die, stay alive, or become alive 
// next turn.
CellState Grid::next_state(const Coord &in_coords) {
    //Rules:
    //  1. A live cell with two or three lives neighbors lives
    //  2. A dead cell with three live neighbors becomes a live cell
    //  3. All other cells die or stay dead.

    Cell cell_to_check{ getCell(in_coords) };
    // check all adjacent cells for alive cells
    int alive_count = getAdjacentAliveCount(in_coords);
    // default CellState is DEAD
    CellState checked_cell_state = CellState::DEAD;

    if (alive_count >= 2 && alive_count <= 3 && cell_to_check.state == CellState::ALIVE) {
        checked_cell_state = CellState::ALIVE;
    }
    else if (alive_count == 3 && cell_to_check.state == CellState::DEAD) {
        checked_cell_state = CellState::ALIVE;
    }

    return checked_cell_state;
}

// The given cell at the coordinate is made alive.
// The coordinates are then added to alive_cells
// and the dead cells around are added to alive_candidates.
void Grid::addAlive(const Coord& in_coord) {
    getCell(in_coord).state = CellState::ALIVE;
    alive_cells.insert(in_coord);
    alive_candidates.erase(in_coord);

    for (int dir = NORTHWEST; dir <= WEST; dir++) {
        Coord current_coord = getAdjacentCoord(in_coord, static_cast<RelativeDir>(dir));
        if (getState(current_coord) == CellState::DEAD) {
            alive_candidates.insert(current_coord);
        }
    }
}

void Grid::killCell(const Coord& in_coord) {
    getCell(in_coord).state = CellState::DEAD;
    alive_cells.erase(in_coord);

    for (int dir = NORTHWEST; dir <= WEST; dir++) {
        Coord current_coord = getAdjacentCoord(in_coord, static_cast<RelativeDir>(dir));
        if (getState(current_coord) == CellState::DEAD
            && getAdjacentAliveCount(current_coord) == 0) {
            alive_candidates.erase(current_coord);
        }
    }
}

void Grid::update() {
    std::set<Coord> to_alive{};
    std::set<Coord> to_kill{};
    for (const auto& coord : alive_candidates) {
        CellState state = next_state(coord);
        if (state == CellState::ALIVE) {
            to_alive.insert(coord);
        }
    }
    for (const auto& coord : alive_cells) {
        CellState state = next_state(coord);
        if (state == CellState::DEAD) {
            to_kill.insert(coord);
        }
    }
    for (const auto& coord : to_kill) {
        killCell(coord);
    }
    for (const auto& coord : to_alive) {
        addAlive(coord);
    }
}

void Grid::printState() {
    std::cout << " ";
    for (int z = 0; z < width; z++) {
        std::cout << z;
    }
    std::cout << std::endl;
    for (int y = 0; y < height; y++) {
        std::cout << y;
        for (int x = 0; x < width; x++) {
            CellState current_state = getState({ x, y });
            if (current_state == CellState::ALIVE) {
                std::cout << "x";
            }
            else {
                std::cout << "o";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
