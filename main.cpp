#include <iostream>
#include <thread>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <mutex>
#include <condition_variable>

using namespace std;
mutex mtx, landing_mtx, service_mtx;
condition_variable cv_landing, cv_service, cv_landed;


const int aircrafts = 5;
const int ground_service = 2;
const int runways = 2;
const int parking_spaces = 2;
const int capability = 150;
int fuel = 99;



int num_of_aircraft[aircrafts];
int aircrafts_in_the_air = 0;
int aircrafts_on_ground = 0;


int landed=0;
int departed=0;


int circling = 0;
bool landing = false; // 1 odbywa sie ladowanie / 0 można podchodzić do lądowania
int taxiing = 0;
bool service = false; // 1 serwis naziemny pracuje / 0 serwis nie pracuje
int departure = 1;

//---------------------------

void showStatus(){

  mtx.lock();

  mvprintw(2,5,"Aircrafts: %d", aircrafts);
  mvprintw(2,30,"In the air: %d", aircrafts_in_the_air);
  mvprintw(2,45,"On ground: %d", aircrafts_on_ground);

  mvprintw(6,5,"circling:  %d", circling );
  mvprintw(7,5,"landing:   %d", landing );
  mvprintw(8,5,"taxiing:   %d", taxiing );
  mvprintw(9,5,"service:   %d", service );
  mvprintw(10,5,"departure: %d", departure );

  
  
  refresh();
  mtx.unlock();

  sleep(1);

}


void showTerminal1(){

  mvprintw(14,180,"--------Terminal1--------");
  mvprintw(15,180,"|");
  mvprintw(16,180,"|");
  mvprintw(17,180,"|");
  mvprintw(18,180,"|");
  mvprintw(19,180,"|");
  mvprintw(20,180,"|");
  mvprintw(21,180,"|");
  mvprintw(22,180,"|");
  mvprintw(23,180,"|");
  mvprintw(24,180,"-------------------------");
}


void showTerminal2(){

  mvprintw(30,180,"--------Terminal2--------");
  mvprintw(31,180,"|");
  mvprintw(32,180,"|");
  mvprintw(33,180,"|");
  mvprintw(34,180,"|");
  mvprintw(35,180,"|");
  mvprintw(36,180,"|");
  mvprintw(37,180,"|");
  mvprintw(38,180,"|");
  mvprintw(39,180,"|");
  mvprintw(40,180,"-------------------------");
}

void showRunway(){
  mvprintw(22,60,"--------------------------------------------------------------------");
  mvprintw(24,60,"");
  mvprintw(25,60,"");
  mvprintw(26,60,"");
  mvprintw(27,60,"");
  mvprintw(28,60,"");
  mvprintw(29,60,"");
  mvprintw(30,60,"");
  mvprintw(31,60,"");
  mvprintw(32,60,"--------------------------------------------------------------------");
}

void showStatusAircraft(int tID, int position, int fuel){
  
  //std::lock_guard<std::mutex> mtx1_lock(mtx);
  mtx.lock();
  {
  if(position == 0)
  {
    if(fuel<=0){
      attron(COLOR_PAIR(1));
    }
	      mvprintw(7+tID*8,35,"     @@  Flying...");
	      mvprintw(8+tID*8,35,"@     @@@");
        mvprintw(9+tID*8,35,"@@     @@@@");
        mvprintw(10+tID*8,35,"@@-%d-@@@-%d-@@@@@@>", fuel, tID);   
        mvprintw(11+tID*8,35,"@@     @@@@");
        mvprintw(12+tID*8,35,"@     @@@");
	      mvprintw(13+tID*8,35,"     @@");
        refresh();
	
    }
  
  else
  {
    for(int i=35;i<145;i++)
        {
            for(int j=7;j<=13;j++)
            {
               mvprintw(j+tID*8,i-1,"                                     ");
            }
              mvprintw(7+tID*8,i,"     @@");
	            mvprintw(8+tID*8,i,"@     @@@");
              mvprintw(9+tID*8,i,"@@     @@@@");
              mvprintw(10+tID*8,i,"@@@@@@@@@@@@@@@@@>");
              mvprintw(11+tID*8,i,"@@     @@@@");
              mvprintw(12+tID*8,i,"@     @@@");
	            mvprintw(13+tID*8,i,"     @@");
            
            
            refresh();
            usleep(20000);
        }

    

     //mtx.unlock();
  }
  }
  
mtx.unlock();
}

