import sys

def parse_grid(lines):
    grid = [line.rstrip('\n') for line in lines if line.strip()]
    rows = len(grid)
    cols = len(grid[0]) if rows > 0 else 0
    return grid, rows, cols

def is_open(grid, r, c, rows, cols):
    return 0 <= r < rows and 0 <= c < cols and grid[r][c] == '.'

def main():
    # Read grid from stdin or file
    if len(sys.argv) > 1:
        with open(sys.argv[1]) as f:
            lines = f.readlines()
    else:
        lines = sys.stdin.readlines()

    grid, rows, cols = parse_grid(lines)

    # Add all open points
    for r in range(rows):
        for c in range(cols):
            if grid[r][c] == '.':
                print(f"add {r},{c} {r} {c}")

    # 8 directions: N, NE, E, SE, S, SW, W, NW
    directions = [(-1,0), (-1,1), (0,1), (1,1), (1,0), (1,-1), (0,-1), (-1,-1)]

    # Link all open neighbors
    for r in range(rows):
        for c in range(cols):
            if grid[r][c] != '.':
                continue
            for dr, dc in directions:
                nr, nc = r + dr, c + dc
                if is_open(grid, nr, nc, rows, cols):
                    print(f"link {r},{c} {nr},{nc}")

if __name__ == "__main__":
    main()
