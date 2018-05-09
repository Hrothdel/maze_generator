#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

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

int maze[100][100];

void clear_maze(int height, int width){
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      maze[i][j] = 2;
    }
  }
}

void init(int height, int width){
  //clear_maze(n);
  srand(time(0));
}

point search_space(int height, int width){
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      if(maze[i][j] == 2){
        return point(i, j);
      }
    }
  }

  return point(-1, -1);
}

void border_spot(int height, int width, point spot){
  point border_point;
  int offset_x[] = { 0, -1, -1, -1, 0, 1, 1,  1},
      offset_y[] = {-1, -1,  0,  1, 1, 1, 0, -1};

  for(int i = 0; i < 8; i++){
    border_point.x = spot.x + offset_x[i];
    border_point.y = spot.y + offset_y[i];

    if(maze[border_point.y][border_point.x] == 2){
      maze[border_point.y][border_point.x] = 1;
    }
  }
}

void clear_spot(int height, int width, point spot){
  maze[spot.y][spot.x] = 0;
}

void change_direction(int &direction){
  int last = direction;

  direction += 1 + ((-2) * rand()%2);
  direction %= 4;
}

void move(int height, int width, point &pos, int direction){
  int offset_x[] = {0, -1, 0, 1},
      offset_y[] = {-1, 0, 1, 0};
  point next(pos.x + offset_x[direction], pos.y + offset_y[direction]);

  if(maze[next.y][next.x] == 2){
    pos = next;
  }
}

bool on_border(int height, int width, point pos){
  return (pos.x == 0 || pos.y == 0) || (pos.x == width-1 || pos.y == height-1);
}

void main_path(int height, int width, int min_length, bool &retry){
  point pos(0, rand()%(height-2)+1), last = pos, to_be_bordered(-1, -1), next;
  int length = 1,
      direction = 2,
      offset_x[] = {0, -1, 0, 1},
      offset_y[] = {-1, 0, 1, 0},
      move_attempts = 0;
  bool end = 0;

  retry = 0;

  clear_spot(height, width, pos);
  while(!end){
    if(rand()%4 >= 3){
      // cout << "was " << direction << ' ';
      change_direction(direction);
      // cout << "changed direction to: " << direction << '\n';
    }

    next = pos;

    while(pos.equal_to(next) && !end){
      move_attempts++;
      if(move_attempts > 1){ // spiraly
        direction += 1;
        direction %= 4;
      }
      move(height, width, next, direction);

      if(length < min_length && on_border(height, width, next)){
        next = pos;
      }

      if(move_attempts > 4){
        end = 1;
        retry = 1;
      }
    }
    move_attempts = 0;
    pos = next;

    clear_spot(height, width, pos);
    if(to_be_bordered.x != -1){
      border_spot(height, width, to_be_bordered);
    }

    to_be_bordered = last;
    last = pos;

    length++;

    if(on_border(height, width, pos)){
      end = 1;
    }
  }

  border_spot(height, width, last);
  border_spot(height, width, pos);

  cout << "length: " << length << '\n';

  if(length < min_length){
    retry = 1;
  }
}

void generate(int height, int width, int min_length){
  point start;
  bool try_main_path = 1;

  while(try_main_path){
    clear_maze(height, width);
    main_path(height, width, min_length, try_main_path);
  }

  start = search_space(height, width);
}

void print(int height, int width){
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      if(maze[i][j] == 2){
        cout << '.';
      } else if(maze[i][j] == 1){
        cout << '#';
      } else {
        cout << ' ';
      }
      cout << ' ';
    }
    cout << '\n';
  }
}

int main(){
  int height = 20, width = 30, min_length = 100;

  init(height, width);

  generate(height, width, min_length);

  print(height, width);
}