void showStatusAircraft2(int tID, int position){
  mtx.lock();
  if(position >= 35)
  {
    for(int i=109;i>0;i--)
        {
            for(int j=7;j<=13;j++)
            {
               mvprintw(j+tID*8,35+i-1,"                                 ");
            }
        mvprintw(7+tID*8,35+i,"           @@");
	      mvprintw(8+tID*8,35+i,"         @@@     @");
        mvprintw(9+tID*8,35+i,"       @@@@     @@");
        mvprintw(10+tID*8,35+i,"<@@@@@@@@@@@@@@@@@");
        mvprintw(11+tID*8,35+i,"       @@@@     @@");
        mvprintw(12+tID*8,35+i,"         @@@     @");
	      mvprintw(13+tID*8,35+i,"           @@");
        refresh();
	usleep(20000);
  }
  }
  
  mtx.unlock();
}

void clearStatusAircraft(int tID, int i){
  mtx.lock();
  if(i == 1){
              mvprintw(7+tID*8,144,"       ");
	            mvprintw(8+tID*8,144,"         ");
              mvprintw(9+tID*8,144,"           ");
              mvprintw(10+tID*8,144,"                  ");
              mvprintw(11+tID*8,144,"           ");
              mvprintw(12+tID*8,144,"         ");
	            mvprintw(13+tID*8,144,"       ");
              refresh();
  }
  else
  {
              mvprintw(7+tID*8,35,"                       ");
	            mvprintw(8+tID*8,35,"                       ");
              mvprintw(9+tID*8,35,"                       ");
              mvprintw(10+tID*8,35,"                      ");
              mvprintw(11+tID*8,35,"                      ");
              mvprintw(12+tID*8,35,"                      ");
	            mvprintw(13+tID*8,35,"                      ");
              refresh();
              
  }
  mtx.unlock();
}

void showService(int tID)
{
  mtx.lock();
  

  for (int i=0; i<100; i++){
       attron(COLOR_PAIR(2));

              mvprintw(7+tID*8,144,"     @@   Refueling...");
	            mvprintw(8+tID*8,144,"@     @@@");
              mvprintw(9+tID*8,144,"@@     @@@@");
              mvprintw(10+tID*8,144,"@@@@@@@@@@@@@@@@@>");
              mvprintw(11+tID*8,144,"@@     @@@@");
              mvprintw(12+tID*8,144,"@     @@@");
	            mvprintw(13+tID*8,144,"     @@");
              refresh();
               attron(COLOR_PAIR(3));

              usleep(20000);
              
              mvprintw(7+tID*8,144,"                       ");
	            mvprintw(8+tID*8,144,"                       ");
              mvprintw(9+tID*8,144,"                       ");
              mvprintw(10+tID*8,144,"                      ");
              mvprintw(11+tID*8,144,"                      ");
              mvprintw(12+tID*8,144,"                      ");
	            mvprintw(13+tID*8,144,"                      ");
              refresh();
              usleep(20000);
              
}
mtx.unlock();
}

// void landingg(){
//   landing++;
//   mvprintw(30,60,"fadfafdas: %d", landing);
  
// }

void showStats(int tID, int fuel, bool landing, bool landed){

        // mtx.lock();
	      mvprintw(7+tID*8,10,"tID: %d",tID);
        mvprintw(8+tID*8,10,"fuel %d",fuel);
        mvprintw(9+tID*8,10,"Landing: %", landing);
        mvprintw(10+tID*8,10,"Landed: %bool", landed);
        refresh();
        // mtx.unlock();
	
}

