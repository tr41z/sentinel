#include "include/db.h"
#include "include/flow.h"
#include "include/sniffer.h"
#include <stdio.h>
#include <thread>

void terminate_and_save_flows(sqlite3 *db);

int main() {
  sqlite3 *db;
  char *home_dir = get_home_dir();
  connect_db(home_dir, &db);

  // Create a thread to run terminate_and_save_flows
  std::thread terminate_thread(terminate_and_save_flows, db);

  // Start the sniffer in the main thread (or in another thread if needed)
  start_multithreaded_sniffing();

  terminate_thread.join();
  return 0;
}