CREATE TABLE IF NOT EXISTS flows (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    src_ip VARCHAR(45), 
    src_port_count INTEGER NOT NULL,
    dst_ip VARCHAR(45),
    dst_port_count INTEGER NOT NULL,
    protocol INT,
    total_bytes INT,
    total_packet_count INT,
    start_time INTEGER,
    last_updated_time INTEGER,  
    dur FLOAT
);
