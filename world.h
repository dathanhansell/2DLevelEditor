const int GRID_WIDTH = 20;
const int GRID_HEIGHT = 20;

// Define the different block types
enum BlockType {
    BLOCK_TYPE_1,
    BLOCK_TYPE_2,
    BLOCK_TYPE_3
};

class World {
public:
    World() {
        // Initialize the grid with empty cells
        for (int x = 0; x < GRID_WIDTH; x++) {
            for (int y = 0; y < GRID_HEIGHT; y++) {
                m_grid[x][y] = -1;
            }
        }
    }

    void setBlock(int x, int y, BlockType blockType) {
        m_grid[x][y] = blockType;
    }

    BlockType getBlock(int x, int y) {
        return static_cast<BlockType>(m_grid[x][y]);
    }

private:
    int m_grid[GRID_WIDTH][GRID_HEIGHT];
};
