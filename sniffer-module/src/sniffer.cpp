#include "include/sniffer.h"

char err_buff[PCAP_ERRBUF_SIZE];
pcap_if_t *interfaces, *temp;

// Function for each thread to start sniffing on a specific interface
void *sniffer_thread(void *args) {
  ThreadArgs *thread_args = (ThreadArgs *)args;
  char *device_name = thread_args->device_name;
  pcap_t *handle;
  struct pcap_pkthdr *header;
  const u_char *packet;
  time_t last_packet_time, current_time;

  // Open the device for sniffing
  handle = pcap_open_live(device_name, BUFSIZ, 1, 5, err_buff);
  if (!handle) {
    fprintf(stderr, "Error opening device %s: %s\n", device_name, err_buff);
    pthread_exit(NULL);
  }

  printf("Started sniffing on device: %s\n", device_name);

  // Initialize the last packet timestamp
  last_packet_time = time(NULL);

  while (1) {
    int res = pcap_next_ex(handle, &header, &packet);
    current_time = time(NULL);

    if (res == 1) {                    /* Packet successfully captured */
      last_packet_time = current_time; /* Update last packet time */
      packet_handler((u_char *)device_name, header, packet);
    } else if (res == 0) { /* Timeout (no packet in the interval) */
      // Check if timeout exceeded
      if (current_time - last_packet_time >= PACKET_ARRIVAL_THRESHOLD) {
        printf("No packets received on device %s for %d seconds. Stopping.\n",
               device_name, PACKET_ARRIVAL_THRESHOLD);
        break;
      }
    } else if (res == -1) { /* Error in capture */
      fprintf(stderr, "Error reading packets on device %s: %s\n", device_name,
              pcap_geterr(handle));
      break;
    } else if (res == -2) { /* Loop terminated */
      break;
    }
  }

  // Close the handle and clean up
  pcap_close(handle);
  printf("Stopped sniffing on device: %s\n", device_name);

  free(thread_args->device_name);
  free(thread_args);
  pthread_exit(NULL);
}

// Function to start sniffing on all available devices using threads
void start_multithreaded_sniffing() {
  pthread_t threads[20];
  int thread_count = 0;
  ThreadArgs *args;

  // Find all devices
  if (pcap_findalldevs(&interfaces, err_buff) == -1) {
    fprintf(stderr, "Error finding devices: %s\n", err_buff);
    return;
  }

  // Iterate over each device
  for (temp = interfaces; temp; temp = temp->next) {
    if (thread_count >= 20) {
      fprintf(stderr,
              "Maximum thread limit reached. Skipping extra devices.\n");
      break;
    }

    args = (ThreadArgs *)malloc(sizeof(ThreadArgs));
    if (!args) {
      fprintf(stderr, "Memory allocation failed for thread arguments.\n");
      continue;
    }
    args->device_name = strdup(temp->name);

    // Create a thread for each interface
    if (pthread_create(&threads[thread_count], NULL, sniffer_thread,
                       (void *)args) != 0) {
      fprintf(stderr, "Error creating thread for device %s\n", temp->name);
      free(args->device_name);
      free(args);
      continue;
    }

    thread_count++;
  }

  // Wait for all threads to finish
  for (int i = 0; i < thread_count; i++) {
    pthread_join(threads[i], NULL);
  }

  // Free device list
  pcap_freealldevs(interfaces);
  printf("All threads completed. Exiting.\n");
}