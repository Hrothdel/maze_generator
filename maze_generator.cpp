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

void clear_maze(int n){
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      maze[i][j] = 2;
    }
  }
}

void init(int n){
  //clear_maze(n);
  srand(time(0));
}

point search_space(int n){
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      if(maze[i][j] == 2){
        return point(i, j);
      }
    }
  }

  return point(-1, -1);
}

void border_spot(int n, point spot){
  point border_point;
  int offset_x[] = { 0, -1, -1, -1, 0, 1, 1,  1},
      offset_y[] = {-1, -1,  0,  1, 1, 1, 0, -1};

  for(int i = 0; i < 8; i++){
    border_point.x = spot.x + offset_x[i];
    border_point.y = spot.y + offset_y[i];

    if(maze[border_point.x][border_point.y] == 2){
      maze[border_point.x][border_point.y] = 1;
    }
  }
}

void clear_spot(int n, point spot){
  maze[spot.x][spot.y] = 0;
}

void change_direction(int &direction){
  int last = direction;

  direction += 1 + ((-2) * rand()%2);
  direction %= 4;
}

// bool try_direction(int n, point &pos, int direction){
//
// }

void move(int n, point &pos, int direction, bool &end, bool &retry){
  int offset_x[] = {0, -1, 0, 1},
      offset_y[] = {-1, 0, 1, 0};
  point next(pos.x + offset_x[direction], pos.y + offset_y[direction]);

  if(maze[next.x][next.y] == 2){
    pos = next;
  } else {
    direction = (direction + 2) % 4;
    next.x = pos.x + offset_x[direction];
    next.y = pos.y + offset_y[direction];

    if(maze[next.x][next.y] == 2){
      pos = next;
    } else {
      cout << next.x << ' ' << next.y << '\n';
      cout << "!\n";
      end = 1;
      retry = 1;
    }
  }
}

bool on_border(int n, point pos){
  return (pos.x == 0 || pos.y == 0) || (pos.x == n-1 || pos.y == n-1);
}

void main_path(int n, int min_length, bool &retry){
  point pos(rand()%(n-2)+1, 0), last = pos, to_be_bordered(-1, -1), next;
  int length = 1,
      direction = 2,
      offset_x[] = {0, -1, 0, 1},
      offset_y[] = {-1, 0, 1, 0},
      move_attempts = 0;
  bool end = 0;

  retry = 0;

  clear_spot(n, pos);
  while(!end){
    if(rand()%4 >= 3){
      cout << "was " << direction << ' ';
      change_direction(direction);
      cout << "changed direction to: " << direction << '\n';
    }

    next = pos;

    while(pos.equal_to(next) && !end){
      move_attempts++;
      move(n, next, direction, end, retry);

      if(length < min_length && on_border(n, next)){
        next = pos;
        cout << "happened\n";

        change_direction(direction);
      }

      if(move_attempts > 4){
        end = 1;
        cout << "stuck\n";
        retry = 1;
      }
    }
    move_attempts = 0;
    pos = next;

    clear_spot(n, pos);
    if(to_be_bordered.x != -1){
      border_spot(n, to_be_bordered);
    }

    to_be_bordered = last;
    last = pos;

    length++;

    if((pos.x == 0 || pos.x == n-1) || (pos.y == 0 || pos.y == n-1)){
      end = 1;
    }
  }

  border_spot(n, last);
  border_spot(n, pos);

  cout << "length: " << length << '\n';

  if(length < min_length){
    retry = 1;
  }
}

void generate(int n, int min_length){
  point start;
  bool try_main_path = 1;

  while(try_main_path){
    clear_maze(n);
    main_path(n, min_length, try_main_path);
  }

  start = search_space(n);
}

void print(int n){
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
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
  int n = 25, min_length = 100;

  init(n);

  generate(n, min_length);

  print(n);
}
