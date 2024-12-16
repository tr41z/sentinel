#include "include/db.h"
#include "include/sniffer.h"
#include "include/flow.h"
#include <stdio.h>
#include <thread>  

int main() {
  char *home_dir = get_home_dir();
  connect_db(home_dir);

  // Create a thread to run terminate_and_save_flows
  std::thread terminate_thread(terminate_and_save_flows);
  
  // Start the sniffer in the main thread (or in another thread if needed)
  start_multithreaded_sniffing();

  // Wait for the terminate_and_save_flows thread to finish
  terminate_thread.join();  

  return 0;
}
