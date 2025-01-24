import { LucideIcon } from "lucide-react";

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

export interface StatCardProps {
    name: string;
    icon: LucideIcon;
    value: number | string;
    color: string;
    iconSize?: number; 
}

export interface DashboardPageProps {
    totalFlows: number;
    avgFlowSize: string;
    avgFlowRate: string;
    totalBytes: number;
    flows: Flow[];
}