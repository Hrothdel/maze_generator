#include <iostream>
#include <time.h>
#include <cstdlib>

#define HEIGHT 10
#define WIDTH 10

using namespace std;

class node{
public:
  bool up, down, left, right;

  node(){
    initialize();
  }

  bool used(){
    return up || down || left || right;
  }

  void initialize(){
    up = down = left = right = 0;
  }
};

class point{
public:
  int x, y;

  point(int a, int b){
    x = a;
    y = b;
  }

  point(){
    x = 0;
    y = 0;
  }

  bool equal_to(point b){
    return x == b.x && y == b.y;
  }
};

void initialize(){
  srand(time(0));
}

void clear_maze(node maze[HEIGHT][WIDTH], int height, int width){
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      maze[i][j].initialize();
    }
  }
}

void print(node maze[HEIGHT][WIDTH], int height, int width){
  string space = "  ", wall = "# ", unused = ". ";

  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      cout << wall;

      if(maze[i][j].up){
        cout << space;
      } else {
        cout << wall;
      }
    }
    cout << wall;

    cout << '\n';

    for(int j = 0; j < width; j++){
      if(maze[i][j].left){
        cout << space;
      } else {
        cout << wall;
      }

      if(maze[i][j].used()){
        cout << space;
      } else {
        cout << unused;
      }
    }
    if(maze[i][width-1].right){
      cout << space;
    } else {
      cout << wall;
    }

    cout << '\n';
  }

  for(int j = 0; j < width; j++){
    cout << wall;
    if(maze[height-1][j].down){
      cout << space;
    } else {
      cout << wall;
    }
  }
  cout << wall;

  cout << '\n';
}

bool on_border(int height, int width, point pos){
  return (pos.x == 0 || pos.x == width-1) &&
         (pos.y == 0 || pos.y == height-1);
}

bool in_bounds(int height, int width, point pos){
  return (pos.x >= 0 && pos.x < width) &&
         (pos.y >= 0 && pos.y < height);
}

void move(point &pos, int direction){
  int offset_x[] = {-1,  0, 1, 0},
      offset_y[] = { 0, -1, 0, 1};

  pos.x += offset_x[direction];
  pos.y += offset_y[direction];
}

void clear_picked_directions(bool picked_directions[4]){
  for(int i = 0; i < 4; i++){
    picked_directions[i] = 0;
  }
}

bool remaining_directions(bool picked_directions[4]){
  for(int i = 0; i < 4; i++){
    if(!picked_directions[i]){
      return 1;
    }
  }

  return 0;
}

void connect(node maze[HEIGHT][WIDTH], point a, point b, int direction){
  switch(direction){
    case 3: { // down
      maze[a.y][a.x].down = 1;
      maze[b.y][b.x].up = 1;
      break;
    }
    case 2: { // right
      maze[a.y][a.x].right = 1;
      maze[b.y][b.x].left = 1;
      break;
    }
    case 1: { // up
      maze[a.y][a.x].up = 1;
      maze[b.y][b.x].down = 1;
      break;
    }
    case 0: { // left
      maze[a.y][a.x].left = 1;
      maze[b.y][b.x].right = 1;
      break;
    }
  }
}

bool generate_path(node maze[HEIGHT][WIDTH], int height, int width, point start,
                   int min_length, int max_length, bool main_path){
  bool success = 0, end = 0, picked_directions[4];
  point pos = start, last = pos;
  int length = 1, direction = 2, attempts = 0;

  while(!end){
    attempts = 0;
    clear_picked_directions(picked_directions);

    while(pos.equal_to(last) && attempts < 5 && !end){
      move(pos, direction);
      attempts++;

      if((!in_bounds(height, width, pos)) || (maze[pos.y][pos.x].used())){
        pos = last;
        picked_directions[direction] = 1;

        while(remaining_directions(picked_directions) &&
              picked_directions[direction]){
          direction = rand()%4;
        }
      }

      if(attempts > 4){
        end = 1;
      }
    }

    if(!end){
      connect(maze, last, pos, direction);

      if(rand()%10 >= 7){
        direction += 1 + ((-2) * (rand()%2));

        if(direction < 0){
          direction = 3;
        }

        direction %= 4;
      }

      length++;

      if(main_path && (length >= min_length && length <= max_length) && on_border(height, width, pos)){
        end = 1;
        success = 1;
      }

      last = pos;
    }
  }

  return success;
}

void generate(node maze[HEIGHT][WIDTH], int height, int width,
              int main_min_length, int main_max_length,
              int secondary_min_length, int secondary_max_length){
  point start(0, rand()%height + 1);
  int length = 0;
  bool success = 0;

  while(!success){
    cout << "try\n";
    success = generate_path(maze, height, width, start, main_min_length, main_max_length, 1);

    if(!success){
      clear_maze(maze, height, width);
    }
  }
}

int main(){
  node maze[HEIGHT][WIDTH];
  int main_min_length = 50, main_max_length = 300,
      secondary_min_length = 1, secondary_max_length = 30;
  initialize();

  generate(maze, HEIGHT, WIDTH,
           main_min_length, main_max_length,
           secondary_min_length, secondary_max_length);

  print(maze, HEIGHT, WIDTH);
}
