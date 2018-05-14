#include <iostream>

#define HEIGHT 10
#define WIDTH 10

using namespace std;

class node{
public:
  bool up, down, left, right;

  node(){
    up = down = left = right = 0;
  }

  bool used(){
    return up || down || left || right;
  }
};

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

int main(){
  node maze[HEIGHT][WIDTH];

  maze[1][0].left = 1;
  maze[1][0].right = 1;
  maze[1][1].left = 1;

  print(maze, HEIGHT, WIDTH);
}
