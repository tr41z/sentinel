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

interface Stats {
    name: string;
    value: number | string;
}

export interface ModuleStatCardProps {
    moduleName: string;
    icon: LucideIcon;
    color: string;
    statistics: Stats[];
}

export interface DashboardPageProps {
    totalFlows: number;
    avgFlowSize: string;
    avgFlowRate: string;
    totalBytes: number;
    flows: Flow[];
}

export interface HomePageProps {
    snifferStatus: string;
    snifferUptime: number;
    snifferErrorCount: number;
    flows: Flow[];
    bandwidth: number;
}