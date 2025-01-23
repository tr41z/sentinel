export interface Flow {
    id: number;
    source_ip: string;
    destination_ip: string;
    source_ports: string;
    destination_ports: string;
    protocol: number;
    total_bytes: number;
    rate: number;
    average_packet_size: number;
    total_packet_count: number;
    start_time: number;
    last_updated_time: number;
    duration: number;
}