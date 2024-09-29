CREATE TABLE IF NOT EXISTS flows (
    id SERIAL PRIMARY KEY,
    source_ip VARCHAR(45), 
    source_port INT,
    destination_ip VARCHAR(45),
    destination_port INT,
    protocol INT,
    total_bytes INT,
    total_packet_count INT,
    start_time DATETIME,
    end_time DATETIME,
    duration TEXT
);