void startThreadFuel(int tID){

while(1){
  

  
  // usleep(rand()%1000000);
  // mtx.lock();
  // fuel--;
  // //showStatusAircraft(tID, 0, fuel);
  // mvprintw(15+tID,5,"%d Fuel: %d %", tID, fuel);
  // mvprintw(10+tID*8,35,"@@-%d-@@@-%d-@@@@@@>", fuel, tID);

  // mtx.unlock();
  // //refresh();
  // //usleep(1000000);
  // mvprintw(10+tID*8,35,"@@-%d-@@@-%d-@@@@@@>", fuel, tID);
}
}

void startThreadAircraft(int tID){

  //startThreadFuel(tID);

  while(1){
    usleep(rand()%1000000);
    fuel--;
    showStatusAircraft(tID, 0, fuel);
    
    std::unique_lock<mutex> lck(landing_mtx);
    //while(landing){
       //usleep(rand()%10000);
     cv_landing.wait(lck, []{
       
       return landing == false && landed < 2;
     });
     
        landing = true;
        landed++;
        showStatusAircraft(tID, 1, fuel);
        usleep(100000);
        clearStatusAircraft(tID, 1);
lck.unlock();
cv_landing.notify_all();

        showService(tID);
        
        showStatusAircraft2(tID, 144);
        //mtx.lock();
        fuel=99;
        //mtx.unlock();
        
        clearStatusAircraft(tID, 0);
        showStatusAircraft(tID, 0, fuel);

    }
}



void startControlTower(int tID){

  while(1)
  {
    std::lock_guard<std::mutex> lck(landing_mtx);
    landing = false;
    landed--;

    //usleep(100000);
    cv_landing.notify_all();
    
  }
}

void startGroundService(int tID){
  while(1)
  {
    std::unique_lock<std::mutex> lck(mtx);
    service = true;
    cv_service.notify_one();
  }
}






int main()
{

  initscr();
	nodelay(stdscr, TRUE);

  refresh();
  srand(time(NULL));
  clear();
  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_BLUE, COLOR_BLACK);
  init_pair(3, COLOR_WHITE, COLOR_BLACK);
  init_pair(4, COLOR_BLACK, COLOR_BLUE);
  init_pair(5, COLOR_BLACK, COLOR_MAGENTA);
  init_pair(6, COLOR_BLACK, COLOR_WHITE);
  init_pair(7, COLOR_BLACK, COLOR_CYAN);
  
  curs_set(0);

  

//----------------THREADS------------------
thread aircraft[aircrafts];
thread ground_service[ground_service];
thread controlTower[1];
thread groundService[1];
thread terminal[2];
thread ThreadFuel[aircrafts];

//-----------------------------------------
  
  

  showStatus();
  showTerminal1();
  showTerminal2();
  //showRunway();

//-------------OPEN-THREADS----------------

  for (int i =0; i<aircrafts; i++)
  {
    aircraft[i] = thread(startThreadAircraft, i);
  }

  controlTower[0] = thread(startControlTower,0);
  ground_service[0] = thread(startGroundService, 0);
  terminal[0] = thread(showTerminal1);
  terminal[1] = thread(showTerminal2);

  for (int i =0; i<aircrafts; i++)
  {
    ThreadFuel[i] = thread(startThreadFuel, i);
  }
  

//-----------------------------------------







//-------------JOIN-THREADS----------------

for (int i=0; i<aircrafts; i++)
{
  aircraft[i].join();
}

controlTower[0].join();
ground_service[0].join();
terminal[0].join();
terminal[1].join();

for (int i=0; i<aircrafts; i++)
{
  ThreadFuel[i].join();
}
//-----------------------------------------


  

  endwin();
  return 0;

}
