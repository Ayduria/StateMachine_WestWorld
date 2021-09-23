#include <fstream>
#include <time.h>
#include <thread>
#include <mutex>
#include <chrono>

#include "Locations.h"
#include "Miner.h"
#include "MinersWife.h"
#include "Drunkard.h"
#include "EntityManager.h"
#include "MessageDispatcher.h"
#include "misc/ConsoleUtils.h"
#include "EntityNames.h"

std::ofstream os;
std::mutex Mutex;
int threadActif = 0;

void entityUpdate(BaseGameEntity* entity) {
    int cpt = 0;

    Mutex.lock();
    threadActif++;
    Mutex.unlock();
    // S'assurer que les trois threads sont actifs avant de commencer
    while (threadActif != 3) {} 

    while (cpt < 30) {
        if (Mutex.try_lock()) {
            entity->Update();
            Dispatch->DispatchDelayedMessages();
            Mutex.unlock();
            cpt++;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }

}

int main()
{
//define this to send output to a text file (see locations.h)
#ifdef TEXTOUTPUT
  os.open("output.txt");
#endif

  //seed random number generator
  srand((unsigned) time(NULL));

  //create a miner
  Miner* Bob = new Miner(ent_Miner_Bob);

  //create his wife
  MinersWife* Elsa = new MinersWife(ent_Elsa);

  //create a drunkard
  Drunkard* Cornelius = new Drunkard(ent_Cornelius);

  //register them with the entity manager
  EntityMgr->RegisterEntity(Bob);
  EntityMgr->RegisterEntity(Elsa);
  EntityMgr->RegisterEntity(Cornelius);

  std::thread tMiner(entityUpdate, Bob);
  std::thread tMinersWife(entityUpdate, Elsa);
  std::thread tDrunkard(entityUpdate, Cornelius);

  tMiner.join();
  tMinersWife.join();
  tDrunkard.join();

  //tidy up
  delete Bob;
  delete Elsa;
  delete Cornelius;

  //wait for a keypress before exiting
  PressAnyKeyToContinue();
  return 0;
}






