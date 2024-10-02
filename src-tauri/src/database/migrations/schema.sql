CREATE TABLE IF NOT EXISTS flows (
    id SERIAL PRIMARY KEY,
    source_ip VARCHAR(45), 
    source_port INT,
    destination_ip VARCHAR(45),
    destination_port INT,
    protocol INT,
    total_bytes INT,
    total_packet_count INT,

    rate FLOAT,
    source_load INT,
    destination_load INT,

    start_time DATETIME,
    last_updated_time DATETIME,
    duration TEXT
);
