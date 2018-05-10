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

bool on_border(int height, int width, point pos){
  return (pos.x == 0 || pos.y == 0) ||
         (pos.x == width-1 || pos.y == height-1);
}

void clear_maze(int height, int width){
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      maze[i][j] = 2;
    }
  }
}

void initialize(int height, int width){
  srand(time(0));
}

bool check_for_space_forward(int height, int width, point pos, int direction){
  int offset_x[] = {0, -1, 0, 1},
      offset_y[] = {-1, 0, 1, 0};

  if(on_border(height, width, pos)){
    return 0;
  }

  pos.x += offset_x[direction];
  pos.y += offset_y[direction];

  return maze[pos.y][pos.x] == 0;
}

point check_for_adjecent_wall(int height, int width, int i, int j){
  int offset_x[] = {0, -1, 0, 1},
      offset_y[] = {-1, 0, 1, 0};
  point pos;

  for(int k = 0; k < 4; k++){
    pos.x = j + offset_x[k];
    pos.y = i + offset_y[k];

    if(maze[pos.y][pos.x] == 1 &&
       check_for_space_forward(height, width, pos, k)){
      return pos;
    }
  }

  return point(-1, -1);
}

point search_space(int height, int width){
  point pos(-1, -1);
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      if(maze[i][j] == 2){
        pos = check_for_adjecent_wall(height, width, i, j);

        if(pos.x != -1){
          return pos;
        }
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

void fill_in_spot(point spot){
  maze[spot.y][spot.x] = 1;
}

void fill_in_unused(int height, int width){
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      if(maze[i][j] == 2){
        fill_in_spot(point(j, i));
      }
    }
  }
}

void change_direction(int &direction){
  int last = direction;

  direction += 1 + ((-2) * rand()%2);
  direction %= 4;
}

void move(int height, int width, point &pos, int direction){
  int offset_x[] = {0, -1, 0, 1},
      offset_y[] = {-1, 0, 1, 0};
  point next(pos.x + offset_x[direction],
             pos.y + offset_y[direction]);

  if(maze[next.y][next.x] == 2){
    pos = next;
  }
}

void attempt_to_move(int height, int width, point &pos, int &direction,
                     int length, int min_length, bool &end, bool &retry){
  int move_attempts = 0;
  point next = pos;

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

  pos = next;
}

void main_path(int height, int width, int min_length, bool &retry){
  point pos(0, rand()%(height-2)+1), last = pos, to_be_bordered(-1, -1);
  int length = 1, direction = 2;
  bool end = 0;

  clear_spot(height, width, pos);

  retry = 0;
  while(!end){
    if(rand()%4 >= 3){
      change_direction(direction);
    }

    attempt_to_move(height, width, pos, direction,
                    length, min_length, end, retry);

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

  cout << "try length: " << length << '\n';

  if(length < min_length){
    retry = 1;
  }
}

void secondary_path(int height, int width, point pos,
                    int min_length, int max_length){
  bool end = 0, useless_flag;
  point next, last = pos, to_be_bordered(-1, -1);
  int direction = rand()%4, length = 0;

  clear_spot(height, width, pos);

  while(!end){
    if(rand()%4 >= 1){
      change_direction(direction);
    }

    attempt_to_move(height, width, pos, direction,
                    length, min_length, end, useless_flag);

    if(!on_border(height, width, pos)){
      clear_spot(height, width, pos);
    }
    if(to_be_bordered.x != -1){
      border_spot(height, width, to_be_bordered);
    }

    to_be_bordered = last;
    last = pos;

    length++;

    if(on_border(height, width, pos) || length >= max_length || (length >= min_length && rand()%10 >= 7)){
      end = 1;
    }
  }

  border_spot(height, width, last);
  border_spot(height, width, pos);

  cout << "secondary length: " << length << '\n';
}

void generate(int height, int width, int main_min_length, int secondary_min_length, int secondary_max_length){
  point start;
  bool try_main_path = 1;

  while(try_main_path){
    clear_maze(height, width);
    main_path(height, width, main_min_length, try_main_path);
  }

  start = search_space(height, width);
  while(start.x != -1){
    secondary_path(height, width, start,
                   secondary_min_length, secondary_max_length);

    start = search_space(height, width);
  }

  fill_in_unused(height, width);
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
  int height = 20, width = 30, main_min_length = 100,
      secondary_min_length = 1, secondary_max_length = 10;

  initialize(height, width);

  generate(height, width, main_min_length,
           secondary_min_length, secondary_max_length);

  print(height, width);
}
