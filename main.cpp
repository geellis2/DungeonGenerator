#include <iostream>
#include "beastiary.h"
#include <ctime>
using namespace std;

int main(int argc , char * argv[])
{
  if ( argc != 3 ){
    cout<<"usage: "<< argv[0] <<" <beastiary filename> <APL>" << endl;  
    return 0;
  }
  srand(time(0));
  Beastiary complete(argv[1]);
  complete.buildDungeon(stod(argv[2]));
  return 0;
}

