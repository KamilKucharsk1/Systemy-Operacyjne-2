#include <iostream>
#include <thread>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <mutex>
#include <condition_variable>

using namespace std;
mutex mtx, landing_mtx, service_mtx, fuel_mtx;
condition_variable cv_landing, cv_service, cv_landed, cv_fuel;


const int aircrafts = 5;
const int ground_service = 2;
const int runways = 2;
const int parking_spaces = 2;
const int capability = 150;
int fuel = 99;
int Fuel[5];





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

  // mvprintw(6,5,"circling:  %d", circling );
  // mvprintw(7,5,"landing:   %d", landing );
  // mvprintw(8,5,"taxiing:   %d", taxiing );
  // mvprintw(9,5,"service:   %d", service );
  // mvprintw(10,5,"departure: %d", departure );

  
  
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
  
        mtx.lock();

  {
  if(position == 0)
  {
    if(Fuel[tID]<=0){
      attron(COLOR_PAIR(1));
    }
	      mvprintw(7+tID*8,35,"     @@  Flying...");
	      mvprintw(8+tID*8,35,"@     @@@");
        mvprintw(9+tID*8,35,"@@     @@@@");
        mvprintw(10+tID*8,35,"@@-%d-@@@-%d-@@@@@@>", Fuel[tID], tID);   
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
  

  for (int i=0; i<10; i++){
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

              usleep(200000);
              
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


// void showStats(int tID, int fuel, bool landing, bool landed){

//         // mtx.lock();
// 	      mvprintw(7+tID*8,10,"tID: %d",tID);
//         mvprintw(8+tID*8,10,"fuel %d",Fuel[tID]);
//         mvprintw(9+tID*8,10,"Landing: %", landing);
//         mvprintw(10+tID*8,10,"Landed: %bool", landed);
//         refresh();
//         // mtx.unlock();
	
// }

void startThreadFuel(int tID){

while(1){

   usleep(/* rand()%*/100000);
   Fuel[tID]--;
  // service_mtx.lock();    //service_mtx
        mvprintw(7+tID*8,10,"tID: %d",tID);
        mvprintw(8+tID*8,10,"fuel %d",Fuel[tID]);
        refresh();
// service_mtx.unlock();  //service_mtx
  }
}

void startThreadAircraft(int tID){

  while(1){
    usleep(rand()%1000);
    //fuel--;
    showStatusAircraft(tID, 0, fuel);
    
    std::unique_lock<mutex> lck(landing_mtx);
     cv_landing.wait(lck, []{
       
       return (landing == false && landed <= 2);
     });
     
        landing = true;
        landed++;
        showStatusAircraft(tID, 1, fuel);
        usleep(100000);
        clearStatusAircraft(tID, 1);
        lck.unlock();
        cv_landing.notify_one();

        
        std::unique_lock<mutex> lck2(service_mtx);
        cv_service.wait(lck2, []{
       
       return (service == false);
     });
        service = true;
        showService(tID);
        

        showStatusAircraft2(tID, 144);
        fuel_mtx.lock();
        Fuel[tID] = 99;
        fuel_mtx.unlock();

        clearStatusAircraft(tID, 0);
        showStatusAircraft(tID, 0, fuel);
        lck2.unlock();
        cv_service.notify_one();
    }
}



void startControlTower(int tID){

  while(1)
  {
    std::lock_guard<std::mutex> lck(landing_mtx);
    landing = false;
    landed--;
    cv_landing.notify_all();
    
  }
}

void startGroundService(int tID){
  while(1)
  {
    std::lock_guard<std::mutex> lck(service_mtx);
    service = false;
    cv_service.notify_all();
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

for(int i =0; i<aircrafts; i++){
  Fuel[i] = 99;
}
  

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
